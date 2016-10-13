// Data types for convenient work

#ifndef TYPEDEF_H
#define TYPEDEF_H

#pragma once

#include <sys/types.h>
#include "config.h"

    // Definitions of standard types of known size
typedef signed char      i8;
typedef signed short     i16;
typedef signed long      i32;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned long      u32;

#ifdef USE_X64
typedef signed long long i64;
typedef unsigned long long u64;
#endif



		// Definitions of data types for long numbers
typedef u32 limb_t;   // One limb - 32 bits (unsigned)
typedef i32 slimb_t;  // Signed 'limb'
typedef u16 hlimb_t;  // Half of a limb - 16 bits (unsigned)
typedef i8  sign_t;   // Sign (-1, 0, 1)
typedef i32 offset_t; // Signed size of a long number
typedef u32 bitcnt_t; // Type for bit count

#ifdef USE_X64
typedef u64 dlimb_t;  // Double limb - 64 bits (unsigned)
#endif

#endif
