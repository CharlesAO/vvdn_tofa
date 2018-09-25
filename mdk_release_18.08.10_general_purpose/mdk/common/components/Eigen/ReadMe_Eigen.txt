
Eigen is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.


Setup Description
=================
shave:
------The header files required to compile applications using Eigen are in the shave folder. 

EigenTests: 
------------The EigenTests folder contains the eigen tests and benchmark that were ported to myriad. 
------------The shared file are located in EigenTests/shared.
------------The individual tests are located in the EigenTests/tests.
------------The benchmark for eigen3 can be found in EigenBenchmark folder.

Tests that are currently working:
Eigen_array_of_string
Eigen_array_replicate
Eigen_array_reverse
Eigen_bandmatrix
Eigen_corners
Eigen_dense_storage
Eigen_determinant
Eigen_diagonal
Eigen_dontalign
Eigen_evaluators
Eigen_fastmath
Eigen_first_aligned
Eigen_geo_alignedbox
Eigen_geo_eulerangles
Eigen_geo_homogeneous
Eigen_geo_hyperplane
Eigen_geo_orthomethods
Eigen_geo_parametrizedline
Eigen_half_float
Eigen_is_same_dense
Eigen_jacobi
Eigen_mapped_matrix
Eigen_mapstaticmethods
Eigen_mapstride
Eigen_meta
Eigen_miscmatrices
Eigen_nullary
Eigen_packetmath
Eigen_permutationmatrices
Eigen_prec_inverse_4x4
Eigen_product_syrk
Eigen_product_trmm
Eigen_product_trmv
Eigen_rand
Eigen_ref
Eigen_resize
Eigen_rvalue_types
Eigen_selfadjoint
Eigen_sizeof
Eigen_sparseLM
Eigen_sparse_ref
Eigen_special_numbers
Eigen_stable_norm
Eigen_stddeque
Eigen_stddeque_overload
Eigen_stdlist
Eigen_stdlist_overload
Eigen_stdvector
Eigen_stdvector_overload
Eigen_unalignedassert
Eigen_unalignedcount
Eigen_visitor
Eigen_zerosized

Supported Platform
==================
MyriadX - This example works on MyriadX: ma2x5x and ma2480 silicon


Build
==================

------Go to EigenTests/tests then choose the test you want to build and enter the folder, open terminal, please type "make help" and follow the steps suggested there.
To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"
     Where {Myriad_version} may be ma2480.

Benchmark: 
---------Go to EigenBenchmark/BenchTestsEigen3, open terminal, please type "make help" and follow the steps suggested there.
- To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}" 
     Where {Myriad_version} may be ma2480.



Run
===
For running the tests and benchmark: "make run MV_SOC_REV={Myriad_version}".


Eigen on PC - Useful external links:
====================================
---Download the package: http://eigen.tuxfamily.org/index.php?title=Main_Page
---Documentation: http://eigen.tuxfamily.org/dox/
---Run tests: http://eigen.tuxfamily.org/index.php?title=Tests
---Run the benchmark suite: http://www.eigen.tuxfamily.org/index.php?title=How_to_run_the_benchmark_suite






