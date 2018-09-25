/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2017-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   invert8_unittest.c
 *  Description:   Sample unit-test demonstrating a bug for the 'testStub' project
 *  --------------------------------------------------------------------------- */

#define NELEMS (8)

extern void invert8(unsigned char* pDst, const unsigned char* pSrc, int nElems);

// Sample input data set to be inverted
static const unsigned char input[NELEMS] =
	{ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

// Expected result after calling 'invert8'
static const unsigned char expected[NELEMS] =
	{ 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F };

// A buffer for the actual results of calling 'invert8'
static unsigned char output[NELEMS] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int main() {
	// Call the function with the input data set
	invert8(output, input, NELEMS);

	// Check the actual output against the expected output
	for (int i = 0; i < NELEMS; i++)
		if (output[i] != expected[i])
			return 1; // Failure

	return 0; // Success
}
