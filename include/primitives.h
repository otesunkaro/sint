#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include <math.h>

#ifndef NAN
/* massive hack fallback */
#define NAN (HUGE_VAL - HUGE_VAL)
#endif

#if __STDC_VERSION__ >= 199901L
#include <stdint.h>
typedef uint_least8_t u8;
typedef uint_least16_t u16;
typedef uint_least32_t u32;
typedef uint_least64_t u64;
typedef uintptr_t uptr;

typedef int_least8_t s8;
typedef int_least16_t s16;
typedef int_least32_t s32;
typedef int_least64_t s64;

typedef float f32;
typedef double f64;
#else
#include <limits.h>

static char MUST_SUPPORT_U64S[sizeof(long int) * CHAR_BIT >= 64 ? 2 : -1];
static char MUST_HAVE_A_UPTR[sizeof(size_t) >= sizeof(void *) ? 2 : -1];

typedef unsigned char u8;
typedef unsigned int u16; /* short is often slower */
typedef unsigned int u32;
typedef unsigned long int u64;
typedef size_t uptr;

typedef signed char s8;
typedef signed int s16; /* ditto above rationale */
typedef signed int s32;
typedef signed long int s64;

typedef float f32;
typedef double f64;
#endif

#endif
