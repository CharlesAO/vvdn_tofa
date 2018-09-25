// -*- C++ -*-
//===--------------------------- wctype.h ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

/*
    wctype.h synopsis

Macros:

    WEOF

Types:

    wint_t
    wctrans_t
    wctype_t

int iswalnum(wint_t wc);
int iswalpha(wint_t wc);
int iswblank(wint_t wc);  // C99
int iswcntrl(wint_t wc);
int iswdigit(wint_t wc);
int iswgraph(wint_t wc);
int iswlower(wint_t wc);
int iswprint(wint_t wc);
int iswpunct(wint_t wc);
int iswspace(wint_t wc);
int iswupper(wint_t wc);
int iswxdigit(wint_t wc);
int iswctype(wint_t wc, wctype_t desc);
wctype_t wctype(const char* property);
wint_t towlower(wint_t wc);
wint_t towupper(wint_t wc);
wint_t towctrans(wint_t wc, wctrans_t desc);
wctrans_t wctrans(const char* property);

*/

#include <__config>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

#include_next <wctype.h>

#if defined(_LIBCPP_INCLUDING_STDC_HEADER) && !defined(_LIBCPP_WCTYPE_H)
#define _LIBCPP_WCTYPE_H

#undef iswalnum
#undef iswalpha
#undef iswblank
#undef iswcntrl
#undef iswdigit
#undef iswgraph
#undef iswlower
#undef iswprint
#undef iswpunct
#undef iswspace
#undef iswupper
#undef iswxdigit
#undef iswctype
#undef wctype
#undef towlower
#undef towupper
#undef towctrans
#undef wctrans

#endif  // _LIBCPP_INCLUDING_STDC_HEADER && !_LIBCPP_WCTYPE_H
