#include "parse.h"
#include "primitives.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

FILE *wasm;

u8 parse_byte(void) {
  int c = fgetc(wasm);

  if (c == EOF) {
    fprintf(stderr, "0x%lx: expected byte, found EOF\n",
            (long unsigned)ftell(wasm));
    exit(EXIT_FAILURE);
  }

  return (u8)c;
}

static u64 parse_u(unsigned max_bits) {
  u64 result = 0;

  for (unsigned shift = 0; shift < max_bits; shift += 7) {
    u8 byte = parse_byte();

    result |= (byte & 0x7F) << shift;

    if ((byte & 0x80) == 0) /* last byte? */
      return result;
  }

  fprintf(stderr,
          "0x%lx: expected unsigned integer with %u bits at maximum, found "
          "additional data\n",
          (long unsigned)ftell(wasm), max_bits);
  exit(EXIT_FAILURE);
}

static s64 parse_s(unsigned max_bits) {
  u64 result = 0;

  for (unsigned shift = 0; shift < max_bits; shift += 7) {
    u8 byte = parse_byte();

    result |= (byte & 0x7F) << shift;

    if ((byte & 0x80) == 0) {                /* last byte? */
      if ((byte & 0x40) != 0) {              /* negative? */
        result = ((u64)1 << shift) - result; /* sign extended negate */
        return -((s64)result);
      } else {
        return (s64)result;
      }
    }
  }

  fprintf(stderr,
          "0x%lx: expected signed integer with %u bits at maximum, found "
          "additional data\n",
          (long unsigned)ftell(wasm), max_bits);
  exit(EXIT_FAILURE);
}

/* each of these bit counts is rounded up to the nearest multiple of 7 */
u8 parse_u8(void) { return (u8)parse_u(14); }
u16 parse_u16(void) { return (u16)parse_u(21); }
u32 parse_u32(void) { return (u32)parse_u(35); }
u64 parse_u64(void) { return (u64)parse_u(70); }
s8 parse_s8(void) { return (u8)parse_s(14); }
s16 parse_s16(void) { return (u16)parse_s(21); }
s32 parse_s32(void) { return (u32)parse_s(35); }
s64 parse_s64(void) { return (u64)parse_s(70); }

f32 parse_f32(void) {
  u32 bits = 0;
  u32 mantissa;
  s32 exponent;
  f32 result;

  for (unsigned shift = 0; shift < 32; shift += 8)
    bits |= (u32)parse_byte() << shift;

  mantissa = bits & (((u32)1 << 23) - 1);
  exponent = (bits >> 23) & (((u32)1 << 8) - 1);

  if (exponent == ((u32)1 << 8) - 1) {
    result = mantissa ? NAN : -HUGE_VAL;
  } else if (exponent == 0) {
    /* subnormal */
    result = (f32)ldexp((double)mantissa, -149);
  } else {
    result = (f32)ldexp((double)(mantissa | ((u32)1 << 23)), exponent - 150);
  }

  if ((bits >> 31) != 0) {
    return -result;
  } else {
    return result;
  }
}

f64 parse_f64(void) {
  u64 bits = 0;
  u64 mantissa;
  s64 exponent;
  f64 result;

  for (unsigned shift = 0; shift < 64; shift += 8)
    bits |= ((u64)parse_byte()) << shift;

  mantissa = bits & (((u64)1 << 52) - 1);
  exponent = (bits >> 52) & (((u64)1 << 11) - 1);

  if (exponent == ((u64)1 << 11) - 1) {
    result = mantissa ? NAN : HUGE_VAL;
  } else if (exponent == 0) {
    /* subnormal */
    result = (f64)ldexp((double)mantissa, -1074);
  } else {
    result = (f64)ldexp((double)(mantissa | ((u64)1 << 52)), exponent - 1075);
  }

  if ((bits >> 63) != 0) {
    return -result;
  } else {
    return result;
  }
}
