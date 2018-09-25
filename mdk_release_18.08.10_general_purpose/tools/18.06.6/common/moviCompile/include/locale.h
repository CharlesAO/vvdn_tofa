/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2017-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   locale.h
 *  Description:   Lightweight "C Locale" implementation of the ISO C '<locale.h>'
 *                 library interface.
 *  --------------------------------------------------------------------------- */

#ifndef _MV_LOCALE_H_
#define _MV_LOCALE_H_ (1)


#include <sys/__moviconfig.h>

#ifdef _MV_USE_SYS_LOCALE
#include_next <locale.h>
#else /* _MV_USE_SYS_LOCALE */
#include <xlocale.h>

#include <limits.h>		/* Required for CHAR_MAX */
#include <stddef.h>		/* Required for NULL */


/* ISO C'11 - 7.11 Localization <locale.h> */
#define LC_ALL			(LC_COLLATE | LC_CTYPE | LC_MONETARY | LC_NUMERIC | LC_TIME)
#define LC_COLLATE		(1 << 0)
#define LC_CTYPE		(1 << 1)
#define LC_MONETARY		(1 << 2)
#define LC_NUMERIC		(1 << 3)
#define LC_TIME			(1 << 4)

struct lconv {
	char* decimal_point;
	char* thousands_sep;
	char* grouping;
	char* mon_decimal_point;
	char* mon_thousands_sep;
	char* mon_grouping;
	char* positive_sign;
	char* negative_sign;
	char* currency_symbol;
	char* int_curr_symbol;

	char  frac_digits;
	char  p_cs_precedes;
	char  n_cs_precedes;
	char  p_sep_by_space;
	char  n_sep_by_space;
	char  p_sign_posn;
	char  n_sign_posn;
	char  int_frac_digits;
	char  int_p_cs_precedes;
	char  int_n_cs_precedes;
	char  int_p_sep_by_space;
	char  int_n_sep_by_space;
	char  int_p_sign_posn;
	char  int_n_sign_posn;
};

/* ISO C'11 - 7.11.1 Locale control */
/* ISO C'11 - 7.11.1.1 The setlocale function */
_MV_INLINE_DEFN	char*	setlocale(int _MV_UNUSED(__category), const char* __locale)	{ return (char*)((!__locale || (__locale[0] == '\0') || ((__locale[0] == 'C') && (__locale[1] == '\0'))) ? "C" : NULL); }

/* ISO C'11 - 7.11.2 Numeric formatting convention inquiry */
/* ISO C'11 - 7.11.1.1 The localeconv function */
_MV_INLINE_DEFN	struct lconv* localeconv(void) {
	static const struct lconv conv = {
		".", "", "", "", "", "", "", "", "", "",
		CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
		CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX
	};
	return (struct lconv*)&conv;
}


/*
 * The following are all extensions to ISO C that are commonly required.
 * In addition, the following Posix extensions are required to support the LLVM LibC++ library:
 *     http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap07.html#tag_07
 */

/* Posix extensions */
#define	LC_MESSAGES			(1 << 5)

#define	LC_ALL_MASK			(LC_ALL | LC_MESSAGES)
#define	LC_COLLATE_MASK		LC_COLLATE
#define	LC_CTYPE_MASK		LC_CTYPE
#define	LC_MESSAGES_MASK	LC_MESSAGES
#define	LC_MONETARY_MASK	LC_MONETARY
#define	LC_NUMERIC_MASK		LC_NUMERIC
#define	LC_TIME_MASK		LC_TIME

#define	LC_DEFAULT_LOCALE	((struct __locale_t*)0x80000000)
#define	LC_GLOBAL_LOCALE	((struct __locale_t*)0x7FFFFFFF)


#ifndef __STRICT_ANSI__
/* The following are extensions to support common non-Standard uses */
_MV_INLINE_DEFN	struct lconv*	_localeconv_r(void* _MV_UNUSED(__a)) { return localeconv(); }

/* These are required by POSIX, and by implication the LLVM LibC++ library implementation */
_MV_INLINE_DEFN	locale_t		duplocale(locale_t _MV_UNUSED(__locobj)) { return LC_DEFAULT_LOCALE; }
_MV_INLINE_DEFN	void			freelocale(locale_t _MV_UNUSED(__locobj)) {}
_MV_INLINE_DEFN	locale_t		newlocale(int _MV_UNUSED(__category_mask), const char* __locale, locale_t _MV_UNUSED(__base)) { return (setlocale(0, __locale) == NULL) ? ((struct __locale_t*)-1) : LC_DEFAULT_LOCALE; }
_MV_INLINE_DEFN	locale_t		uselocale(locale_t _MV_UNUSED(__locobj)) { return LC_DEFAULT_LOCALE; }
#endif /* __STRICT_ANSI__ */

#endif /* _MV_USE_SYS_LOCALE */

#endif /* _MV_LOCALE_H_ */
