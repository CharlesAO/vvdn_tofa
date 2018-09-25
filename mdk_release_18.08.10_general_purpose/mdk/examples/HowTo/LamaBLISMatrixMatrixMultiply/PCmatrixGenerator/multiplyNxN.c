#include <stdio.h>
#include <stdlib.h>


#define N_MAX 2048

float aGlob[N_MAX*N_MAX];
float bGlob[N_MAX*N_MAX];
float aGlobT[N_MAX*N_MAX];
float bGlobT[N_MAX*N_MAX];
float abGlob[N_MAX*N_MAX];
float abGlobT[N_MAX*N_MAX];

void genRandomNxN(float *a, int n, int seed, float low_num, float hi_num);
void multiplyNxN(float *a, float *b, float *ab, int n);
void tranpose(float *a, float *b, int size);

int main(int argc, char *argv[])
{

    int n = atoi(argv[1]);    
    int seed = atoi(argv[2]);

    genRandomNxN(aGlob, n, seed, -1.0, 1.0);
    genRandomNxN(bGlob, n, seed+99, -1.0, 1.0);

    multiplyNxN(aGlob, bGlob, abGlob, n);

    tranpose(abGlobT, abGlob, n);
    tranpose(aGlobT, aGlob, n);
    tranpose(bGlobT, bGlob, n);

    FILE *aFile;
    FILE *bFile;
    FILE *aFileT;
    FILE *bFileT;
    FILE *abFile;
    FILE *abTFile;

    aFile = fopen("a.bin", "wb");
    bFile = fopen("b.bin","wb");
    aFileT = fopen("aTranspose.bin", "wb");
    bFileT = fopen("bTranspose.bin","wb");
    abFile = fopen("ab.bin", "wb");
    abTFile = fopen("abTranspose.bin", "wb");

    fwrite (aGlob, sizeof(float), n*n, aFile);
    fwrite (bGlob, sizeof(float), n*n, bFile);
    fwrite (aGlobT, sizeof(float), n*n, aFileT);
    fwrite (bGlobT, sizeof(float), n*n, bFileT);
    fwrite (abGlob, sizeof(float), n*n, abFile);
    fwrite (abGlobT, sizeof(float), n*n, abTFile);

    fclose(aFile);
    fclose(bFile);
    fclose(aFileT);
    fclose(bFileT);
    fclose(abFile);
    fclose(abTFile);

    printf("Done matrix generating\n");
    return 0;
}

void multiplyNxN(float *a, float *b, float *ab, int n)

{
    int i, j, k;

    for(i = 0; i < n * n; i++)
    {
        ab[i] = 0.0F;
    }

     for( i = 0; i < n; ++i ) 
     {
        printf(". ");
        for ( j = 0; j < n; ++j )
         {
            for( k = 0; k < n; ++k ) 
            {
                ab[i*n+j] += a[i*n+k] * b[k*n+j];
            }
        }
    }
}

void tranpose(float *a, float *b, int size)
{
    int i, j;
    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            a[i*size+j] = b[j*size+i];
}

void genRandomNxN(float *a, int n, int seed, float low_num, float hi_num)
{
    int i;
    srand(seed);

    for(i = 0; i < n * n; i++)
    {
        float temp =(float)( rand() & 0x7FFF )/ 32768.0f;
        a[i] = low_num + temp * ( hi_num - low_num );

    }
}
