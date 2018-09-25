/* def/typemacs.h	Tue Jun 14 2016 09:55:21 chwon */

/*

Module:  def/typemacs.h

Function:
	Numerous macros for defining and/or dealing with types of things.

Version:
	V3.17a	Tue Jun 14 2016 09:55:21 chwon	Edit level 11

Copyright notice:
	This file copyright (C) 1998-2008, 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation
 
Author:
	Terry Moore, MCCI Corporation	June 1998

Revision history:
   2.10a  Thu Jun 11 1998 23:44:24  tmm
	Module created.

   2.10v  3/17/1999  tmm
	Get rid of warnings for arm sdk.

   3.01a  5/22/2000  tmm
	We need to have union typedefs, too.

   3.02a  11/26/2000  tmm
	We need enum typedefs.

   3.04a  11/23/2001  tmm
	bug 724:  expand __TMS_FNTYPE_DEF correctly when cloaked.

   3.05f  Tue Oct 26 2004 08:52:11  maw
   	Comment added about cloaking

   3.05g  Wed Mar 30 2005 15:04:04  maw
   	Added TYPE_DEF_HANDLE

   3.05g  Sat Dec  3 2005 15:46:29  tmm
	I finally could not handle it any more.  Add __TMS_TYPE_DEF_FUNCTION,
	and deprecate (but don't remove) __TMS_FNTYPE_DEF().

   3.09a  Fri Jul 18 2008 08:59:30  chwon
	4311: add HANDLE pointer type definition

   3.17a  Tue Jun 14 2016 09:55:21  chwon
	20168: Add _e postfix for enum name definition.

*/

#ifndef _DEF_TYPEMACS_H_		/* prevent multiple includes */
#define _DEF_TYPEMACS_H_

#ifndef __TMS_CLOAKED_NAMES_ONLY
# define	__TMS_CLOAKED_NAMES_ONLY	0	/* backwards compat */
#endif /* ndef __TMS_CLOAKED_NAMES_ONLY */

#ifndef __TMS_COMPAT_FNTYPE_DEF
# define	__TMS_COMPAT_FNTYPE_DEF		1	/* backwards compat */
#endif /* ndef __TMS_COMPAT_FNTYPE_DEF */

/*
|| Macros for defining types
||
|| NOTE : uncloaked names for these are intentionally not provided, as they 
||        are only intended to be used in header files
*/
#define	__TMS_STRUCTNAME(StructName)	__TMS_CONCAT(__TMS_,StructName)
#define	__TMS_UNIONNAME(UnionName)	__TMS_CONCAT(__TMS_,UnionName)
#define	__TMS_ENUMNAME(EnumName)	__TMS_CONCAT3(__TMS_,EnumName,_e)

#define	__TMS_TYPE_DEF_RAW(TypeName, TypeExpr) \
	typedef	TypeExpr TypeName, *__TMS_CONCAT(P,TypeName)

#define	__TMS_TYPE_DEF_PFX(Pfx, TypeName, TypeExpr) \
	typedef	TypeExpr __TMS_CONCAT(Pfx,TypeName), \
		*__TMS_CONCAT3(Pfx,P,TypeName)

#define	__TMS_TYPE_DEF_CLOAK(TypeName, TypeExpr) \
	__TMS_TYPE_DEF_PFX(__TMS_, TypeName, TypeExpr)

#define	__TMS_TYPE_DEF_BOTH(TypeName, TypeExpr) \
	__TMS_TYPE_DEF_CLOAK(TypeName, TypeExpr); \
	__TMS_TYPE_DEF_RAW(TypeName, TypeExpr)

#define	__TMS_TYPE_DEF_STRUCT_BOTH(StructName) \
	__TMS_TYPE_DEF_BOTH(StructName, struct __TMS_STRUCTNAME(StructName))

#define	__TMS_TYPE_DEF_STRUCT_CLOAK(StructName) \
	__TMS_TYPE_DEF_CLOAK(StructName, struct __TMS_STRUCTNAME(StructName))

#define	__TMS_TYPE_DEF_UNION_BOTH(UnionName) \
	__TMS_TYPE_DEF_BOTH(UnionName, union __TMS_UNIONNAME(UnionName))

#define	__TMS_TYPE_DEF_UNION_CLOAK(UnionName) \
	__TMS_TYPE_DEF_CLOAK(UnionName, union __TMS_UNIONNAME(UnionName))

#define	__TMS_TYPE_DEF_ENUM_BOTH(EnumName) \
	__TMS_TYPE_DEF_BOTH(EnumName, enum __TMS_ENUMNAME(EnumName))

#define	__TMS_TYPE_DEF_ENUM_CLOAK(EnumName) \
	__TMS_TYPE_DEF_CLOAK(EnumName, enum __TMS_ENUMNAME(EnumName))

/*
|| Define a handle type.  Three flavors are
|| possible....  with type prefix, without type prefix, or both.
*/
#define	__TMS_TYPE_DEF_HANDLE_RAW(HandleName) 		\
	typedef	struct __TMS_CONCAT(__TMX_,HandleName)  \
		*HandleName,				\
		**__TMS_CONCAT(P,HandleName)		/* 4311 */

#define	__TMS_TYPE_DEF_HANDLE_PFX(Pfx, HandleName)	\
	typedef	struct __TMS_CONCAT(__TMX_,HandleName)	\
		*__TMS_STRUCTNAME(HandleName),		\
		**__TMS_CONCAT3(Pfx,P,HandleName)	/* 4311 */

#define	__TMS_TYPE_DEF_HANDLE_CLOAK(HandleName) \
	__TMS_TYPE_DEF_HANDLE_PFX(__TMS_, HandleName)

#define	__TMS_TYPE_DEF_HANDLE_BOTH(HandleName) \
	__TMS_TYPE_DEF_HANDLE_CLOAK(HandleName); \
	__TMS_TYPE_DEF_HANDLE_RAW(HandleName)

/*
|| Define a function type and its pointer type.  Three flavors are
|| possible....  with type prefix, without type prefix, or both.
*/
#define	__TMS_TYPE_DEF_FUNCTION_PFX(Pfx, FunctionType, ResultType, ArgList) \
	typedef	ResultType (__TMS_CONCAT(Pfx,FunctionType)) ArgList; \
	typedef __TMS_CONCAT(Pfx,FunctionType) *__TMS_CONCAT3(Pfx,P,FunctionType)
#define	__TMS_TYPE_DEF_FUNCTION_RAW(FunctionType, ResultType, ArgList) \
	typedef	ResultType (FunctionType) ArgList; \
	typedef FunctionType *__TMS_CONCAT(P,FunctionType)

#define	__TMS_TYPE_DEF_FUNCTION_CLOAK(FunctionType, ResultType, ArgList) \
	__TMS_TYPE_DEF_FUNCTION_PFX(__TMS_, FunctionType, ResultType, ArgList)

#define	__TMS_TYPE_DEF_FUNCTION_BOTH(FunctionType, ResultType, ArgList) \
	__TMS_TYPE_DEF_FUNCTION_CLOAK(FunctionType, ResultType, ArgList); \
	__TMS_TYPE_DEF_FUNCTION_RAW(FunctionType, ResultType, ArgList)

/* 
|| arm sdk wants __TMS_CLOAKED_NAMES_ONLY to have a value -- with UNIX and
|| other C compilers, this works without warnings, but arm cc is picky.
*/
#ifndef __TMS_CLOAKED_NAMES_ONLY
# define __TMS_CLOAKED_NAMES_ONLY 0
#endif

#if __TMS_CLOAKED_NAMES_ONLY
# define __TMS_TYPE_DEF(TypeName, TypeExpr)	\
	__TMS_TYPE_DEF_CLOAK(TypeName, TypeExpr)
# define __TMS_TYPE_DEF_STRUCT(StructName)	\
	__TMS_TYPE_DEF_STRUCT_CLOAK(StructName)
# define __TMS_TYPE_DEF_UNION(UnionName)	\
	__TMS_TYPE_DEF_UNION_CLOAK(UnionName)
# define __TMS_TYPE_DEF_ENUM(EnumName)	\
	__TMS_TYPE_DEF_ENUM_CLOAK(EnumName)
# define __TMS_TYPE_DEF_HANDLE(HandleName)	\
	__TMS_TYPE_DEF_HANDLE_CLOAK(HandleName)
# define __TMS_TYPE_DEF_FUNCTION(FunctionType, ResultType, ArgList) \
	 __TMS_TYPE_DEF_FUNCTION_CLOAK(FunctionType, ResultType, ArgList)
#else
# define __TMS_TYPE_DEF(TypeName, TypeExpr)	\
	__TMS_TYPE_DEF_BOTH(TypeName, TypeExpr)
# define __TMS_TYPE_DEF_STRUCT(StructName)	\
	__TMS_TYPE_DEF_STRUCT_BOTH(StructName)
# define __TMS_TYPE_DEF_UNION(UnionName)	\
	__TMS_TYPE_DEF_UNION_BOTH(UnionName)
# define __TMS_TYPE_DEF_ENUM(EnumName)	\
	__TMS_TYPE_DEF_ENUM_BOTH(EnumName)
# define __TMS_TYPE_DEF_HANDLE(HandleName)	\
	__TMS_TYPE_DEF_HANDLE_BOTH(HandleName)
# define __TMS_TYPE_DEF_FUNCTION(FunctionType, ResultType, ArgList) \
	 __TMS_TYPE_DEF_FUNCTION_BOTH(FunctionType, ResultType, ArgList)
#endif

/*
|| Use the following when you only want cloaked arg types
*/
/* this one lets you specify the two base types */
#define	__TMS_TYPE_DEF_ARG_CLOAK2(TypeNameNoPfx, BaseType, ArgType) \
	__TMS_TYPE_DEF_CLOAK(TypeNameNoPfx, BaseType); \
	__TMS_TYPE_DEF_CLOAK(__TMS_CONCAT(ARG_,TypeNameNoPfx), ArgType)

/* this one requires a BaseTypeName (uncloaked) */
#define	__TMS_TYPE_DEF_ARG_CLOAK(TypeName, BaseTypeName) \
	__TMS_TYPE_DEF_ARG_CLOAK2(TypeName, \
			__TMS_CONCAT(__TMS_,BaseTypeName), \
			__TMS_CONCAT(__TMS_ARG_,BaseTypeName))

/*
|| Use the following when you want cloaked and uncloaked types,
|| and when you have a base type name that already exists cloaked
|| in the base/arg forms.
*/
#define	__TMS_TYPE_DEF_ARG(TypeName, BaseTypeName)	\
	__TMS_TYPE_DEF(TypeName, __TMS_CONCAT(__TMS_,BaseTypeName));	\
	__TMS_TYPE_DEF(__TMS_CONCAT(ARG_,TypeName), __TMS_CONCAT(__TMS_ARG_,BaseTypeName))

/*
|| Use the following to disclose a cloaked type.
*/
/* create TypeName from __TMS_TypeName */
#define	__TMS_TYPE_DEF_DISCLOSE_BASIC(TypeName) \
	typedef __TMS_CONCAT(__TMS_,TypeName) TypeName

/* create TypeName, PTypeName from __TMS_TypeName */
#define __TMS_TYPE_DEF_DISCLOSE(TypeName) \
	__TMS_TYPE_DEF_DISCLOSE_BASIC(TypeName), *__TMS_CONCAT(P,TypeName)

/* create TypeName, PTypeName, ARG_TypeName, PARG_TypeName from __TMS_* */
#define	__TMS_TYPE_DEF_DISCLOSE_ARG(TypeName) \
	__TMS_TYPE_DEF_DISCLOSE(TypeName); \
	__TMS_TYPE_DEF_DISCLOSE(__TMS_CONCAT(ARG_,TypeName))

/*
|| Compatible names for __TMS_FNTYPE_DEF:
*/
#if __TMS_COMPAT_FNTYPE_DEF
# define __TMS_FNTYPE_DEF_PFX(Pfx, FunctionType, ResultType, ArgList) \
	 __TMS_TYPE_DEF_FUNCTION_PFX(Pfx, FunctionType, ResultType, ArgList)

# define __TMS_FNTYPE_DEF_RAW(FunctionType, ResultType, ArgList) \
	 __TMS_TYPE_DEF_FUNCTION_RAW(FunctionType, ResultType, ArgList)

# define __TMS_FNTYPE_DEF_CLOAK(FunctionType, ResultType, ArgList) \
	 __TMS_TYPE_DEF_FUNCTION_CLOAK(FunctionType, ResultType, ArgList)

# define __TMS_FNTYPE_DEF_BOTH(FunctionType, ResultType, ArgList) \
	 __TMS_TYPE_DEF_FUNCTION_BOTH(FunctionType, ResultType, ArgList)

# define __TMS_FNTYPE_DEF(FunctionType, ResultType, ArgList) \
	 __TMS_TYPE_DEF_FUNCTION(FunctionType, ResultType, ArgList)
#endif /* __TMS_WANT_FNTYPE_DEF */

/**** end of typemacs.h ****/
#endif /* _DEF_TYPEMACS_H_ */
