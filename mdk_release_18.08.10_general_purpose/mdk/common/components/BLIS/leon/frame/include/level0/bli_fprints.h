/*

   BLIS    
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2014, The University of Texas

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    - Neither the name of The University of Texas nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef BLIS_FPRINTS_H
#define BLIS_FPRINTS_H

#ifdef LEON_BLIS
#define FPRINTF0(_file, _s)                   printf(_s)
#define FPRINTF1(_file, _s, _value0)          printf(_s, _value0)
#define FPRINTF2(_file, _s, _value0, _value1) printf(_s, _value0, _value1)
#define FFLUSH(_file)
#else
#define FPRINTF0(_file, _s)                   fprintf(file, _s)
#define FPRINTF1(_file, _s, _value0)          fprintf(file, _s, _value0)
#define FPRINTF2(_file, _s, _value0, _value1) fprintf(file, _s, _value0, _value1)
#define FFLUSH(_file)                         fflush(_file)
#endif

// prints
#define bli_sfprints( file, spec, x ) \
{ \
	FPRINTF1( file, spec, (x) ); \
}
#define bli_dfprints( file, spec, x ) \
{ \
	FPRINTF1( file, spec, (x) ); \
}
#define bli_cfprints( file, spec, x ) \
{ \
	FPRINTF1( file, spec, bli_creal(x) ); \
	FPRINTF0( file, " + " ); \
	FPRINTF1( file, spec, bli_cimag(x) ); \
	FPRINTF0( file, " " ); \
}
#define bli_zfprints( file, spec, x ) \
{ \
	FPRINTF1( file, spec, bli_zreal(x) ); \
	FPRINTF0( file, " + " ); \
	FPRINTF1( file, spec, bli_zimag(x) ); \
	FPRINTF0( file, " " ); \
}
#define bli_ifprints( file, spec, x ) \
{ \
	FPRINTF1( file, spec, (x) ); \
}

#endif
