// Data types for convenient work
// We don't recommend touch anything, because changes of limb size in "config.h" automatically changes types in this file.

#ifndef TYPEDEF_H
#define TYPEDEF_H
#pragma once

#include <assert.h>
#include "config.h"
#include <cstdint>	// std::intX_t - 'guaranteed' (*heh*) size integers
#include <cstddef>	// size_t, intptr_t



    // Definitions of standard types of known size
typedef std::int8_t   i8;
typedef std::int16_t  i16;
typedef std::int32_t  i32;
typedef std::int64_t  i64;

typedef std::uint8_t  u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;



    // Definitions of data types for long numbers
typedef std::uintptr_t size_t;	// Unsigned size of a long number
typedef std::intptr_t offset_t;	// Signed size of a long number
typedef i8 sign_t;	// A sign (-1, 0, 1)
typedef size_t bitcnt_t;	// A type for bit count

#ifdef LIMB_BITS

#if LIMB_BITS == 8
  typedef u8 limb_t;	// One limb
  typedef i8 slimb_t;	// Signed 'limb'

  const limb_t LIMB_MAX = UINT8_MAX;
  const limb_t SLIMB_MIN = INT8_MIN;
  const limb_t SLIMB_MAX = INT8_MAX;

      // You can use either double-limb, or half-limb
  #ifdef USE_DLIMB
    typedef u16 dlimb_t;  // Double-limb
  #else
    #error Half-limb is not supported in case of limb_t = 8 bits. Please, undefine USE_DLIMB in "config.h"  // Half-limb
  #endif

#elif LIMB_BITS == 16
  typedef u16 limb_t;
  typedef i16 slimb_t;

  const limb_t LIMB_MAX = UINT16_MAX;
  const limb_t SLIMB_MIN = INT16_MIN;
  const limb_t SLIMB_MAX = INT16_MAX;

  #ifdef USE_DLIMB
    typedef u32 dlimb_t;
  #else
    typedef u8 hlimb_t;
  #endif

#elif LIMB_BITS == 32
  typedef u32 limb_t;
  typedef i32 slimb_t;

  const limb_t LIMB_MAX = UINT32_MAX;
  const limb_t SLIMB_MIN = INT32_MIN;
  const limb_t SLIMB_MAX = INT32_MAX;

  #ifdef USE_DLIMB
    typedef u64 dlimb_t;
  #else
    typedef u16 hlimb_t;
  #endif

#elif LIMB_BITS == 64
  typedef u64 limb_t;
  typedef i64 slimb_t;

  const limb_t LIMB_MAX = UINT64_MAX;
  const limb_t SLIMB_MIN = INT64_MIN;
  const limb_t SLIMB_MAX = INT64_MAX;

  #ifdef USE_DLIMB
    #error Double-limb is not supported in case of limb_t = 64 bits. Please, define USE_DLIMB in "config.h"
  #else
    typedef u32 hlimb_t;
  #endif

#else
  #error LIMB_BITS is not supported. See "config.h".

#endif

#else
  #error You forgot to define size of limb. See "config.h".

#endif

#ifdef USE_DLIMB
	const bitcnt_t DLIMB_BITS = LIMB_BITS * 2;
#else
	const bitcnt_t HLIMB_BITS = LIMB_BITS / 2;
#endif



#endif
