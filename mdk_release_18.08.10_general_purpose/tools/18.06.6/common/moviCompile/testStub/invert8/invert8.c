/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2017-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   invert8.c
 *  Description:   Sample broken function to illustrate building a unit-test for
 *                 the 'testStub' framework.
 *
 *                 This function inverts the 1st 'nElems' of the array pointed
 *                 to by 'pSrc' and placed the inverted values in the
 *                 corresponding element of the array pointerd to by 'pDst'.
 *
 *                 However, it has been deliberately poisoned to have a wrong
 *                 result to demonstrate the construction of a bug-report unit-
 *                 test for 'moviCompile' using the 'testStub' framework.
 *  --------------------------------------------------------------------------- */


void invert8(unsigned char* pDst, const unsigned char* pSrc, int nElems) {
	// Perform the inversion - this bit is correct
	for (int i = 0; i < nElems; i++)
		pDst[i] = ~pSrc[i];

	// Deliberately "poison" the 3rd element
	pDst[2]++;
}
