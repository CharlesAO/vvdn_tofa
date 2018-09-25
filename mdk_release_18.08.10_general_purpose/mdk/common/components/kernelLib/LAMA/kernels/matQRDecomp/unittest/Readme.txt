Matrix decomposition using the Householder method

Overview
==========
This is an implementation of the Householder algorithm for the QR Decomposition of a Matrix. 
The algorithm is based on roto-translations of the input matrix. It takes as input the matrix to be decomposed and outputs the Q and R matrices of the decomposition.

Algorithm Description
=====================
- having A as an input matrix, copy it into a temporary matrix called Z0 on which the computation takes place;
- iterate on the diagonal of Z0 and for each k iteration, compute the (k,k) minor of Z0 and copy it into Z0;
- take the k-th column of Z0 and calculate its vector norm;
- if the k element of the diagonal from A is negative, invert the sign of the vector norm;
- add the vector norm to the k-th element of the selected column and calculate another norm on it;
- divide the vector by the latest norm and obtain a vector called Reflection Vector;
- compute the Householder matrix from the reflection vector;
- multiply the Householder matrix with Z0 and that becomes the new Z0 for the next iteration;
- at the first iteration, copy the Householder matrix into Q;
- in the next iterations, Q becomes the matrix product between the Householder matrix and Q;
- after all the iterations, multiply Q with A and obtain matrix R;
- transpose matrix Q and that becomes the final form of matrix Q.

However, due to performance reasons, the code for the algorithm has been written in a refactorized way. 

Hardware needed
==================
This software should run on MV182 boards.

Build
==================
To build the project, please type "make help" and follow the steps suggested there.
Myriad2: "make clean" then "make all DBGI=jtag" if you run on board or "make all" if you run on simulator.


Setup
==================
To run the test, start moviDebugServer, then launch with "make run_test".


