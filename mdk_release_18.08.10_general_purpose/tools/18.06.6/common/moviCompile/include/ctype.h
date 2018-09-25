/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2017-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   ctype.h
 *  Description:   Lightweight "Locale Free" implementation of the ISO C '<ctype.h>'
 *                 library interface.
 *  --------------------------------------------------------------------------- */

#ifndef _MV_CTYPE_H_
#define _MV_CTYPE_H_ (1)


#include <sys/__moviconfig.h>


#ifdef _MV_USE_SYS_CTYPE
#include_next <ctype.h>
#else /* _MV_USE_SYS_CTYPE */
#define __ALNUM  (__ALPHA | __DIGIT)
#define __ALPHA  (__LOWER | __UPPER)
#define __BLANK  (1 << 0)       /* SPACE and TAB */
#define __CNTRL  (1 << 1)       /* Control-characters */
#define __DIGIT  (1 << 2)       /* '0' .. '9' */
#define __LOWER  (1 << 3)       /* 'a' .. 'z' */
#define __PRINT  (1 << 4)       /* Other printable characters */
#define __SPACE  (1 << 5)       /* Whitespace-character: SPACE, FF, LF, CR, TAB and VT */
#define __UPPER  (1 << 6)       /* 'A' .. 'Z' */
#define __XDIGT  (1 << 7)       /* '0' .. '9', 'a' .. 'f', 'A' .. 'F' */

_MV_PURE_INLINE_DEFN const char* __get_ctype_map() {
    static const char ctype_map[128] = {
        /* NUL, SOH, STX, ETX, EOT, ENQ */
        __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL,
        /* ACK, BEL, BS, TAB, LF, VT */
        __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL | __BLANK,  __CNTRL | __SPACE,  __CNTRL | __SPACE,
        /* FF, CR, SO, SI, DLE, DC1 */
        __CNTRL | __SPACE,  __CNTRL | __SPACE,  __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL,
        /* DC2, DC3, DC4, NAK, SYN, ETB */
        __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL,
        /* CAN, EM, SUB, ESC, FS, GS */
        __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL,            __CNTRL,
        /* RS, US */
        __CNTRL,            __CNTRL,

        /* SPACE */
        __BLANK,

        /* '!', '"', '#', '$', '%', '&' */
        __PRINT,            __PRINT,            __PRINT,            __PRINT,            __PRINT,            __PRINT,
        /* ''', '(', ')', '*', '+', ',' */
        __PRINT,            __PRINT,            __PRINT,            __PRINT,            __PRINT,            __PRINT,
        /* '-', '.', '/' */
        __PRINT,            __PRINT,            __PRINT,

        /* '0..9' */
        __DIGIT,            __DIGIT,            __DIGIT,            __DIGIT,            __DIGIT,            __DIGIT,
        __DIGIT,            __DIGIT,            __DIGIT,            __DIGIT,

        /* ':', ';', '<', '=', '>', '?', '@' */
        __PRINT,            __PRINT,            __PRINT,            __PRINT,            __PRINT,            __PRINT,
        __PRINT,

        /* 'A..F' are uppercase letters and hex digits */
        __UPPER | __XDIGT,  __UPPER | __XDIGT,  __UPPER | __XDIGT,  __UPPER | __XDIGT,  __UPPER | __XDIGT,  __UPPER | __XDIGT,
        /* 'G..Z' */
        __UPPER,            __UPPER,            __UPPER,            __UPPER,            __UPPER,            __UPPER,
        __UPPER,            __UPPER,            __UPPER,            __UPPER,            __UPPER,            __UPPER,
        __UPPER,            __UPPER,            __UPPER,            __UPPER,            __UPPER,            __UPPER,
        __UPPER,            __UPPER,

        /* '[', '\', ']', '^', '_', '`' */
        __PRINT,            __PRINT,            __PRINT,            __PRINT,            __PRINT,            __PRINT,

        /* 'a..f' are lowercase letters and hex digits */
        __LOWER | __XDIGT,  __LOWER | __XDIGT,  __LOWER | __XDIGT,  __LOWER | __XDIGT,  __LOWER | __XDIGT,  __LOWER | __XDIGT,
        /* 'g..z' */
        __LOWER,            __LOWER,            __LOWER,            __LOWER,            __LOWER,            __LOWER,
        __LOWER,            __LOWER,            __LOWER,            __LOWER,            __LOWER,            __LOWER,
        __LOWER,            __LOWER,            __LOWER,            __LOWER,            __LOWER,            __LOWER,
        __LOWER,            __LOWER,

        /* '{', '|', '}', '~'           */
        __PRINT,            __PRINT,            __PRINT,            __PRINT,

        /* DEL */
        __CNTRL
    };
    return ctype_map;
}

_MV_PURE_INLINE_DEFN int __ascii(int __c)  { return (int)((__c & ~0x7F) == 0); }

/* Character classification functions */
_MV_PURE_INLINE_DEFN int isalnum(int __c)  { return __ascii(__c) ? (int)(__get_ctype_map()[__c] & __ALNUM) : 0; }
_MV_PURE_INLINE_DEFN int isalpha(int __c)  { return __ascii(__c) ? (int)(__get_ctype_map()[__c] & __ALPHA) : 0; }
_MV_PURE_INLINE_DEFN int isblank(int __c)  { return __ascii(__c) ? (int)(__get_ctype_map()[__c] & __BLANK) : 0; }
_MV_PURE_INLINE_DEFN int iscntrl(int __c)  { return __ascii(__c) ? (int)(__get_ctype_map()[__c] & __CNTRL) : 0; }
_MV_PURE_INLINE_DEFN int isdigit(int __c)  { return __ascii(__c) ? (int)(__get_ctype_map()[__c] & __DIGIT) : 0; }
_MV_PURE_INLINE_DEFN int islower(int __c)  { return __ascii(__c) ? (int)(__get_ctype_map()[__c] & __LOWER) : 0; }
_MV_PURE_INLINE_DEFN int isprint(int __c)  { return __ascii(__c) ? (int)(__get_ctype_map()[__c] & (__SPACE | __BLANK | __PRINT | __ALNUM)) : 0; }
_MV_PURE_INLINE_DEFN int isspace(int __c)  { return __ascii(__c) ? (int)(__get_ctype_map()[__c] & (__SPACE | __BLANK)) : 0; }
_MV_PURE_INLINE_DEFN int isupper(int __c)  { return __ascii(__c) ? (int)(__get_ctype_map()[__c] & __UPPER) : 0; }
_MV_PURE_INLINE_DEFN int isxdigit(int __c) { return __ascii(__c) ? (int)(__get_ctype_map()[__c] & (__XDIGT | __DIGIT)) : 0; }

_MV_PURE_INLINE_DEFN int isgraph(int __c)  { return (isprint(__c) && (__c != ' ')); }
_MV_PURE_INLINE_DEFN int ispunct(int __c)  { return (isprint(__c) && !isalnum(__c) && !isspace(__c)); }

/* Character case mapping functions */
_MV_PURE_INLINE_DEFN int tolower(int __c)  { return isupper(__c) ? (int)((__c - 'A') + 'a') : __c; }
_MV_PURE_INLINE_DEFN int toupper(int __c)  { return islower(__c) ? (int)((__c - 'a') + 'A') : __c; }

#ifndef __STRICT_ANSI__
/* Common extensions to ISO C */
_MV_PURE_INLINE_DEFN int isascii(int __c)  { return __ascii(__c); }
_MV_PURE_INLINE_DEFN int toascii(int __c)  { return (int)(__c & 0x7F); }
#endif /* __STRICT_ANSI__ */

#endif /* _MV_USE_SYS_CTYPE */

#endif /* _MV_CTYPE_H_ */
