#ifndef PARSE_H
#define PARSE_H

#include "primitives.h"
#include <stdio.h>

extern FILE *wasm;
extern u8 parse_u8(void);
extern u16 parse_u16(void);
extern u32 parse_u32(void);
extern u64 parse_u64(void);
extern s8 parse_s8(void);
extern s16 parse_s16(void);
extern s32 parse_s32(void);
extern s64 parse_s64(void);
extern f32 parse_f32(void);
extern f64 parse_f64(void);

#endif
