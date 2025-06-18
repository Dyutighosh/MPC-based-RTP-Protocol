#ifndef FLOATINGPOINT_HPP
#define FLOATINGPOINT_HPP

#include <cstdint>
#include <Constants.hpp>

struct doubleNumber
{
    bool sign;
    int_fast16_t exp;
    uint_fast64_t sig;
};

class FloatingPoint
{
public:
    
    struct exp16_sig64 { int_fast16_t exp; uint_fast64_t sig; };
    struct uint128 { uint64_t v0, v64; };

    static struct exp16_sig64 sdouble_normSubnormalF64Sig(uint_fast64_t sig);

    static uint64_t sdouble_shiftRightJam64(uint64_t a, uint_fast32_t dist);

    static doubleNumber getSignExpSig(double in_number);

    static double rePackForTesting(const doubleNumber &in_number);
    static double rePackForTesting(bool sign, int_fast16_t exp, uint_fast64_t sig);

    static double64_t f64_add(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                         bool signB, int_fast16_t expB, uint_fast64_t sigB,
                         bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ, int &packingProcess);

    static double64_t f64_mul(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                         bool signB, int_fast16_t expB, uint_fast64_t sigB,
                         bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ);

    static double64_t f64_div(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                         bool signB, int_fast16_t expB, uint_fast64_t sigB,
                         bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ);

private:
    static uint_fast8_t sdouble_countLeadingZeros64(uint64_t a);
    static double64_t sdouble_roundPackToF64(bool sign, int_fast16_t exp, uint_fast64_t sig);
    static double64_t sdouble_normRoundPackToF64(bool sign, int_fast16_t exp, uint_fast64_t sig);
    

    static double64_t sdouble_subMagsF64(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                                    bool signB, int_fast16_t expB, uint_fast64_t sigB,
                                    bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ,
                                    int &packingProcess);

    static double64_t sdouble_addMagsF64(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                                    bool signB, int_fast16_t expB, uint_fast64_t sigB,
                                    bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ,
                                    int &packingProcess);

    static struct uint128 sdouble_mul64To128(uint64_t a, uint64_t b);
    static uint32_t sdouble_approxRecip32_1(uint32_t a);
};

#endif
