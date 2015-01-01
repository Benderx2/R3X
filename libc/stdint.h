#ifndef REX_LIBC_STDINT_H
#define REX_LIBC_STDINT_H
// Only for the REX Operating System.
#include "rexos.h"
#ifndef __REX__
#error "Compiling for a non-REX platform"
#endif
typedef intmax_t __REX_INTMAX;
typedef uintmax_t __REX_UINTMAX;
typedef int8_t __REX_8;
typedef uint8_t __REX_U8;
typedef int16_t __REX_16;
typedef uint16_t __REX_U16;
typedef int32_t __REX_32;
typedef uint32_t __REX_U32;
#ifdef __REX64__
typedef int64_t __REX_64;
typedef uint64_t __REX_U64;
#endif
#define INT8_MAX __REX_LIMIT8;
#define INT16_MAX __REX_LIMIT16;
#define INT32_MAX __REX_LIMIT32;
#define UINT8_MAX __REX_LIMITU8;
#define UINT16_MAX __REX_LIMITU16;
#define UINT32_MAX __REX_LIMITU32;
#endif
