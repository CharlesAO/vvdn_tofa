///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <DrvLeonL2C.h>
#include <UnitTestApi.h>
#include <VcsHooksApi.h>
#include <DrvTimer.h>

#include "mv_types.h"
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define N_MAX 2048
#define SIZE 2048
#define DELTA 1.0

#include "blis.h"
#include "Defines.h"
#include "Helpers.h"

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
#ifdef LEON_MALLOC_ISSUE
/* actual pool sizes (bytes)
 for mc x kc blocks of A  136320
 for kc x nc panels of B  8458368
 for mc x nc panels of C  128
 ----------
 8594816
 The rest of DDR memory is used for leon_heap => */

#define LEON_HEAP  ( 54*1024*1024)
unsigned char DDR_LEON_HEAP ALIGNED(16) leon_heap[LEON_HEAP];
#endif

// 4: Static Local Data
// ----------------------------------------------------------------------------
float DDR_BSS matrixA[N_MAX * N_MAX];
float DDR_BSS matrixB[N_MAX * N_MAX];
float DDR_BSS matrixC[N_MAX * N_MAX];
float DDR_BSS matrixAB[N_MAX * N_MAX];

tyTimeStamp timer_data;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void createMatrixObj(dim_t gs, num_t dt, trans_t trans, char storage, dim_t m,
		dim_t n, obj_t* a);
int matrixCmp(float *actual, float *expected, int width, float delta);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

int main(void) {

	initClocksAndMemory();

	unitTestInit();
	unitTestVerbosity(VERBOSITY_ALL);

#ifdef SHAVE_BLIS
	printf("\nStart using %d SHAVEs!\n", numberShaves);
#else
	printf("\nStart using LEON only!\n");
#endif
#ifdef LEON_MALLOC_ISSUE
	bli_init_leon(leon_heap, LEON_HEAP);
#endif

	bli_init();

	dim_t m, n, k;
	trans_t transa, transb;
	obj_t kappa;
	obj_t alpha, a, b, beta, c;

	num_t datatype = BLIS_FLOAT;
	char storage = 'c';

	// Map the dimension specifier to actual dimensions.
	m = SIZE;
	n = SIZE;
	k = SIZE;

	// Map parameter characters to BLIS constants.
	transa = BLIS_NO_TRANSPOSE;
	transb = BLIS_NO_TRANSPOSE;

	// Create test scalars.
	bli_obj_scalar_init_detached(datatype, &kappa);
	bli_obj_scalar_init_detached(datatype, &alpha);
	bli_obj_scalar_init_detached(datatype, &beta);

	// Create test operands (vectors and/or matrices).
	createMatrixObj(32, datatype, transa, storage, m, k, &a);
	createMatrixObj(32, datatype, transb, storage, k, n, &b);
	createMatrixObj(32, datatype, BLIS_NO_TRANSPOSE, storage, m, n, &c);

	memcpy(a.buffer, matrixA, SIZE*SIZE*sizeof(float));
	memcpy(b.buffer, matrixB, SIZE*SIZE*sizeof(float));

	// Set alpha and beta.
	bli_setsc(1.0, 0.0, &alpha);
	bli_setsc(0.0, 0.0, &beta);

	// Apply the parameters.
	bli_obj_set_conjtrans(transa, a);
	bli_obj_set_conjtrans(transb, b);

	// General Matrix Multiply call and time counter
	DrvTimerStartTicksCount(&timer_data);
	bli_gemm(&alpha, &a, &b, &beta, &c);
	u64 cycles_elapsed;
	DrvTimerGetElapsedTicks(&timer_data,&cycles_elapsed);
	// Flush L2 cache
	DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, 0);
	asm volatile("flush");

	// Verify output
	unitTestAssert(matrixCmp(c.buffer, matrixAB, SIZE, DELTA));
	unitTestFinalReport();


	// Free the test objects.
	bli_obj_free(&a);
	bli_obj_free(&b);
	bli_obj_free(&c);
	printf("\nDone in %llu cycles\n", cycles_elapsed);
	return 0;
}

// Exit at first absdiff higher then delta
int matrixCmp(float *actual, float *expected, int width, float delta) {
	int i;
	for (i = 0; i < width * width; i++) {
		if (fabs(actual[i] - expected[i]) > delta)
		{
			printf("(actual val) %f (expected val) %f poz:%d\n", actual[i], expected[i], i);
			return 0;
		}
	}
	return 1;
}

void createMatrixObj(dim_t gs, num_t dt, trans_t trans, char storage, dim_t m,
		dim_t n, obj_t* a)
{
	dim_t m_trans = m;
	dim_t n_trans = n;
	dim_t rs_g;
	dim_t cs_g;

	// Apply the trans parameter to the dimensions (if needed).
	bli_set_dims_with_trans(trans, m, n, m_trans, n_trans);

	// In case of general strides, use the general stride spacing specified by the
	// user to create strides with a column-major tilt.
	rs_g = gs * 1;
	cs_g = gs * m_trans;

	if (storage == 'c')
		bli_obj_create(dt, m_trans, n_trans, 0, 0, a);
	else if (storage == 'r')
		bli_obj_create(dt, m_trans, n_trans, n_trans, 1, a);
		else if (storage == 'g')
			bli_obj_create(dt, m_trans, n_trans, rs_g, cs_g, a);
			else
			{
				printf("Invalid storage character: %c\n", storage);
			}
}
