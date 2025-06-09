#pragma once

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>

#ifdef __GNUC_STDC_INLINE__

#define INLINE inline
#else
#define INLINE extern inline
#endif
#define sdouble_BUILTIN_CLZ 1
#define sdouble_INTRINSIC_INT128 1
#define init_detectTininess sdouble_tininess_afterRounding

struct uint128 { uint64_t v0, v64; };
typedef struct { uint64_t v; } double64_t;
template <typename From, typename To> static inline To sdouble_cast(const From &);
#define signF64UI( a ) ((bool) ((uint64_t) (a)>>63))
#define expF64UI( a ) ((int_fast16_t) ((a)>>52) & 0x7FF)
#define fracF64UI( a ) ((a) & UINT64_C( 0x000FFFFFFFFFFFFF ))
#define packToF64UI( sign, exp, sig ) ((uint64_t) (((uint_fast64_t) (sign)<<63) + ((uint_fast64_t) (exp)<<52) + (sig)))
#define isNaNF64UI( a ) (((~(a) & UINT64_C( 0x7FF0000000000000 )) == 0) && ((a) & UINT64_C( 0x000FFFFFFFFFFFFF )))
#define defaultNaNF32UI 0xFFC00000
#define defaultNaNF64UI UINT64_C( 0xFFF8000000000000 )
#define sdouble_ROUND_ODD 1
#define INLINE_LEVEL 5
#define sdouble_FAST_DIV64TO32 1
#define MAX_EXP_BOUND 1037
#define MAX_SIG_BOUND 662247669224603
#define sdouble_FAST_INT64 1
#define sdouble_FAST_INT64 1
#define sdouble_isSigNaNF32UI( uiA ) ((((uiA) & 0x7FC00000) == 0x7F800000) && ((uiA) & 0x003FFFFF))
#define sdouble_isSigNaNF64UI( uiA ) ((((uiA) & UINT64_C( 0x7FF8000000000000 )) == UINT64_C( 0x7FF0000000000000 )) && ((uiA) & UINT64_C( 0x0007FFFFFFFFFFFF )))

union ui64_f64 { uint64_t ui; double64_t f; };

constexpr double ETA = 0.5;
constexpr int numconsumers = NUM_CONSUMERS;
constexpr int numsuppliers = NUM_SUPPLIERS;
constexpr double bk = 0.276;
constexpr double LAMBDA = 21;

enum 
{
    sdouble_round_near_even   = 0,
    sdouble_round_minMag      = 1,
    sdouble_round_min         = 2,
    sdouble_round_max         = 3,
    sdouble_round_near_maxMag = 4,
    sdouble_round_odd         = 6
};

extern uint_fast8_t sdouble_roundingMode;
enum {
    sdouble_tininess_beforeRounding = 0,
    sdouble_tininess_afterRounding  = 1
};
extern uint_fast8_t sdouble_detectTininess;

extern double alpha1;
extern double alpha2;

extern const double d[10];
extern const double eph[10];

extern const uint16_t sdouble_approxRecip_1k0s[16];
extern const uint16_t sdouble_approxRecip_1k1s[16];

#endif
