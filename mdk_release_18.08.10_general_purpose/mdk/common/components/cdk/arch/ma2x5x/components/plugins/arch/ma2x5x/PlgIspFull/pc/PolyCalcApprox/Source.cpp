#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_PHASES      16

uint32_t policy = 0; //0:real 1:approx

//##################################################################################
//##################################################################################
//##################################################################################
//  float _sinfApproxOriginal(float x/*angle*/, int numTerms)
//  {
//     float val = 1;
//     for(int k = numTerms - 1; k >= 0; --k)
//        val = 1 - x*x/(2*k+2)/(2*k+3)*val;
//
//     return x * val;
//  }
//##################################################################################
#define NUM_TERMS 12
float factor[NUM_TERMS];

void sinCalcFactors()
{
  //Precompute offline
   printf("FACTORS: \n");
   for(int k=0; k<NUM_TERMS; k++)
   {
       factor[k] = 1.0f/(2*k+2)/(2*k+3);
       printf("0x%08x, \n", *(uint32_t*)&factor[k]);
   }
   printf("\n");
}
//##################################################################################
float _sinfApprox(float x/*angle*/)
{
   float val = 1.0f;
   float x2  = x*x;

   for(int k = NUM_TERMS - 1; k >= 0; --k)
      val = 1.0f - x2 * val * factor[k];

   return x * val;
}

//##################################################
// SINC taylor series
//   float val=1.0f;
//   for(int k = numTerms - 1; k >= 0; --k)
//     val = 1 - x*x/(2*k+2)/(2*k+3)*val;

float sincAprx(float x, int numTerms = 12)
{
    float x2  = x*x;
    float val = 1.0f;

    #if 0
      for(int k = NUM_TERMS - 1; k >= 0; --k)
       val = 1 - x2 * val * factor[k];

    #else //loop unrolling... a bit faster
      val = 1.0f - x2 * val * factor[11];
      val = 1.0f - x2 * val * factor[10];
      val = 1.0f - x2 * val * factor[ 9];
      val = 1.0f - x2 * val * factor[ 8];
      val = 1.0f - x2 * val * factor[ 7];
      val = 1.0f - x2 * val * factor[ 6];
      val = 1.0f - x2 * val * factor[ 5];
      val = 1.0f - x2 * val * factor[ 4];
      val = 1.0f - x2 * val * factor[ 3];
      val = 1.0f - x2 * val * factor[ 2];
      val = 1.0f - x2 * val * factor[ 1];
      val = 1.0f - x2 * val * factor[ 0];
    #endif

    return val;
}

//##################################################################################
//##################################################################################
//##################################################################################
//http://lab.polygonal.de/?p=205
float Sin(float x)
{
   float sin;

   //always wrap input angle to -PI..PI
   if(x < -3.14159265)
      x += 6.28318531;
   else{
   if (x >  3.14159265)
       x -= 6.28318531;
   }

//compute sine
  if (x < 0) {
    sin = 1.27323954 * x + .405284735 * x * x;

    if (sin < 0)  sin = .225 * (sin *-sin - sin) + sin;
    else          sin = .225 * (sin * sin - sin) + sin;
  }
  else
  {
    sin = 1.27323954 * x - 0.405284735 * x * x;
    if (sin < 0) sin = .225 * (sin *-sin - sin) + sin;
    else         sin = .225 * (sin * sin - sin) + sin;
  }

  return sin;
}

float sincValue(float x);
void genSincTable();
//##################################################################################
float sincWrap(float a)
{
    /*LOG*/ static FILE *log = NULL;
    /*LOG*/ if(log == NULL)
    /*LOG*/    log = fopen("sincArgs.log", "w");
    /*LOG*/ fprintf(log, "%f\n", a);
    /*LOG*/ fflush(log);

   switch(policy)
   {
    case 0: return  sinf      (a)/a; //REF
    case 1: return _sinfApprox(a)/a; //OK, same results
    case 2: return  Sin(a)       /a; //minor mismatches !!!
    case 3: return  sincValue(a);
    case 4: return  sincAprx(a);
   }
}

//##################################################################################
void calcCoeffs(uint8_t *coef, int n, int d, int *ksize)
{
    float   scale = (float)n / (float)d;
    int     ncoefs = 7 * n;     /* 7 taps, N phases */
    int     i, p, oIdx;
    float   f, sinc, window;
    float   fCutoff;
    float   tcoef[7*MAX_PHASES];
    float   sum;
    int     sumi;

    #define ZERO_COEF   64
    memset(coef, ZERO_COEF, sizeof(*coef)*8*MAX_PHASES);

    /* When downscaling, we want a lower cutoff frequency, to avoid aliasing */
    fCutoff = scale * 3;
    if (fCutoff < 1) {
        fCutoff = 1;
    }
    if (fCutoff > 3) {
        fCutoff = 3;
    }

    for (i = 0; i < ncoefs; i++) {
        f = (float)i / (ncoefs-1) * 2 * M_PI - M_PI;
        if (f == 0) {
            tcoef[i] = 1.0;
        } else {
            /* Lanczos-windowed Sinc function */
            sinc     = sincWrap(f*fCutoff);
            window   = sincWrap(f        );
            tcoef[i] = sinc * window;
        }
    }

    /*
     * Normalise the co-efficients for each phase so that they sum to 1.
     * For the hardware, we group the co-efficients into phases.  They're
     * in groups of 7, with each group followed by a padding element.
     * Co-efficients are also mapped to the range [-0.5, 1.49219], which
     * is stored as a uint8_t in the hardware registers, by scaling and
     * adding a bias of 64.
     */
    oIdx = 0;
    for (p = n-1; p >= 0; p--) {
        sum = 0;
        for (i = p; i < ncoefs; i += n) {
            sum += tcoef[i];
        }
        sumi = 0;
        for (i = p; i < ncoefs; i += n) {

            //coef[oIdx] = (tcoef[i] / sum) * 128 + 64 + .5;
            int curCoef = (tcoef[i] / sum) * 128 + 64 + .5f;
            assert(curCoef >=   0); //check it's in [0..255] range
            assert(curCoef <= 255);
            coef[oIdx] = curCoef;
            sumi += coef[oIdx]-64;
            if (coef[oIdx] < 0 || coef[oIdx] > 255) {
                fprintf(stderr, "Co-efficient out of range!\n");
                abort();
            }
            if ((++oIdx & 7) == 7) {
                /* Co-efficients are 8 element aligned */
                oIdx++;
            }
        }

       // if (sumi != 128) {
       //     /*
       //      * Due to rounding when converting to integer, co-efficients don't
       //      * sum to exactly 1.0, so perform adjustment.
       //      */
       //     coef[oIdx-5] += (128-sumi);
       // }

    }

    *ksize = 7;
    if (coef[0] == ZERO_COEF && coef[6] == ZERO_COEF) {
        *ksize -= 2;
    }
    if (coef[1] == ZERO_COEF && coef[5] == ZERO_COEF) {
        *ksize -= 2;
    }
}

//##########################################################################################
int maxDiffEver = 0;

void maxMismatch(uint8_t *a, uint8_t *b, uint32_t len, uint32_t *maxDiff, uint32_t *nDiffs)
{

    *maxDiff = 0;
    *nDiffs  = 0;

    int curDiff;

    for(int i=0; i<len; i++)
    {
        curDiff = abs(a[i]-b[i]);
        if(curDiff)
           (*nDiffs)++;

        if(*maxDiff < curDiff)
           *maxDiff = curDiff;
    }

    if(maxDiffEver < *maxDiff)
       maxDiffEver = *maxDiff;
}

//##########################################################################################
void logCoefs(uint8_t *coefs, uint32_t N, FILE *f)
{
    for(int p=0; p<N; p++)
    {
     fprintf(f, "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
             coefs[p*8+0],
             coefs[p*8+1],
             coefs[p*8+2],
             coefs[p*8+3],
             coefs[p*8+4],
             coefs[p*8+5],
             coefs[p*8+6],
             coefs[p*8+7]);
    }
    fprintf(f,"\n");
}

//##########################################################################################
int main()
{
    uint8_t coefs [8*MAX_PHASES];
    uint8_t coefs2[8*MAX_PHASES];
    int total, miss;
    int totMismatches = 0;
    int n, d, ksize;
    FILE *logA = fopen("kerGold.txt", "w");
    FILE *logB = fopen("kerLut.txt" , "w");

    sinCalcFactors();
    genSincTable();
    total = miss = 0;

    for (n = 1; n <= 16; n++)
    {
        for (d = 1; d <= 63; d++)
        {
            policy = 0; calcCoeffs(coefs,  n, d, &ksize);
            policy = 3; calcCoeffs(coefs2, n, d, &ksize);
            printf("N = %d, D = %d, ksize = %d\n", n, d, ksize);

            total++;
            if(memcmp(coefs, coefs2, sizeof(coefs)))
            {
               uint32_t maxDiff, nDiffs;
               maxMismatch(coefs, coefs2, n*8, &maxDiff, &nDiffs);
               logCoefs(coefs,  n, logA); fflush(logA);
               logCoefs(coefs2, n, logB); fflush(logB);
               printf("mismatch (max_diff = %d, num_Diffs= %d)!\n", maxDiff, nDiffs);
               if(maxDiff > 1)
                  exit(2);
               miss++;
               totMismatches += nDiffs;
            }
        }
    }

    fclose(logA);
    fclose(logB);

   return 0;
}