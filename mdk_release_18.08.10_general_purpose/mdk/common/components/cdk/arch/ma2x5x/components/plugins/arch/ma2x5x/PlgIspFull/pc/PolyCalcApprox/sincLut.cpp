#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_SZ  1024
#define MAX_VAL  9.5f //max value 9.42

float sincLut[MAX_SZ];
float delta = MAX_VAL/MAX_SZ;

//#######################################################
void genSincTable()
{
   FILE *log    = fopen("sincTable.txt", "w");
   FILE *logHex = fopen("sincTable.hex", "w");

   for(int i=0; i<MAX_SZ; i++)
   {
     if(i==0) sincLut[i] = 1.0f;
     else     sincLut[i] = sinf(i*delta)/(i*delta); //sinf(9.12)/9.12 max value eventually...

     fprintf(log,    "%f\n",     sincLut[i]);

     if((i % 128)==0)
       fprintf(logHex, "\n");
     fprintf(logHex, "0x%08x, ", *(uint32_t*)&sincLut[i]);

   }
   fclose(log);
   fclose(logHex);
}

//#######################################################
float LinearInterpolate(
   float y1,float y2,
   float mu)
{
   return(y1*(1-mu)+y2*mu);
}
//#######################################################
//http://paulbourke.net/miscellaneous/interpolation/
float CosineInterpolate(
   float y1,float y2,
   float mu)
{
   float mu2;

   mu2 = (1-cosf(mu*M_PI))/2;
   return(y1*(1-mu2)+y2*mu2);
}
//#######################################################
/*
   Tension: 1 is high, 0 normal, -1 is low
   Bias: 0 is even,
         positive is towards first segment,
         negative towards the other
*/
float HermiteInterpolate(
   float y0,double y1,
   float y2,double y3,
   float mu,
   float tension,
   float bias)
{
   float m0,m1,mu2,mu3;
   float a0,a1,a2,a3;

   mu2 = mu * mu;
   mu3 = mu2 * mu;
   m0  = (y1-y0)*(1+bias)*(1-tension)/2;
   m0 += (y2-y1)*(1-bias)*(1-tension)/2;
   m1  = (y2-y1)*(1+bias)*(1-tension)/2;
   m1 += (y3-y2)*(1-bias)*(1-tension)/2;
   a0 =  2*mu3 - 3*mu2 + 1;
   a1 =    mu3 - 2*mu2 + mu;
   a2 =    mu3 -   mu2;
   a3 = -2*mu3 + 3*mu2;

   return(a0*y1+a1*m0+a2*m1+a3*y2);
}

//#######################################################
float sincValue(float x)
{
    float tr  = sinf(x)/x;

    x         = fabs(x);
    int p     = (int)(x*(MAX_SZ/MAX_VAL)+0.5f);
    float ret = sincLut[p]; //nearest neighbor

    return ret;
}