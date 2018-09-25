#ifdef __PC__
#include <half.h>
#include <VectorTypes.h>
#include <builtinFunctions.h>
#else
#include <moviVectorTypes.h>
#include <moviVectorConvert.h>
#endif

#define __Deprecated__(message) __attribute__((deprecated(message)))

#pragma GCC diagnostic ignored "-Wattributes"

#if defined(__cplusplus) && defined(__MOVICOMPILE__)
#define MVCV_EXPORT extern "C"
#else
#define MVCV_EXPORT
#endif

#ifdef __MOVICOMPILE__
	#define MVCV_FUNC(type, name, ...) MVCV_EXPORT type name##_asm(__VA_ARGS__);\
	MVCV_EXPORT type name(__VA_ARGS__);
	#define MVCV_FUNC_OVERLOADED(type, name, ...) MVCV_EXPORT __attribute__((overloadable)) type name##_asm(__VA_ARGS__);\
	MVCV_EXPORT __attribute__((overloadable)) type name(__VA_ARGS__);
#else
	#define MVCV_FUNC(type, name, ...) MVCV_EXPORT type name(__VA_ARGS__);
	#define MVCV_FUNC_OVERLOADED(type, name, ...) MVCV_EXPORT __attribute__((overloadable)) type name(__VA_ARGS__);
#endif
