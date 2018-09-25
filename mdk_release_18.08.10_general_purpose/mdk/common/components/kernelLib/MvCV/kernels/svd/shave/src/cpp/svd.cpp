#include "svd.h"
#include "math.h"
#include <stdio.h> 
void mvcvSVD(float **A, float *S, float **V, int rank)
{

	int  i, j, k, EstColRank = rank, RotCount = rank, SweepCount = 0,  slimit = (rank<120) ? 30 : rank/4;
	float eps = (float) (1e-15);
	float	e2 = 10.0f*rank*eps*eps, tol = 0.1f*eps, vt, p, x0, y0, q, r, c0, s0, d1, d2;

	for (i = 0; i < rank; i++)	{
		for (j = 0; j < rank; j++)
			V[i][j] = 0.0; 
		V[i][i] = 1.0; 
	}

	while (RotCount != 0 && SweepCount++ <= slimit) {
		RotCount = EstColRank*(EstColRank-1)/2;
		for (j=0; j<EstColRank-1; j++) 
			for (k=j+1; k<EstColRank; k++) {
				p = q = r = 0.0;
				for (i=0; i<rank; i++) {
					x0 = A[i][j]; 
					y0 = A[i][k];
					p += x0*y0; 
					q += x0*x0; 
					r += y0*y0;
				}
				S[j] = q; S[k] = r;
				if (q >= r) {
					if (q<=e2*S[0] || fabs(p)<=tol*q)
						RotCount--;
					else {
						p /= q; 
						r = 1.0f-r/q; 
						vt = sqrt(4.0f*p*p+r*r);
						c0 = sqrt(0.5f*(1.0f+r/vt)); 
						s0 = p/(vt*c0);
						for (i=0; i<rank; i++) {
							d1 = A[i][j]; 
							d2 = A[i][k];
							A[i][j] = d1*c0+d2*s0; 
							A[i][k] = -d1*s0+d2*c0;
							d1 = V[i][j]; 
							d2 = V[i][k];
							V[i][j] = d1*c0+d2*s0; 
							V[i][k] = -d1*s0+d2*c0;
						}
					}
				}
				else {
					p /= r; 
					q = q/r-1.0f; 
					vt = sqrt(4.0f*p*p+q*q);
					s0 = sqrt(0.5f*(1.0f-q/vt));
					if (p<0.0) 
						s0 = -s0;
					c0 = p/(vt*s0);
					for (i=0; i<rank; i++) {
						d1 = A[i][j]; 
						d2 = A[i][k];
						A[i][j] = d1*c0+d2*s0; 
						A[i][k] = -d1*s0+d2*c0;
						d1 = V[i][j]; 
						d2 = V[i][k];
						V[i][j] = d1*c0+d2*s0; 
						V[i][k] = -d1*s0+d2*c0;
					}
				}
			}
               {
			while (EstColRank>2 && S[EstColRank-1]<=S[0]*tol+tol*tol) EstColRank--;
               }
	}
	for (i = 0; i < rank; i++)
	{
		S[i] = sqrt(S[i]);
	}
	
	//if (SweepCount > slimit)
	//  printf("Warning: Reached maximum number of sweeps (%d) in SVD routine...\n", slimit);
}
