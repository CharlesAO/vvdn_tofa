/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   fenv.h
 *  Description:   Lightweight implementation of the 'fenv' header for the
 *                 SHAVE library 'mlibm.a'
 *  --------------------------------------------------------------------------- */

#ifndef _MCC_FENV_H_
#define _MCC_FENV_H_ (1)


#ifndef __shave__
# error "fenv.h: this header is for the SHAVE processor!"
#endif

#pragma GCC system_header

#include <sys/__moviconfig.h>


_MV_EXTERNC_BEGIN


/* Floating-point exception types */
#define FE_DIVBYZERO   (1u << 0)
#define FE_INEXACT     (1u << 1)
#define FE_INVALID     (1u << 2)
#define FE_OVERFLOW    (1u << 3)
#define FE_UNDERFLOW   (1u << 4)
#define FE_ALL_EXCEPT  (FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)

/* Floating-point rounding modes: */
# define FE_TONEAREST  (0u)
# define FE_TOWARDZERO (1u)
# define FE_UPWARD     (2u)
# define FE_DOWNWARD   (3u)

typedef unsigned fexcept_t;
typedef unsigned fenv_t;

extern const fenv_t __fe_dfl_env;
#define FE_DFL_ENV     (&__fe_dfl_env)

/* 7.6.2 - Floating-point exceptions: */
extern int feclearexcept(int excepts);
extern int feraiseexcept(int excepts);
extern int fegetexceptflag(fexcept_t *flagp, int excepts);
extern int fesetexceptflag(const fexcept_t *flagp, int excepts);
extern int fetestexcept(int excepts);

/* 7.6.3 - Rounding: */
extern int fegetround(void);
extern int fesetround(int round);

/* 7.6.4 - Environment */
extern int fegetenv(fenv_t *envp);
extern int fesetenv(const fenv_t *envp);
extern int feholdexcept(fenv_t *envp);
extern int feupdateenv(const fenv_t *envp);

/* SHAVE Extensions - Floating-point to integer rounding modes: */
extern int __getfptointround(void);
extern int __setfptointround(int round);

#ifndef __STRICT_ANSI__
#define getfptointround()   __getfptointround()
#define setfptointround(x)  __setfptointround(x)
#endif /* __STRICT_ANSI__ */


_MV_EXTERNC_END


#endif /* _MCC_FENV_H_ */
