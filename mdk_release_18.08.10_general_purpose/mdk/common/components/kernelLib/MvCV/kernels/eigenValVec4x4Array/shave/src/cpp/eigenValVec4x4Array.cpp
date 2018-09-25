#include "eigenValVec4x4Array.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


typedef float R;

#define n 4


void mvcvEigenValVec4x4Array( R** matrix, R* eigvals, R** eigvec, u32 num_matrix, float threshold)
{
R m2[n][n]; 
R eigvec1[n * n]; 
R eigvals1[n]; 
int z = 0;

for (z = 0; z <  (int)num_matrix; z++)
{

	{
		int i=n; 
		while(i--) 
		{
			
			int j=n; 
			while(j--) 
			{
				m2[i][j] = matrix[z][i * 4 +j];
				eigvec1[i * 4 + j] = i==j;
			}
		}
	}
	while(1)
	{
		R mod = 0; 
		int i=0, j=0;
		{
			int k=n; 
			while(k--)
			{
				int m=n; 
				while((--m)>k)
				{
				
					R q = fabs(m2[k][m]);
					if(q > mod) {mod=q; i=k; j=m;}
				}
			}
		}
		if(mod < threshold) break;

		
		R th = 0.5*atan(2*m2[i][j]/(m2[i][i] - m2[j][j]));

		
		{
			R c = cos(th), s = sin(th);
			
		
		
		
			
			
			{
				int k=n; 
				while(k--)
				{
					R t = c*m2[k][i] + s*m2[k][j];
					m2[k][j] = -s*m2[k][i] + c*m2[k][j]; 
					m2[k][i]=t;
				}
			}
			

			
			
			
			
			int k=n; 
			while(k--)
			{
				R t     =  c*m2[i][k]+s*m2[j][k];
				R w     =  c*eigvec1[i * 4 + k]+s*eigvec1[j * 4 + k];

				m2[j][k] = -s*m2[i][k]+c*m2[j][k];
				eigvec1[j * 4 + k] = -s*eigvec1[i * 4 + k]+c*eigvec1[j * 4 + k];

				m2[i][k] = t;
				eigvec1[i * 4 + k] = w;
			}

		}
	}
	{int j=n; while(j--) eigvals1[j] = m2[j][j];}
	float min = eigvals1[0];
	
	
	int indx = 0;
	
	if (eigvals1[1] < min )
	{
		min  = eigvals1[1];
		indx = 1;
	}
	
	if (eigvals1[2] < min )
	{
		min  = eigvals1[2];
		indx = 2;
	}
	
	if (eigvals1[3] < min )
	{
		min  = eigvals1[3];
		indx = 3;
	}
	eigvals[z] = min;
	eigvec[z][0] = eigvec1[indx * 4 + 0]; 
	eigvec[z][1] = eigvec1[indx * 4 + 1]; 
	eigvec[z][2] = eigvec1[indx * 4 + 2]; 
	eigvec[z][3] = eigvec1[indx * 4 + 3]; 
	

}
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvEigenValVec4x4Array_opt( R** matrix, R* eigvals, R** eigvec, u32 num_matrix, float threshold)
{
	R m2[n][n];  
	R eigvals1[n]; 

	float4 *in0 = (float4*)&matrix[0][0];
	float4 *in1 = (float4*)&matrix[0][4];
	float4 *in2 = (float4*)&matrix[0][8];
	float4 *in3 = (float4*)&matrix[0][12];
	float4 line0 = *in0;
	float4 line1 = *in1;
	float4 line2 = *in2;
	float4 line3 = *in3;

    float4* m2_line0 = (float4*)&m2[0];
    float4* m2_line1 = (float4*)((float*)m2_line0 + 4);
    float4* m2_line2 = (float4*)((float*)m2_line1 + 4);
    float4* m2_line3 = (float4*)((float*)m2_line2 + 4);

	*m2_line0 = line0;
	*m2_line1 = line1;
	*m2_line2 = line2;
	*m2_line3 = line3;
    R eigvec1[16] = {1, 0, 0, 0,
        		     0, 1, 0, 0,
                     0, 0, 1, 0,
                     0, 0, 0, 1};
	while(1)
	{
		R mod = 0; 
		int i = 0, j = 0;
		
		int k = n; 
		while(k--)
		{
			int m=n; 
			while((--m)>k)
			{
			
				R q = __builtin_shave_sau_abs_f32_r(m2[k][m]);
				if(q > mod) 
				{
					mod = q; 
					i = k;  
					j = m;
				}
			}
		}
		
		if(mod < threshold) break;

		
		R th = 0.5*atanf(2*m2[i][j]/(m2[i][i] - m2[j][j]));		
		R c = cosf(th);
		R s = sinf(th);
		
		float4* eigvec1Iptr = (float4*)&eigvec1[i<<2];
		float4* eigvec1Jptr = (float4*)&eigvec1[j<<2];
		
		for(k = 0; k < 4; k++)
		{
			R t = c*m2[k][i] + s*m2[k][j];
			m2[k][j] = c*m2[k][j] - s*m2[k][i]; 
			m2[k][i] = t;
		}		
	
		float4 cVec = c;
		float4 sVec = s;
		float4* m2Jptr = (float4*)&m2[j][0];		
		float4* m2Iptr = (float4*)&m2[i][0];
	    float4 m2J = *m2Jptr;
	    float4 m2I = *m2Iptr;
	    float4 eigvec1I = *eigvec1Iptr;
	    float4 eigvec1J = *eigvec1Jptr;
        float4 t = cVec * m2I + sVec * m2J;
        float4 w = cVec * eigvec1I + sVec * eigvec1J;
        
        *m2Jptr = cVec * m2J - sVec * m2I;
        *eigvec1Jptr = cVec * eigvec1J - sVec * eigvec1I;
        *m2Iptr = t;
        *eigvec1Iptr = w;
		
	}       

	for(int j = 0; j < 4; j++)
		eigvals1[j] = m2[j][j];

 	in0 = (float4*)&matrix[1][0];
	in1 = (float4*)&matrix[1][4];
	in2 = (float4*)&matrix[1][8];
	in3 = (float4*)&matrix[1][12];
	line0 = *in0;
	line1 = *in1;
	line2 = *in2;
	line3 = *in3;

    m2_line0 = (float4*)&m2[0];
    m2_line1 = (float4*)((float*)m2_line0 + 4);
    m2_line2 = (float4*)((float*)m2_line1 + 4);
    m2_line3 = (float4*)((float*)m2_line2 + 4);

	*m2_line0 = line0;
	*m2_line1 = line1;
	*m2_line2 = line2;
	*m2_line3 = line3;

	float min = eigvals1[0];	
	int indx = 0;
	
	if (eigvals1[1] < min )
	{
		min  = eigvals1[1];
		indx = 1;
	}
	
	if (eigvals1[2] < min )
	{
		min  = eigvals1[2];
		indx = 2;
	}
	
	if (eigvals1[3] < min )
	{
		min  = eigvals1[3];
		indx = 3;
	}
	
	float4* eigvec1Ptr = (float4*)&eigvec1[indx << 2];
    float4* eigvecZPtr = (float4*)&eigvec[0][0];
    float4 temp = *eigvec1Ptr;
    eigvals[0] = min; 

    *eigvecZPtr = temp;


	for (u32 z = 1; z <  num_matrix; z++)
	{

 		R eigvec1[n * n] = {1, 0, 0, 0,
        		            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1};

        in0 = (float4*)&matrix[z+1][0];
		in1 = (float4*)&matrix[z+1][4];
		in2 = (float4*)&matrix[z+1][8];
		in3 = (float4*)&matrix[z+1][12];
		
		while(1)
		{
			R mod = 0; 
			int i = 0, j = 0;
			
			int k = n; 
			while(k--)
			{
				int m=n; 
				while((--m)>k)
				{
				
					R q = __builtin_shave_sau_abs_f32_r(m2[k][m]);
					if(q > mod) 
					{
						mod = q; 
						i = k;  
						j = m;
					}
				}
			}
			
			if(mod < threshold) break;
	
			float diff = m2[i][i] - m2[j][j];
			R th = 0.5*atanf(2*m2[i][j]/diff);			
			
			R c = cosf(th);
			R s = sinf(th);
			
			float4* eigvec1Iptr = (float4*)&eigvec1[i<<2];
			float4* eigvec1Jptr = (float4*)&eigvec1[j<<2];
			
			for(k = 0; k < 4; k++)
			{
				R t = c*m2[k][i] + s*m2[k][j];
				m2[k][j] = c*m2[k][j] - s*m2[k][i]; 
				m2[k][i] = t;
			}			
		
			float4 cVec = c;
			float4 sVec = s;
			float4* m2Jptr = (float4*)&m2[j][0];		
			float4* m2Iptr = (float4*)&m2[i][0];

		    float4 m2J = *m2Jptr;
		    float4 m2I = *m2Iptr;
		    float4 eigvec1I = *eigvec1Iptr;
		    float4 eigvec1J = *eigvec1Jptr;

		    float4 cVecI = cVec * m2I;
		    float4 sVecJ = sVec * m2J;
		    float4 cVecEigI = cVec * eigvec1I;
		    float4 sVecEigJ = sVec * eigvec1J;
            float4 t = cVecI + sVecJ;
            float4 w = cVecEigI + sVecEigJ;
            *m2Jptr = cVec * m2J - sVec * m2I;
            *eigvec1Jptr = cVec * eigvec1J - sVec * eigvec1I;
            *m2Iptr = t;
            *eigvec1Iptr = w;			
		}

		for(int j = 0; j < 4; j++)
			eigvals1[j] = m2[j][j];

		line0 = *in0;
		line1 = *in1;
		line2 = *in2;
		line3 = *in3;

		float4* eigvec1Ptr = (float4*)&eigvec1[indx << 2];
        float4* eigvecZPtr = (float4*)&eigvec[z][0];

        m2_line0 = (float4*)&m2[0];
        m2_line1 = (float4*)((float*)m2_line0 + 4);
        m2_line2 = (float4*)((float*)m2_line1 + 4);
        m2_line3 = (float4*)((float*)m2_line2 + 4);

        float4 temp = *eigvec1Ptr;

		*m2_line0 = line0;
		*m2_line1 = line1;
		*m2_line2 = line2;
		*m2_line3 = line3;
       
        
        *eigvecZPtr = temp;
		
		float min = eigvals1[0];		
		int indx = 0;
		
		if (eigvals1[1] < min )
		{
			min  = eigvals1[1];
			indx = 1;
		}
		
		if (eigvals1[2] < min )
		{
			min  = eigvals1[2];
			indx = 2;
		}
		
		if (eigvals1[3] < min )
		{
			min  = eigvals1[3];
			indx = 3;
		}
        
        eigvals[z] = min;         
	}
}
#endif //MOVICOMPILE_OPTIMIZED
