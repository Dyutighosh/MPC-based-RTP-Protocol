#include "FloatingPoint.hpp"
#include "Utils.hpp"
#include <limits>
#include <cmath>
#include <iostream>

using namespace Utils;

uint_fast8_t FloatingPoint::sdouble_countLeadingZeros64(uint64_t a)
{
    return Utils::countLeadingZeros64(a);
}

doubleNumber FloatingPoint::getSignExpSig(double in_number)
{
    doubleNumber finalComponents;
    union ui64_f64 uA;
    uint_fast64_t uiA;
    bool signA;
    int_fast16_t expA;
    uint_fast64_t sigA;
    uA.f = *(double64_t*) &in_number;
    uiA = uA.ui;
    signA = signF64UI(uiA);
    expA  = expF64UI(uiA);
    sigA  = fracF64UI(uiA);

    finalComponents.sign = signA;
    finalComponents.exp = expA;
    finalComponents.sig = sigA;
    return finalComponents;
}

double FloatingPoint::rePackForTesting(const doubleNumber &in_number)
{
    uint_fast64_t uiZ;
    union ui64_f64 uZ;
    uiZ = packToF64UI(in_number.sign, in_number.exp, in_number.sig);
    uZ.ui  = uiZ;
    double64_t in_number_reconstruct =uZ.f;
    double t_x1 = *(double*) &in_number_reconstruct; 

    return t_x1;
}

double FloatingPoint::rePackForTesting(bool sign, int_fast16_t exp, uint_fast64_t sig)
{
    doubleNumber in_number;
    in_number.sign = sign;
    in_number.exp = exp;
    in_number.sig = sig;
    return rePackForTesting(in_number);
}

FloatingPoint::exp16_sig64 FloatingPoint::sdouble_normSubnormalF64Sig(uint_fast64_t sig)
{
    struct exp16_sig64 z;
    int_fast8_t shiftDist = sdouble_countLeadingZeros64(sig) - 11;
    z.exp = 1 - shiftDist;
    z.sig = sig << shiftDist;
    return z;
}

uint64_t FloatingPoint::sdouble_shiftRightJam64(uint64_t a, uint_fast32_t dist)
{
    return (dist < 63) ? (a >> dist) | ((uint64_t)(a << (-dist & 63)) != 0) : (a != 0);
}

double64_t FloatingPoint::sdouble_roundPackToF64(bool sign, int_fast16_t exp, uint_fast64_t sig)
{
    uint_fast8_t roundingMode;
    bool roundNearEven;
    uint_fast16_t roundIncrement, roundBits;
    bool isTiny;
    uint_fast64_t uiZ;
    union ui64_f64 uZ;

    roundingMode = sdouble_roundingMode;
    roundNearEven = (roundingMode == sdouble_round_near_even);
    roundIncrement = 0x200;
    
    if ( ! roundNearEven && (roundingMode != sdouble_round_near_maxMag) ) 
    {
        roundIncrement = (roundingMode == (sign ? sdouble_round_min : sdouble_round_max)) ? 0x3FF : 0;
    }

    roundBits = sig & 0x3FF;
    if ( 0x7FD <= (uint16_t) exp ) 
    {
        if ( exp < 0 ) 
        {
            isTiny =
                (sdouble_detectTininess == sdouble_tininess_beforeRounding)
                    || (exp < -1)
                    || (sig + roundIncrement < UINT64_C( 0x8000000000000000 ));
            sig = sdouble_shiftRightJam64( sig, -exp );
            exp = 0;
            roundBits = sig & 0x3FF;

        } 
        else if ((0x7FD < exp) || (UINT64_C( 0x8000000000000000 ) <= sig + roundIncrement)) 
        {
            uiZ = packToF64UI( sign, 0x7FF, 0 ) - ! roundIncrement;
            goto uiZ;
        }
    }
    sig = (sig + roundIncrement)>>10;
    if ( roundBits ) 
    {
#ifdef sdouble_ROUND_ODD
        if ( roundingMode == sdouble_round_odd ) 
        {
            sig |= 1;
            goto packReturn;
        }
#endif
    }

    sig &= ~(uint_fast64_t) (! (roundBits ^ 0x200) & roundNearEven);

    if ( ! sig ) exp = 0;

 packReturn:
    uiZ = packToF64UI( sign, exp, sig );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;
}

double64_t FloatingPoint::sdouble_normRoundPackToF64(bool sign, int_fast16_t exp, uint_fast64_t sig)
{
    int_fast8_t shiftDist;
    union ui64_f64 uZ;

    shiftDist = sdouble_countLeadingZeros64( sig ) - 1;
    exp -= shiftDist;
    if ( (10 <= shiftDist) && ((unsigned int) exp < 0x7FD) ) {
        uZ.ui = packToF64UI( sign, sig ? exp : 0, sig<<(shiftDist - 10) );
        return uZ.f;
    } else {
        return sdouble_roundPackToF64( sign, exp, sig<<shiftDist );
    }
}

double64_t FloatingPoint::sdouble_addMagsF64(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                                             bool signB, int_fast16_t expB, uint_fast64_t sigB,
                                             bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ, int &packingProcess)
{
    int_fast16_t expDiff;
    uint_fast64_t uiZ;
    union ui64_f64 uZ;

    signZ = signA;
    expDiff = expA - expB;
    if ( ! expDiff ) 
    {
        signZ = signA;
        expZ = expA;
        sigZ = UINT64_C( 0x0020000000000000 ) + sigA + sigB;
        sigZ <<= 9;
    } 
    else {

        sigA <<= 9;
        sigB <<= 9;

        if ( expDiff < 0 ) 
        {
            if ( expB == 0x7FF ) 
            {
                expZ = 0x7FF;
                sigZ = 0;
                uiZ = packToF64UI( signZ, 0x7FF, 0 );
                goto uiZ;
            }
            expZ = expB;



            if ( expA ) 
            {
                sigA += UINT64_C( 0x2000000000000000 );
            } else 
            {
                sigA <<= 1;
            }

            sigA = sdouble_shiftRightJam64( sigA, -expDiff );
        } 
        else {
            if ( expA == 0x7FF ) 
            {

            }
            expZ = expA;
            if ( expB ) 
            {
                sigB += UINT64_C( 0x2000000000000000 );
            } 
            else 
            {
                sigB <<= 1;
            }
            sigB = sdouble_shiftRightJam64( sigB, expDiff );
        }

        sigZ = UINT64_C( 0x2000000000000000 ) + sigA + sigB;
        if ( sigZ < UINT64_C( 0x4000000000000000 ) ) 
        {
            --expZ;
            sigZ <<= 1;
        }
    }
    return sdouble_roundPackToF64( signZ, expZ, sigZ );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;
}

double64_t FloatingPoint::sdouble_subMagsF64(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                                             bool signB, int_fast16_t expB, uint_fast64_t sigB,
                                             bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ, int &packingProcess)
{
    int_fast16_t expDiff;
    uint_fast64_t uiZ;
    int_fast64_t sigDiff;
    int_fast8_t shiftDist;
    union ui64_f64 uZ;

    signZ = signA;
    expDiff = expA - expB;
    if ( ! expDiff ) 
    {

        if ( expA == 0x7FF ) 
        {
            uiZ = defaultNaNF64UI;
            goto uiZ;
        }
        sigDiff = sigA - sigB;
        if ( ! sigDiff ) 
        {

            uiZ =
                packToF64UI(
                    (sdouble_roundingMode == sdouble_round_min), 0, 0 );

            signZ = (sdouble_roundingMode == sdouble_round_min);
            expZ = 0;
            sigZ = 0;

            goto uiZ;
        }
        if ( expA ) --expA;
        if ( sigDiff < 0 ) 
        {
            signZ = ! signZ;
            sigDiff = -sigDiff;
        }

        shiftDist = sdouble_countLeadingZeros64( sigDiff ) - 11;
        expZ = expA - shiftDist;
        if ( expZ < 0 ) {
            shiftDist = expA;
            expZ = 0;
        }
        signZ = signZ;
        expZ = expZ;
        sigZ = sigDiff<<shiftDist ;
        packingProcess = 0; 

        uiZ = packToF64UI( signZ, expZ, sigDiff<<shiftDist );
        goto uiZ;
    } else {
        sigA <<= 10;
        sigB <<= 10;
        if ( expDiff < 0 ) 
        {
            signZ = ! signZ;
            if ( expB == 0x7FF )
            {
                signZ = signZ;
                expZ = 0x7FF;
                sigZ = 0 ;
                uiZ = packToF64UI( signZ, 0x7FF, 0 );
                goto uiZ;
            }

            sigA += expA ? UINT64_C( 0x4000000000000000 ) : sigA;
            sigA = sdouble_shiftRightJam64( sigA, -expDiff );
            sigB |= UINT64_C( 0x4000000000000000 );
            expZ = expB;
            sigZ = sigB - sigA;
        } else 
        {
            if ( expA == 0x7FF ) 
            {
                goto uiZ;
            }

            sigB += expB ? UINT64_C( 0x4000000000000000 ) : sigB;
            sigB = sdouble_shiftRightJam64( sigB, expDiff );
            sigA |= UINT64_C( 0x4000000000000000 );
            expZ = expA;
            sigZ = sigA - sigB;
        }
        signZ = signZ;
        expZ = expZ-1;
        sigZ = sigZ;
        packingProcess = 1;
        return sdouble_normRoundPackToF64( signZ, expZ, sigZ );
    }
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}

double64_t FloatingPoint::f64_add(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                                 bool signB, int_fast16_t expB, uint_fast64_t sigB,
                                 bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ, int &packingProcess)
{
    if (signA ^ signB)
    {
        double64_t output_ =  sdouble_subMagsF64(signA, expA, sigA,  signB, expB, sigB, signZ, expZ, sigZ, packingProcess);
        union ui64_f64 uA1_output_;
        uint_fast64_t uiA1_output_;   
        uA1_output_.f = output_;
        uiA1_output_ = uA1_output_.ui;
        expZ  = expF64UI( uiA1_output_ );
        sigZ  = fracF64UI( uiA1_output_ );

        return output_;
    }
    else
    {
        double64_t output_ =   sdouble_addMagsF64(signA, expA, sigA,  signB, expB, sigB, signZ, expZ, sigZ, packingProcess);
        union ui64_f64 uA1_output_;
        uint_fast64_t uiA1_output_;   
        uA1_output_.f = output_;
        uiA1_output_ = uA1_output_.ui;
        expZ  = expF64UI( uiA1_output_ );
        sigZ  = fracF64UI( uiA1_output_ );
        return output_;
    }
}

double64_t FloatingPoint::f64_mul(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                                 bool signB, int_fast16_t expB, uint_fast64_t sigB,
                                 bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ)
{
    uint_fast64_t magBits;
    struct exp16_sig64 normExpSig;
    struct uint128 sig128Z;

    signZ = signA ^ signB;

    if ( expA == 0x7FF ) 
    {
        if ( sigA || ((expB == 0x7FF) && sigB) ) 
        {
            // cout<<"propagateNaN;"<<endl;
        }
        magBits = expB | sigB;
        if ( ! magBits ) 
        {
        // cout<<"defaultNaNF64UI"<<endl;
        } else 
        {
            expZ=0x7FF;
            sigZ=0;
            // uiZ = packToF64UI( signZ, 0x7FF, 0 );
        }
    }
    if ( expB == 0x7FF ) 
    {
        if ( sigB ) 
        {
            // cout<<"propagateNaN;"<<endl;
        }
        magBits = expA | sigA;
        if ( ! magBits )
        {
            // std::cout << "defaultNaNF64UI" <<std::endl;
        }
        else
        {
            expZ = 0x7FF;
            sigZ = 0;
        }
    }

    if ( ! expA ) 
    {
        if ( ! sigA ) 
        {
            expZ = 0;
            sigZ = 0;
        }
        normExpSig = sdouble_normSubnormalF64Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    if ( ! expB ) 
    {
        if ( ! sigB ) 
        {
            expZ = 0;
            sigZ = 0;
        }
        normExpSig = sdouble_normSubnormalF64Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }

    expZ = expA + expB - 0x3FF;
    sigA = (sigA | UINT64_C( 0x0010000000000000 ))<<10;
    sigB = (sigB | UINT64_C( 0x0010000000000000 ))<<11;
    sig128Z = sdouble_mul64To128( sigA, sigB );
    sigZ = sig128Z.v64 | (sig128Z.v0 != 0);
    if ( sigZ < UINT64_C( 0x4000000000000000 ) ) {
        --expZ;
        sigZ <<= 1;
    }
    double64_t output_ = sdouble_roundPackToF64( signZ, expZ, sigZ );

    union ui64_f64 uA1_output_;
    uint_fast64_t uiA1_output_;   

    uA1_output_.f = output_;
    uiA1_output_ = uA1_output_.ui;

    expZ  = expF64UI( uiA1_output_ );
    sigZ  = fracF64UI( uiA1_output_ );

    return output_;
}

FloatingPoint::uint128 FloatingPoint::sdouble_mul64To128(uint64_t a, uint64_t b)
{
    uint32_t a32, a0, b32, b0;
    struct uint128 z;
    uint64_t mid1, mid;

    a32 = a>>32;
    a0 = a;
    b32 = b>>32;
    b0 = b;
    z.v0 = (uint_fast64_t) a0 * b0;
    mid1 = (uint_fast64_t) a32 * b0;
    mid = mid1 + (uint_fast64_t) a0 * b32;
    z.v64 = (uint_fast64_t) a32 * b32;
    z.v64 += (uint_fast64_t) (mid < mid1)<<32 | mid>>32;
    mid <<= 32;
    z.v0 += mid;
    z.v64 += (z.v0 < mid);
    return z;
}

uint32_t FloatingPoint::sdouble_approxRecip32_1( uint32_t a )
{
    int index;
    uint16_t eps, r0;
    uint32_t sigma0;
    uint_fast32_t r;
    uint32_t sqrSigma0;

    index = a>>27 & 0xF;
    eps = (uint16_t) (a>>11);
    r0 = sdouble_approxRecip_1k0s[index]
             - ((sdouble_approxRecip_1k1s[index] * (uint_fast32_t) eps)>>20);
    sigma0 = ~(uint_fast32_t) ((r0 * (uint_fast64_t) a)>>7);
    r = ((uint_fast32_t) r0<<16) + ((r0 * (uint_fast64_t) sigma0)>>24);
    sqrSigma0 = ((uint_fast64_t) sigma0 * sigma0)>>32;
    r += ((uint32_t) r * (uint_fast64_t) sqrSigma0)>>48;
    return r;
}

double64_t FloatingPoint::f64_div(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                                 bool signB, int_fast16_t expB, uint_fast64_t sigB,
                                 bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ)
{
    struct exp16_sig64 normExpSig;
    uint32_t recip32, sig32Z, doubleTerm;
    uint_fast64_t rem;
    uint32_t q;
    uint_fast64_t uiZ;
    union ui64_f64 uZ;

    signZ = signA ^ signB;
    if ( expA == 0x7FF ) {
        if ( expB == 0x7FF ) {
        }
    }
    if ( expB == 0x7FF ) {
        expZ  = 0;
        sigZ  = 0;
        uiZ = packToF64UI( signZ, 0, 0 );
    }

    if ( ! expB ) {
        if ( ! sigB ) {

        }
        normExpSig = sdouble_normSubnormalF64Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    if ( ! expA ) {
        if ( ! sigA )
        {
                expZ  = 0;
                sigZ  = 0;
                uiZ = packToF64UI( signZ, 0, 0 );
        }
        normExpSig = sdouble_normSubnormalF64Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }

    expZ = expA - expB + 0x3FE;
    sigA |= UINT64_C( 0x0010000000000000 );
    sigB |= UINT64_C( 0x0010000000000000 );
    if ( sigA < sigB ) {
        --expZ;
        sigA <<= 11;
    } else {
        sigA <<= 10;
    }
    sigB <<= 11;
    recip32 = sdouble_approxRecip32_1( sigB>>32 ) - 2;
    sig32Z = ((uint32_t) (sigA>>32) * (uint_fast64_t) recip32)>>32;
    doubleTerm = sig32Z<<1;
    rem =
        ((sigA - (uint_fast64_t) doubleTerm * (uint32_t) (sigB>>32))<<28)
            - (uint_fast64_t) doubleTerm * ((uint32_t) sigB>>4);
    q = (((uint32_t) (rem>>32) * (uint_fast64_t) recip32)>>32) + 4;
    sigZ = ((uint_fast64_t) sig32Z<<32) + ((uint_fast64_t) q<<4);
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( (sigZ & 0x1FF) < 4<<4 ) {
        q &= ~7;
        sigZ &= ~(uint_fast64_t) 0x7F;
        doubleTerm = q<<1;
        rem =
            ((rem - (uint_fast64_t) doubleTerm * (uint32_t) (sigB>>32))<<28)
                - (uint_fast64_t) doubleTerm * ((uint32_t) sigB>>4);
        if ( rem & UINT64_C( 0x8000000000000000 ) ) {
            sigZ -= 1<<7;
        } else {
            if ( rem ) sigZ |= 1;
        }
    }
    double64_t output_ = sdouble_roundPackToF64( signZ, expZ, sigZ );

    union ui64_f64 uA1_output_;
    uint_fast64_t uiA1_output_;   

    uA1_output_.f = output_;
    uiA1_output_ = uA1_output_.ui;

    expZ  = expF64UI( uiA1_output_ );
    sigZ  = fracF64UI( uiA1_output_ );

    return output_;

 uiZ:
    uZ.ui = uiZ;
    return uZ.f;
}
