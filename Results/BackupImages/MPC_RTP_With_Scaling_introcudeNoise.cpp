#include <iostream>
#include <bits/stdc++.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits>
#include <thread>
#include <chrono>


using namespace std;

#ifdef __GNUC_STDC_INLINE__

#define INLINE inline
#else
#define INLINE extern inline
#endif
#define sdouble_BUILTIN_CLZ 1
#define sdouble_INTRINSIC_INT128 1
#define init_detectTininess sdouble_tininess_afterRounding


struct exp16_sig64 { int_fast16_t exp; uint_fast64_t sig; };

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
#define sdouble_FAST_INT64 1
#define sdouble_FAST_INT64 1
#define sdouble_isSigNaNF32UI( uiA ) ((((uiA) & 0x7FC00000) == 0x7F800000) && ((uiA) & 0x003FFFFF))
#define sdouble_isSigNaNF64UI( uiA ) ((((uiA) & UINT64_C( 0x7FF8000000000000 )) == UINT64_C( 0x7FF0000000000000 )) && ((uiA) & UINT64_C( 0x0007FFFFFFFFFFFF )))
union ui64_f64 { uint64_t ui; double64_t f; };


const uint16_t sdouble_approxRecip_1k0s[16] = {
    0xFFC4, 0xF0BE, 0xE363, 0xD76F, 0xCCAD, 0xC2F0, 0xBA16, 0xB201,
    0xAA97, 0xA3C6, 0x9D7A, 0x97A6, 0x923C, 0x8D32, 0x887E, 0x8417
};
const uint16_t sdouble_approxRecip_1k1s[16] = {
    0xF0F1, 0xD62C, 0xBFA1, 0xAC77, 0x9C0A, 0x8DDB, 0x8185, 0x76BA,
    0x6D3B, 0x64D4, 0x5D5C, 0x56B1, 0x50B6, 0x4B55, 0x4679, 0x4211
};


#define ERROR .1
// #define SCALE_FACTOR 10000
#define bk 0.276
#define LAMBDA 21
// #define numconsumers 10
// #define numsuppliers 3
#define EXPONENTSIZE 2
#define BITLENGTH 32
#define PRECISION 32
#define ETA .5



double minPricingValues = 1.00000000000;

int numconsumers = 1000000;
int numsuppliers = 10;




// need to generate based on the distribution

double d[10] = {6.57312, 3.19614, 9.39501, 3.23684, 7.11644, 9.60693, 7.11762, 5.15677, 8.61886, 7.70245};
double eph[10] = {-0.812197, -0.908682, -0.731571, -0.907519, -0.796673, -0.725516, -0.796639, -0.852664, -0.753747, -0.77993};


struct doubleNumber
{
    bool sign;
    int_fast16_t exp;
    uint_fast64_t sig;
};


uint64_t sdouble_shiftRightJam64( uint64_t a, uint_fast32_t dist )
{

    return (dist < 63) ? a>>dist | ((uint64_t) (a<<(-dist & 63)) != 0) : (a != 0);
}

enum 
{
    sdouble_round_near_even   = 0,
    sdouble_round_minMag      = 1,
    sdouble_round_min         = 2,
    sdouble_round_max         = 3,
    sdouble_round_near_maxMag = 4,
    sdouble_round_odd         = 6
};

extern uint_fast8_t sdouble_roundingMode = sdouble_round_near_even;


enum {
    sdouble_tininess_beforeRounding = 0,
    sdouble_tininess_afterRounding  = 1
};

extern uint_fast8_t sdouble_detectTininess = init_detectTininess;

const uint_least8_t sdouble_countLeadingZeros8[256] = {
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


double alpha1 =  2.04999713368002112190424668369814753532409667968750;
double alpha2 = 0.36462958398842182372590059458161704242229461669922;


/* -------------------------------------------------------------------------without MPC Start---------------------------------------------------------------------------------*/

void computeConsumerPricingSingaldouble(vector<double> in_consumerPrices, vector<double> in_demand, vector<double> in_wDotLambdaZero, double in_supply, double in_sDotLambdaZero, vector<double> &out_consumerPrices, vector<double> &out_schedulingError, int counter)
{
    out_consumerPrices.clear();

    double t_totalPrice = 0;
    for(int i = 0; i < numconsumers ; i++)
    {
        t_totalPrice = t_totalPrice + in_consumerPrices[i];
    }
    // cout<<"Using double total price should be:"<<t_totalPrice<<endl;
    // cout << "Total Supply without using MPC: " << in_supply <<endl;

    vector<double> t_ChangedPrice;
    t_ChangedPrice.clear();

    for(int i = 0; i < numconsumers ; i++)
    {

        double term2 = in_demand[i] ;

        double term1_1 = in_consumerPrices[i]/t_totalPrice;

        double term1 = in_supply * term1_1;

        double temp1 =  term1 - term2;

        double constant_1 = in_sDotLambdaZero - in_wDotLambdaZero[i];

        double constant = (2.0000 * ETA)/constant_1;

        double temp = temp1 * constant;

        t_ChangedPrice.push_back(temp);

        out_schedulingError.push_back(temp1);

        if(i == 0)
        {
            cout << "NORMAL SCENARIO" <<endl;
           if((counter == 1) || (counter == 2) || (counter == 3))
            {
                cout << "Counter: " << counter <<endl;

                cout << "Price: " << t_totalPrice <<endl;


                cout << "Supply: " << in_supply <<endl;

                cout << "Division: " << in_supply/t_totalPrice <<endl;

                cout << "GSE :    " << temp1 <<endl;

                cout << "Changed in price: " << temp <<endl;

            }

        }
    }

    // cout << "factor value using normal (without MPC)...." <<endl;
    for(int i = 0; i < numconsumers ; i++)
    {
        // cout << t_ChangedPrice[i] << ",";

        {
            out_consumerPrices.push_back(in_consumerPrices[i] - t_ChangedPrice[i]);
        }   
    }

    // cout <<endl;

}


void computeSupplierPricingSingaldouble(vector<double> in_consumerPrices, vector<double> in_demand, vector<double> in_wDotLambdaZero, double in_supply, double in_sDotLambdaZero, vector<double> &out_consumerPrices)
{
    out_consumerPrices.clear();

    double t_totalPrice = 0;
    for(int i = 0; i < numsuppliers ; i++)
    {
        t_totalPrice = t_totalPrice + in_consumerPrices[i];
    }

    vector<double> t_ChangedPrice;
    t_ChangedPrice.clear();

    for(int i = 0; i < numsuppliers ; i++)
    {

        double term1 = in_demand[i] ;

        double term2_1 = in_consumerPrices[i]/t_totalPrice;

        double term2 = in_supply * term2_1;


        double temp1 =  term1 - term2;

        double constant_1 = in_wDotLambdaZero[i] - in_sDotLambdaZero;

        double constant = (2.0000 * ETA) / constant_1;

        double temp = temp1 * constant;


        t_ChangedPrice.push_back(temp);
    }

    // cout << "factor value using normal (without MPC)...." <<endl;
    for(int i = 0; i < numsuppliers ; i++)
    {
        {
            out_consumerPrices.push_back(in_consumerPrices[i] - t_ChangedPrice[i]);
        }
    }
}

/* -------------------------------------------------------------------------without MPC End---------------------------------------------------------------------------------*/


doubleNumber getSignExpSig(double in_number)
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


int getNumberOfDigits(double number) 
{
    // Convert the double to a string
    std::string numberString = std::to_string(number);

    // Find the position of the decimal point
    size_t decimalPosition = numberString.find('.');

    // Calculate the number of digits before the decimal point
    int numberOfDigits = 0;

    // Count the digits before the decimal point
    for (size_t i = 0; i < decimalPosition; ++i) {
        if (isdigit(numberString[i])) {
            ++numberOfDigits;
        }
    }

    return numberOfDigits;
}



double rePackForTesting(doubleNumber in_number)
{
    uint_fast64_t uiZ;
    union ui64_f64 uZ;

    uiZ = packToF64UI(in_number.sign, in_number.exp, in_number.sig);
    uZ.ui  = uiZ;
    double64_t in_number_reconstruct =uZ.f;
    double t_x1 = *(double*) &in_number_reconstruct; 

    return t_x1;
}

double rePackForTesting(bool sign, int_fast16_t exp, uint_fast64_t sig)
{
    doubleNumber in_number;

    in_number.sign = sign;
    in_number.exp = exp;
    in_number.sig = sig;

    double temp_value = rePackForTesting(in_number);

    return temp_value;
}

void divideSupplyValuesIntoShares(double in_values, vector<double> &out_shareOfEachvalue, int in_choice)
{

    vector<double> t_sampleRandomValues;

    // for(int i =0 ; i < numsuppliers; i++)
    // {
    //     double randValue = rand();

    //     t_sampleRandomValues.push_back(randValue);

    //     cout << "randValue:  "<< randValue <<endl;
    // }

    if(in_choice == 1)
    {
        t_sampleRandomValues = {14,17,28,26,24,26,17,13,10,2};
    }
    else if(in_choice == 2)
    {
        t_sampleRandomValues = {10,18,21,20,4,17,1,7,23,17};
    }

    double t_totalSamplingSize = 0;

    for(int i =0 ; i < numsuppliers; i++)
    {
        t_totalSamplingSize = t_totalSamplingSize + t_sampleRandomValues[i];
    }

    // cout << "t_totalSamplingSize:    "<< t_totalSamplingSize <<endl;

    vector<double> t_sampleRandomValueFractions;


    for(int i =0 ; i < numsuppliers - 1; i++)
    {

        double temp_value = t_sampleRandomValues[i]/t_totalSamplingSize;

        t_sampleRandomValueFractions.push_back(temp_value);

        // cout << "temp_value: " << temp_value <<endl;

    }

    double t_totalfractionvalue = 0;

    for(int i =0 ; i < t_sampleRandomValueFractions.size(); i++)
    {
        t_totalfractionvalue = t_totalfractionvalue + t_sampleRandomValueFractions[i];
    }


    double finalFraction = (double)(1.0000000000000000) - t_totalfractionvalue;

    // cout << "finalFraction:  " << finalFraction<<endl;

    t_sampleRandomValueFractions.push_back(finalFraction);

    for(int i =0 ; i < numsuppliers;i++)
    {
        out_shareOfEachvalue.push_back(in_values * t_sampleRandomValueFractions[i]);
    }

}


uint_fast8_t sdouble_countLeadingZeros64( uint64_t a )
{
    uint_fast8_t count;
    uint32_t a32;

    count = 0;
    a32 = a>>32;
    if ( ! a32 ) {
        count = 32;
        a32 = a;
    }
    /*------------------------------------------------------------------------
    | From here, result is current count + count leading zeros of `a32'.
    *------------------------------------------------------------------------*/
    if ( a32 < 0x10000 ) {
        count += 16;
        a32 <<= 16;
    }
    if ( a32 < 0x1000000 ) {
        count += 8;
        a32 <<= 8;
    }
    count += sdouble_countLeadingZeros8[a32>>24];
    return count;

}

struct exp16_sig64 sdouble_normSubnormalF64Sig( uint_fast64_t sig )
{
    int_fast8_t shiftDist;
    struct exp16_sig64 z;

    shiftDist = sdouble_countLeadingZeros64( sig ) - 11;
    z.exp = 1 - shiftDist;
    z.sig = sig<<shiftDist;
    return z;

}

double64_t sdouble_roundPackToF64( bool sign, int_fast16_t exp, uint_fast64_t sig )
{
    // cout << "sdouble_roundPackToF64: " << exp << endl;

    // cout << "sdouble_roundPackToF64: " << sig << endl;

    uint_fast8_t roundingMode;
    bool roundNearEven;
    uint_fast16_t roundIncrement, roundBits;
    bool isTiny;
    uint_fast64_t uiZ;
    union ui64_f64 uZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    roundingMode = sdouble_roundingMode;

    roundNearEven = (roundingMode == sdouble_round_near_even);
    
    roundIncrement = 0x200;
    
    if ( ! roundNearEven && (roundingMode != sdouble_round_near_maxMag) ) 
    {
        roundIncrement = (roundingMode == (sign ? sdouble_round_min : sdouble_round_max)) ? 0x3FF : 0;
    }

    roundBits = sig & 0x3FF;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( 0x7FD <= (uint16_t) exp ) 
    {
        if ( exp < 0 ) 
        {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            isTiny =
                (sdouble_detectTininess == sdouble_tininess_beforeRounding)
                    || (exp < -1)
                    || (sig + roundIncrement < UINT64_C( 0x8000000000000000 ));
            sig = sdouble_shiftRightJam64( sig, -exp );
            exp = 0;
            roundBits = sig & 0x3FF;
            if ( isTiny && roundBits ) 
            {
                // cout<<"sdouble_raiseFlags( sdouble_flag_underflow );"<<endl;
            }
        } 
        else if ((0x7FD < exp) || (UINT64_C( 0x8000000000000000 ) <= sig + roundIncrement)) 
        {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            // cout<<"exp in sdouble_roundPackToF64: "<<exp<<endl;
            // cout<<"sdouble_raiseFlags(sdouble_flag_overflow | sdouble_flag_inexact );"<<endl;
            uiZ = packToF64UI( sign, 0x7FF, 0 ) - ! roundIncrement;
            goto uiZ;
        }
    }




    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sig = (sig + roundIncrement)>>10;
    
    if ( roundBits ) 
    {
        //sdouble_exceptionFlags |= sdouble_flag_inexact;
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
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/

    


    
 packReturn:
     // cout << exp << endl;
    
    // cout << sig << endl;
    uiZ = packToF64UI( sign, exp, sig );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}

double64_t sdouble_normRoundPackToF64( bool sign, int_fast16_t exp, uint_fast64_t sig )
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


double64_t sdouble_subMagsF64(bool signA, int_fast16_t expA, uint_fast64_t sigA,  bool signB, int_fast16_t expB, uint_fast64_t sigB, bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ, int &packingProcess)
{
    // int_fast16_t expA;
    // uint_fast64_t sigA;
    // int_fast16_t expB;
    // uint_fast64_t sigB;
    int_fast16_t expDiff;
    uint_fast64_t uiZ;
    int_fast64_t sigDiff;
    int_fast8_t shiftDist;
    // int_fast16_t expZ;
    // uint_fast64_t sigZ;
    union ui64_f64 uZ;

    signZ = signA;

    // cout << signA << "  " <<expA << "   " << sigA <<endl;

    // cout << signB << "  " <<expB << "   " << sigB <<endl;

    //     printf("expA \t%.15d\n\n", expA);
    // printf("sigA \t%.15lld\n\n", sigA);

    // printf("expB \t%.15d\n\n", expB);
    // printf("sigB \t%.15lld\n\n", sigB);

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiff = expA - expB;
    if ( ! expDiff ) 
    {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        if ( expA == 0x7FF ) 
        {
            //if ( sigA | sigB ) goto propagateNaN;
            // cout<<"sdouble_raiseFlags( sdouble_flag_invalid );"<<endl;
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
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        sigA <<= 10;
        sigB <<= 10;
        if ( expDiff < 0 ) 
        {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            signZ = ! signZ;
            if ( expB == 0x7FF )
            {
               // if ( sigB ) goto propagateNaN;
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
        } else {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            if ( expA == 0x7FF ) {
               // if ( sigA ) goto propagateNaN;
               // uiZ = uiA;
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
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 // propagateNaN:
 //    uiZ = sdouble_propagateNaNF64UI( uiA, uiB );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}


double64_t sdouble_addMagsF64(bool signA, int_fast16_t expA, uint_fast64_t sigA,  bool signB, int_fast16_t expB, uint_fast64_t sigB, bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ, int &packingProcess)
{
    // int_fast16_t expA;
    // uint_fast64_t sigA;
    // int_fast16_t expB;
    // uint_fast64_t sigB;
    int_fast16_t expDiff;
    uint_fast64_t uiZ;
    // int_fast16_t expZ;
    // uint_fast64_t sigZ;
    union ui64_f64 uZ;

    // cout << signA << "  " <<expA << "   " << sigA <<endl;

    // cout << signB << "  " <<expB << "   " << sigB <<endl;


    signZ = signA;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    // expA = expF64UI( uiA );
    // sigA = fracF64UI( uiA );
    // expB = expF64UI( uiB );
    // sigB = fracF64UI( uiB );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiff = expA - expB;
    if ( ! expDiff ) 
    {
        // cout << "111111111111111111111111111111111111111111111111111111111" <<endl;
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        if ( ! expA ) {
            // uiZ = uiA + sigB;
            // goto uiZ;
        }
        if ( expA == 0x7FF ) {
            // if ( sigA | sigB ) goto propagateNaN;
            // uiZ = uiA;
            // goto uiZ;
        }
        signZ = signA;
        expZ = expA;
        sigZ = UINT64_C( 0x0020000000000000 ) + sigA + sigB;
        sigZ <<= 9;
    } 
    else {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        // cout << "hereeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" <<endl;

        sigA <<= 9;
        sigB <<= 9;

            // cout << sigA <<endl;

        // cout << sigB <<endl;

        if ( expDiff < 0 ) 
        {
             // cout << "2222222222222222222222222222222222222222222" <<endl; 

            // cout << expDiff <<endl;

            if ( expB == 0x7FF ) 
            {
                // cout << "3333333333333333333333333333333333333333333333" <<endl;
                //if ( sigB ) goto propagateNaN;
                expZ = 0x7FF;
                sigZ = 0;
                uiZ = packToF64UI( signZ, 0x7FF, 0 );
                goto uiZ;
            }
            expZ = expB;

            // cout << expZ <<endl;

            if ( expA ) 
            {
                // cout << "44444444444444444444444444444444444444444444444444" <<endl;
                sigA += UINT64_C( 0x2000000000000000 );
            } else 
            {
                // cout << "55555555555555555555555555555555555555555555555555555555" <<endl;
                sigA <<= 1;
            }
            // cout << "666666666666666666666666666666666666666666666666666666666" <<endl;
            // cout << sigA <<endl;
            sigA = sdouble_shiftRightJam64( sigA, -expDiff );
            // cout << sigA <<endl;
        } 
        else {
            if ( expA == 0x7FF ) 
            {
                // if ( sigA ) goto propagateNaN;
                // uiZ = uiA;
                // goto uiZ;
                // cout << "77777777777777777777777777777777777777777777" <<endl;
            }
            expZ = expA;
            if ( expB ) 
            {
                // cout << "888888888888888888888888888888888888888888888888888888" <<endl;
                sigB += UINT64_C( 0x2000000000000000 );
            } 
            else 
            {
                // cout << "99999999999999999999999999999999999999999999999999999999999999" <<endl;
                sigB <<= 1;
            }
            // cout << "101010101010101010101010101010101010101010101010101010101010" <<endl;
            sigB = sdouble_shiftRightJam64( sigB, expDiff );
        }
        // cout << "101010101010101010101010101010101010101010101010101010101010.1111111111111111111111111111111" <<endl;

            // cout << sigA <<endl;

        // cout << sigB <<endl;
        sigZ = UINT64_C( 0x2000000000000000 ) + sigA + sigB;

         // cout << sigZ <<endl;
        if ( sigZ < UINT64_C( 0x4000000000000000 ) ) 
        {
            // cout << "101010101010101010101010101010101010101010101010101010101010.2222222222222222222222222222222222222222" <<endl;
            --expZ;
            sigZ <<= 1;
        }

                    // cout << expZ <<endl;

        // cout << sigB <<endl;

    }


                    // cout << expZ <<endl;

        // cout << sigZ <<endl;
    // packingProcess = 2;

    //     std::cout << std::fixed << std::showpoint;
    // std::cout << std::setprecision(400);

    //     cout << signZ <<endl;

    // cout << expZ <<endl;

    // cout << sigZ <<endl;

    // cout << "Repack:    " << rePackForTesting(signZ, expZ, sigZ) <<endl;


    return sdouble_roundPackToF64( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 // propagateNaN:
 //    uiZ = sdouble_propagateNaNF64UI( uiA, uiB );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}

struct uint128 sdouble_mul64To128( uint64_t a, uint64_t b )
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


uint32_t sdouble_approxRecip32_1( uint32_t a )
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


double64_t f64_mul(bool signA, int_fast16_t expA, uint_fast64_t sigA,  bool signB, int_fast16_t expB, uint_fast64_t sigB, bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ)
{
    uint_fast64_t magBits;
    struct exp16_sig64 normExpSig;
    struct uint128 sig128Z;


    signZ = signA ^ signB;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
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
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
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
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
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
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/

}




double64_t f64_add(bool signA, int_fast16_t expA, uint_fast64_t sigA,  bool signB, int_fast16_t expB, uint_fast64_t sigB, bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ, int &packingProcess)
{
    if(signA^signB)
    {
        // cout << "sdouble_subMagsF64" <<endl;
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

        // cout << "sdouble_addMagsF64" <<endl;
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


double64_t f64_div(bool signA, int_fast16_t expA, uint_fast64_t sigA,  bool signB, int_fast16_t expB, uint_fast64_t sigB, bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ)
{
    // union ui64_f64 uA;
    // uint_fast64_t uiA;
    // bool signA;
    // int_fast16_t expA;
    // uint_fast64_t sigA;
    // union ui64_f64 uB;
    // uint_fast64_t uiB;
    // bool signB;
    // int_fast16_t expB;
    // uint_fast64_t sigB;
    // bool signZ;
    

    struct exp16_sig64 normExpSig;
    // int_fast16_t expZ;
    uint32_t recip32, sig32Z, doubleTerm;
    uint_fast64_t rem;
    uint32_t q;
    // uint_fast64_t sigZ;
    uint_fast64_t uiZ;
    union ui64_f64 uZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    signZ = signA ^ signB;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( expA == 0x7FF ) {
       // if ( sigA ) goto propagateNaN;
        if ( expB == 0x7FF ) {
           // if ( sigB ) goto propagateNaN;
            //goto invalid;
        }
       // goto infinity;
    }
    if ( expB == 0x7FF ) {
        //if ( sigB ) goto propagateNaN;
        expZ  = 0;
        sigZ  = 0;
        uiZ = packToF64UI( signZ, 0, 0 );
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( ! expB ) {
        if ( ! sigB ) {
            //if ( ! (expA | sigA) ) goto invalid;
            // cout<<"sdouble_raiseFlags( sdouble_flag_infinite )"<<endl;
            // goto infinity/;
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
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
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
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 // propagateNaN:
 //    uiZ = sdouble_propagateNaNF64UI( uiA, uiB );
 //    goto uiZ;
 //    /*------------------------------------------------------------------------
 //    *------------------------------------------------------------------------*/
 // invalid:
 //    sdouble_raiseFlags( sdouble_flag_invalid );
 //    uiZ = defaultNaNF64UI;
 //    goto uiZ;
 //    /*------------------------------------------------------------------------
 //    *------------------------------------------------------------------------*/
 // infinity:
 //    uiZ = packToF64UI( signZ, 0x7FF, 0 );
 //    goto uiZ;
 //    /*------------------------------------------------------------------------
 //    *------------------------------------------------------------------------*/
 // zero:
 //    uiZ = packToF64UI( signZ, 0, 0 );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;

}

vector<doubleNumber> obtainShares(double in_number, int in_cond)
{

    static int counterShares = 0;

    counterShares++;

    // POC Implementation provide output shares f1, f2, f3 for a secret floating point number f, such that f = f1 + f2 + f3

    double maxValueOfDouble = std::numeric_limits<double>::max();

    double ScaleCounter = -300;

    double num1 = std::numeric_limits<double>::infinity(); // Positive infinity
    double num2 = -std::numeric_limits<double>::infinity(); // Negative infinity
    double num3 = std::numeric_limits<double>::quiet_NaN(); // NaN
    double num4 = -std::numeric_limits<double>::quiet_NaN(); // -NaN

    double SclaeCompareLimit = pow(10,-300);

    while(1)
    {
        // cout << "ScaleCounter:  " << ScaleCounter <<endl;

        double OriginalNumber = in_number;

        // cout << "--------------------------------------------------------------------------------------------------------------------------" <<endl;
        // double ScaleDown = pow(10, ScaleCounter);

        // double ScaleValue = maxValueOfDouble/(OriginalNumber * ScaleDown);

        // union ui64_f64 uA1;
        // uint_fast64_t uiA1;
        // bool signA_ScalingFactor;
        // int_fast16_t expA_ScalingFactor;
        // uint_fast64_t sigA_ScalingFactor;

        // uA1.f = *(double64_t*) &ScaleValue;
        // uiA1 = uA1.ui;
        // signA_ScalingFactor = signF64UI(uiA1);
        // expA_ScalingFactor  = expF64UI(uiA1);
        // sigA_ScalingFactor  = fracF64UI(uiA1);


        // double ScaleValue = pow(10, ScaleCounter);
        // OriginalNumber = OriginalNumber * (ScaleValue);

        if (std::isinf(OriginalNumber) || std::isnan(OriginalNumber) || (abs(OriginalNumber) == 0)) 
        {
            // std::cout << "num5 is a regular number." << std::endl;
            // ScaleCounter++;
            continue;
        }


        int packingProcess  = 1 ;

        vector<doubleNumber> finalShares;

        union ui64_f64 uA;
        uint_fast64_t uiA;
        bool signA;
        int_fast16_t expA;
        uint_fast64_t sigA;

        uA.f = *(double64_t*) &OriginalNumber;
        uiA = uA.ui;
        signA = signF64UI(uiA);
        expA  = expF64UI(uiA);
        sigA  = fracF64UI(uiA);

        // Share Sign bit : 1-bit => mod 2

        int sign_1 = (rand() % 2);
        int sign_2 = (rand() % 2);


        // Share Exponent bits : 8-bit => mod 2^8

        int min = 0;

        int max = pow(2,11) - 1 ;   //pow(2,10); 

        int minValue =  0 ;//pow(2,10) - 1; // Highest 2^10

        int exp_1 = minValue + rand() % (( max));
        int exp_2 = minValue + rand() % (( max));


        // Share Significand bits : 23-bit => mod 2^23

        long long int modValue = pow(2, 52);
        int sig_1 = (rand() % modValue);
        int sig_2 = (rand() % modValue);

        // Assigning to doubleNumber

        doubleNumber t_share1, t_share2, t_share3;

        t_share1.sign = sign_1;
        t_share1.exp  = exp_1;
        t_share1.sig  = sig_1;

        t_share2.sign = sign_2;
        t_share2.exp  = exp_2;
        t_share2.sig  = sig_2;
      

        double t_outputShare1 = rePackForTesting(t_share1);

        double t_outputShare2 = rePackForTesting(t_share2);

     

        bool signA3;
        int_fast16_t expA3;
        uint_fast64_t sigA3;


        f64_add(t_share1.sign, t_share1.exp, t_share1.sig, t_share2.sign, t_share2.exp, t_share2.sig, signA3, expA3, sigA3, packingProcess);

        double SumShares = t_outputShare1 + t_outputShare2;

        signA3 = signA3 ^ 1;


        f64_add(signA, expA, sigA, signA3, expA3, sigA3, t_share3.sign , t_share3.exp , t_share3.sig, packingProcess);

        double t_outputShare3 = rePackForTesting(t_share3);


        if (std::isinf(t_outputShare1) || std::isnan(t_outputShare1)) 
        {
            // ScaleCounter++;
            continue;
        }

        if (std::isinf(t_outputShare2) || std::isnan(t_outputShare2)) 
        {
            // ScaleCounter++;
            continue;
        }


        if (std::isinf(t_outputShare3) || std::isnan(t_outputShare3)) 
        {
            // ScaleCounter++;
            continue;
        }


        // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

        // cout << "Share1: " << rePackForTesting(t_share1) <<endl;
        // cout << t_share1.sign <<endl;
        // cout << t_share1.exp <<endl;
        // cout << t_share1.sig <<endl;

        // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

        // cout << "Share2: " << rePackForTesting(t_share2) <<endl;
        // cout << t_share2.sign <<endl;
        // cout << t_share2.exp <<endl;
        // cout << t_share2.sig <<endl;

        // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

        // cout << "Share3: " << rePackForTesting(t_share3) <<endl;
        // cout << t_share3.sign <<endl;
        // cout << t_share3.exp <<endl;
        // cout << t_share3.sig <<endl;

        // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;


        // union ui64_f64 uA1;
        // uint_fast64_t uiA1;
        // bool signA_ScalingFactor;
        // int_fast16_t expA_ScalingFactor;
        // uint_fast64_t sigA_ScalingFactor;

        // uA1.f = *(double64_t*) &ScaleValue;
        // uiA1 = uA1.ui;
        // signA_ScalingFactor = signF64UI(uiA1);
        // expA_ScalingFactor  = expF64UI(uiA1);
        // sigA_ScalingFactor  = fracF64UI(uiA1);



        // doubleNumber share1, share2, share3;


        // doubleNumber x1;
        // double64_t x1_temp = f64_div(t_share1.sign , t_share1.exp, t_share1.sig, signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor, share1.sign, share1.exp, share1.sig);

        // doubleNumber x2;
        // double64_t x2_temp = f64_div(t_share2.sign , t_share2.exp, t_share2.sig, signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor, share2.sign, share2.exp, share2.sig);

        // doubleNumber x3;
        // double64_t x3_temp = f64_div(t_share3.sign , t_share3.exp, t_share3.sig, signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor, share3.sign, share3.exp, share3.sig);






        double FinalAddedShares =  ((rePackForTesting(t_share1) +  rePackForTesting(t_share2) +  rePackForTesting(t_share3)));


        // cout << "OriginalNumber:    " << OriginalNumber <<endl;
        // cout << "FinalAddedShares:    " << FinalAddedShares <<endl;

        // cout << "Error Diff: " << abs(in_number - FinalAddedShares) <<endl;


        if(abs(in_number - FinalAddedShares) < SclaeCompareLimit)
        {

                // if(in_cond == 0)
                // {
                //     cout << "Share1: " << t_outputShare1 <<endl;
                //     cout << "Share2: " << t_outputShare2 <<endl;
                //     cout << "Share3: " << t_outputShare3 <<endl;
                
                //     cout << "Total: " << t_outputShare1 + t_outputShare2 + t_outputShare3 <<endl;

                //     cout << "Org  : " << in_number <<endl;
                // }



            // cout << "ScaleValue: " << ScaleValue <<endl;


                if((counterShares % 6)  == 0)
                {
                    finalShares.push_back(t_share1);
                    finalShares.push_back(t_share2);
                    finalShares.push_back(t_share3);                
                }

                if((counterShares % 6)  == 1)
                {
                    finalShares.push_back(t_share1);
                    finalShares.push_back(t_share3);
                    finalShares.push_back(t_share2);
                }

                if((counterShares % 6)  == 2)
                {
                    finalShares.push_back(t_share2);
                    finalShares.push_back(t_share1);
                    finalShares.push_back(t_share3);
                }

                if((counterShares % 6)  == 3)
                {
                    finalShares.push_back(t_share2);
                    finalShares.push_back(t_share3);
                    finalShares.push_back(t_share1);             
                }

                if((counterShares % 6)  == 4)
                {
                    finalShares.push_back(t_share3);
                    finalShares.push_back(t_share1);
                    finalShares.push_back(t_share2); 
                }

                if((counterShares % 6)  == 5)
                {
                    finalShares.push_back(t_share3);
                    finalShares.push_back(t_share2);
                    finalShares.push_back(t_share1);    
                }



            // }



            // cout << "Original Secret : " << in_number <<endl;
            // cout << "Total Secret Sum: " <<  ((rePackForTesting(t_share1) +  rePackForTesting(t_share2) +  rePackForTesting(t_share3))) <<endl;

            // // cout << "Share1: " << rePackForTesting(share1) <<endl;
            // // cout << "Share2: " << rePackForTesting(share2) <<endl;
            // // cout << "Share3: " << rePackForTesting(share3) <<endl;


            // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

            // cout << "Share1: " << rePackForTesting(t_share1) <<endl;
            // cout << t_share1.sign <<endl;
            // cout << t_share1.exp <<endl;
            // cout << t_share1.sig <<endl;

            // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

            // cout << "Share2: " << rePackForTesting(t_share2) <<endl;
            // cout << t_share2.sign <<endl;
            // cout << t_share2.exp <<endl;
            // cout << t_share2.sig <<endl;

            // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

            // cout << "Share3: " << rePackForTesting(t_share3) <<endl;
            // cout << t_share3.sign <<endl;
            // cout << t_share3.exp <<endl;
            // cout << t_share3.sig <<endl;

            // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;


            
            // cout << "Sum of Two Shares Should be: " << SumShares <<endl;
            // cout << "Sum of Two Shares is       : " << rePackForTesting(signA3, expA3, sigA3) <<endl;
            
            // cout << "Share3 should be: " <<  in_number - (SumShares) << endl;
             
            
          
            
   
            return finalShares;
        }
        else
        {
            // cout << "Decreasing Counter..." <<endl;
            // ScaleCounter++;
            continue;
        }


    }

}


double64_t MultiPartyAdditionProtocol(doubleNumber x1, doubleNumber x2, doubleNumber x3, doubleNumber y1, doubleNumber y2, doubleNumber y3, doubleNumber &out_totalSum)
{

    // cout << "Value of x1: "  <<endl;//<< rePackForTesting(t_share1) <<endl;
    // cout << x1.sign <<endl;
    // cout << x1.exp <<endl;
    // cout << x1.sig <<endl;

    // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // cout << "Value of x2: "  <<endl;//<< rePackForTesting(t_share1) <<endl;
    // cout << x2.sign <<endl;
    // cout << x2.exp <<endl;
    // cout << x2.sig <<endl;

    // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // cout << "Value of x3: "  <<endl;//<< rePackForTesting(t_share1) <<endl;
    // cout << x3.sign <<endl;
    // cout << x3.exp <<endl;
    // cout << x3.sig <<endl;

    // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // cout << "Value of y1: "  <<endl;//<< rePackForTesting(t_share1) <<endl;
    // cout << y1.sign <<endl;
    // cout << y1.exp <<endl;
    // cout << y1.sig <<endl;

    // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // cout << "Value of y2: "  <<endl;//<< rePackForTesting(t_share1) <<endl;
    // cout << y2.sign <<endl;
    // cout << y2.exp <<endl;
    // cout << y2.sig <<endl;

    // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // cout << "Value of y3: "  <<endl;//<< rePackForTesting(t_share1) <<endl;
    // cout << y3.sign <<endl;
    // cout << y3.exp <<endl;
    // cout << y3.sig <<endl;

    // cout << "-------------------------------------------------------------------------------------------------------------------------------" <<endl;


    int packingProcess = -1;
    
    // cout << "Adding x1 and y1" <<endl;

    doubleNumber partyOneShare1;
    double64_t t_partyOneShare1 = f64_add(x1.sign, x1.exp, x1.sig, x2.sign, x2.exp, x2.sig, partyOneShare1.sign, partyOneShare1.exp, partyOneShare1.sig, packingProcess);

    // cout << "Expected:  " << rePackForTesting(x1) + rePackForTesting(x2) <<endl;
    // cout << "Output  :  " << rePackForTesting(partyOneShare1) <<endl;


    // cout << "Adding x2 and y2" <<endl;

    doubleNumber partyOneShare2;
    double64_t t_partyOneShare2 = f64_add(x3.sign, x3.exp, x3.sig, partyOneShare1.sign, partyOneShare1.exp, partyOneShare1.sig, partyOneShare2.sign, partyOneShare2.exp, partyOneShare2.sig, packingProcess);

    // cout << "Expected:  " << rePackForTesting(x1) + rePackForTesting(x2) + rePackForTesting(x3) <<endl;
    // cout << "Output:    " << rePackForTesting(partyOneShare2) <<endl;

    // cout << "Output: " << *(double*) &t_partyOneShare2 <<endl;
    // cout << "Sign: " <<partyOneShare2.sign <<endl;
    // cout << "Exp: " << partyOneShare2.exp<<endl;
    // cout << "Sig: " <<partyOneShare2.sig <<endl;

    // cout << "Adding x3 and y3" <<endl;

    doubleNumber partyOneShare3;
    double64_t t_partyOneShare3 = f64_add(y1.sign, y1.exp, y1.sig, partyOneShare2.sign, partyOneShare2.exp, partyOneShare2.sig, partyOneShare3.sign, partyOneShare3.exp, partyOneShare3.sig, packingProcess);

    //     cout << "Expected:  " << rePackForTesting(x1) + rePackForTesting(x2) + rePackForTesting(x3)  + rePackForTesting(y1) <<endl;
    // cout << "Output:    " << rePackForTesting(partyOneShare3) <<endl;

    // cout << "Output: " << *(double*) &t_partyOneShare1 <<endl;
    // cout << "Sign: " <<partyOneShare3.sign <<endl;
    // cout << "Exp: " << partyOneShare3.exp<<endl;
    // cout << "Sig: " <<partyOneShare3.sig <<endl;


    // cout << "Adding Party one and party two shares " <<endl;


    doubleNumber temp_add_1;
    double64_t t_temp_add_1 = f64_add(y2.sign, y2.exp, y2.sig, partyOneShare3.sign, partyOneShare3.exp, partyOneShare3.sig, temp_add_1.sign, temp_add_1.exp, temp_add_1.sig, packingProcess);

    // cout << "Expected:  " << rePackForTesting(x1) + rePackForTesting(x2) + rePackForTesting(x3)  + rePackForTesting(y1) + rePackForTesting(y2) <<endl;
    // cout << "Output:    " << rePackForTesting(temp_add_1) <<endl;

    // cout << "Output: " << *(double*) &t_temp_add_1 <<endl;
    // cout << "Sign: " <<temp_add_1.sign <<endl;
    // cout << "Exp: " << temp_add_1.exp<<endl;
    // cout << "Sig: " <<temp_add_1.sig <<endl;


    // cout << "Adding  party three shares " <<endl;


    double64_t t_temp_add = f64_add(temp_add_1.sign, temp_add_1.exp, temp_add_1.sig, y3.sign, y3.exp, y3.sig, out_totalSum.sign, out_totalSum.exp, out_totalSum.sig, packingProcess);


    // cout << "Expected     : " << rePackForTesting(x1) + rePackForTesting(x2) + rePackForTesting(x3)  + rePackForTesting(y1) + rePackForTesting(y2) + rePackForTesting(y3) <<endl;

    // cout << "output       : " << rePackForTesting(out_totalSum) <<endl;


    // cout<< "Total Expected: " <<  (rePackForTesting(x1) + rePackForTesting(y1)) + (rePackForTesting(x2) + rePackForTesting(y2)) + (rePackForTesting(x3) + rePackForTesting(y3))  <<endl;

    // cout<< "Total Expected: " <<  (rePackForTesting(x1) + rePackForTesting(y1)) + (rePackForTesting(x2) + rePackForTesting(y2)) + (rePackForTesting(x3) + rePackForTesting(y3))  <<endl;

    // cout<< "Total Expected: " <<  rePackForTesting(x1) + rePackForTesting(x2) + rePackForTesting(x3) + rePackForTesting(y1) + rePackForTesting(y2) + rePackForTesting(y3)  <<endl;

    // while(1);

    // cout << "Output: " << *(double*) &t_temp_add <<endl;
    // cout << "Sign: " <<out_totalSum.sign <<endl;
    // cout << "Exp: " << out_totalSum.exp<<endl;
    // cout << "Sig: " <<out_totalSum.sig <<endl;

    return t_temp_add;

}

vector<doubleNumber> MultiPartyAdditionProtocolForFinalComputation(doubleNumber x1, doubleNumber y1, doubleNumber x2, doubleNumber y2, doubleNumber x3, doubleNumber y3, double &out_totalSum)
{

    int packingProcess = -1;
    

    doubleNumber partyOneShare1;
    double64_t t_partyOneShare1 = f64_add(x1.sign, x1.exp, x1.sig, y1.sign, y1.exp, y1.sig, partyOneShare1.sign, partyOneShare1.exp, partyOneShare1.sig, packingProcess);

    doubleNumber partyOneShare2;
    double64_t t_partyOneShare2 = f64_add(x2.sign, x2.exp, x2.sig, y2.sign, y2.exp, y2.sig, partyOneShare2.sign, partyOneShare2.exp, partyOneShare2.sig, packingProcess);

    doubleNumber partyOneShare3;
    double64_t t_partyOneShare3 = f64_add(x3.sign, x3.exp, x3.sig, y3.sign, y3.exp, y3.sig, partyOneShare3.sign, partyOneShare3.exp, partyOneShare3.sig, packingProcess);



    vector<doubleNumber> finalShares;
    finalShares.clear();

    finalShares.push_back(partyOneShare1);

    finalShares.push_back(partyOneShare2);

    finalShares.push_back(partyOneShare3);


    out_totalSum = rePackForTesting(partyOneShare1) + rePackForTesting(partyOneShare2) + rePackForTesting(partyOneShare3);

    return finalShares;

}

double64_t CrossMultiplicationUsingDuatallahProtocol(bool signA, int_fast16_t expA, uint_fast64_t sigA,  bool signB, int_fast16_t expB, uint_fast64_t sigB, bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ, doubleNumber &Share1, doubleNumber &Share2, doubleNumber &Share3)
{

    // double alpha1,alpha2;
    // alpha1 = (double)(rand()) / (double)(rand()) ; //(rand() % 10) + 1;
    // alpha2 = (double)(rand()) / (double)(rand()) ;

    // alpha1 = 2.04999713368002112190424668369814753532409667968750;
    // alpha2 = 0.36462958398842182372590059458161704242229461669922;


    double X1 = rePackForTesting(signA, expA, sigA);

    // cout << "X1: " << X1 <<endl;
    // cout << signA << "  " <<expA << "   " << sigA <<endl;

    double sum1 = X1 + alpha1;

    // cout << "sum1: " << sum1 <<endl;

    double X2 = rePackForTesting(signB, expB, sigB);

    // cout << "X2: " << X2 <<endl;
    // cout << signB << "  " <<expB << "   " << sigB <<endl;

    double sum2 = X2 + alpha2;

    // cout << "sum2: " << sum2 <<endl;

     // cout << "Original Expected Output: " << X1 * X2 <<endl;


     // cout << "------------------------------------------------------------------------------------------------------------" <<endl;

    double64_t alp1, alp2;
    alp1 = *(double64_t*) &alpha1;
    alp2 = *(double64_t*) &alpha2;

    union ui64_f64 uAlpha1;
    uint_fast64_t uiAlpha1;
    bool signAlpha1;
    int_fast16_t expAlpha1;
    uint_fast64_t sigAlpha1;
    union ui64_f64 uAlpha2;
    uint_fast64_t uiAlpha2;
    bool signAlpha2;
    int_fast16_t expAlpha2;
    uint_fast64_t sigAlpha2;

    uAlpha1.f = alp1;
    uiAlpha1 = uAlpha1.ui;
    signAlpha1 = signF64UI( uiAlpha1 );
    expAlpha1  = expF64UI( uiAlpha1 );
    sigAlpha1  = fracF64UI( uiAlpha1 );

    uAlpha2.f = alp2;
    uiAlpha2 = uAlpha2.ui;
    signAlpha2 = signF64UI( uiAlpha2 );
    expAlpha2  = expF64UI( uiAlpha2 );
    sigAlpha2  = fracF64UI( uiAlpha2 );

    bool sign_Alpha1_Alpha2;
    int_fast16_t expA_Alpha1_Alpha2;
    uint_fast64_t sigA_Alpha1_Alpha2;


    // cout << "alpha_1: " << alpha1 <<endl;
    
    // cout << signAlpha1 <<endl;
    // cout << expAlpha1 <<endl;
    // cout << sigAlpha1 <<endl;


    // cout << "alpha_2: " << alpha2 <<endl;

    // cout << signAlpha2 <<endl;
    // cout << expAlpha2 <<endl;
    // cout << sigAlpha2 <<endl;

    // cout << "------------------------------------------------------------------------------------------------------------" <<endl;
    // cout <<endl;

    // cout << "Input1: " << alpha1 <<endl;
    // cout << signAlpha1 << "  " <<expAlpha1 << "   " << sigAlpha1 <<endl;

    // cout << "Input2: " << alpha2 <<endl;
    // cout << signAlpha2 << "  " <<expAlpha2 << "   " << sigAlpha2 <<endl;


    double64_t t_Alpha1_Alpha2 = f64_mul(signAlpha1, expAlpha1, sigAlpha1, signAlpha2, expAlpha2, sigAlpha2, sign_Alpha1_Alpha2, expA_Alpha1_Alpha2, sigA_Alpha1_Alpha2);

    double output_Alpha1_Alpha2 = rePackForTesting(sign_Alpha1_Alpha2, expA_Alpha1_Alpha2, sigA_Alpha1_Alpha2); 


    double output_Alpha1_Alpha2_Expected = alpha1 * alpha2;

    doubleNumber t_output_Alpha1_Alpha2_Expected = getSignExpSig(output_Alpha1_Alpha2_Expected);

    // cout << "333333333333333333333333333333333333333333333333333333: Computed Output: " << output_Alpha1_Alpha2 << endl;
    // cout << sign_Alpha1_Alpha2 << endl;
    // cout << expA_Alpha1_Alpha2 << endl;
    // cout << sigA_Alpha1_Alpha2 << endl;
    // cout << "333333333333333333333333333333333333333333333333333333: Expected Output: " << output_Alpha1_Alpha2_Expected << endl;
    // cout << t_output_Alpha1_Alpha2_Expected.sign << endl;
    // cout << t_output_Alpha1_Alpha2_Expected.exp << endl;
    // cout << t_output_Alpha1_Alpha2_Expected.sig << endl;

    // cout << "------------------------------------------------------------------------------------------------------------" <<endl;

    // cout <<endl;

    //------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // double t_partyOne_X1_Alpha1 = t_positRepresentationOfAlpha1 + in_firstPositRepresentation;

    bool sign_partyOne_X1_Alpha1;
    int_fast16_t expA_partyOne_X1_Alpha1;
    uint_fast64_t sigA_partyOne_X1_Alpha1;

    int t_packingProcess = -1;


    // cout << "Input1: " << alpha1 <<endl;
    // cout << signAlpha1 << "  " <<expAlpha1 << "   " << sigAlpha1 <<endl;

    // cout << "Input2: " << X1 <<endl;
    // cout << signA << "  " <<expA << "   " << sigA <<endl;

    

    double64_t t_partyOne_X1_Alpha1 = f64_add(signAlpha1, expAlpha1, sigAlpha1, signA, expA, sigA, sign_partyOne_X1_Alpha1, expA_partyOne_X1_Alpha1, sigA_partyOne_X1_Alpha1, t_packingProcess);


    // double temp1 = (X1 + alpha1);

    // doubleNumber t_temp1 = getSignExpSig(temp1);

    // cout << "(X1 + alpha1)" <<endl;
    // cout << "Computed: " << rePackForTesting(sign_partyOne_X1_Alpha1, expA_partyOne_X1_Alpha1, sigA_partyOne_X1_Alpha1) <<endl; 

    // cout << sign_partyOne_X1_Alpha1 << endl;
    // cout << expA_partyOne_X1_Alpha1 << endl;
    // cout << sigA_partyOne_X1_Alpha1 << endl;

    // cout << "Expected: " << temp1 << endl;
    // cout << t_temp1.sign << endl;
    // cout << t_temp1.exp << endl;
    // cout << t_temp1.sig << endl;
   

    // cout << "------------------------------------------------------------------------------------------------------------" <<endl;

    // cout <<endl;
    //------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // double t_partyTwo_X2_Alpha2 = in_SecondPositRepresentation + t_positRepresentationOfAlpha2;


    bool sign_partyOne_X2_Alpha2;
    int_fast16_t expA_partyOne_X2_Alpha2;
    uint_fast64_t sigA_partyOne_X2_Alpha2;
    int t_packingProcess1 = -1;

    // cout << "Input1: " << alpha2 <<endl;
    // cout << signAlpha2 << "  " <<expAlpha2 << "   " << sigAlpha2 <<endl;

    // cout << "Input2: " << X2 <<endl;
    // cout << signB << "  " <<expB << "   " << sigB <<endl;



    double64_t t_partyOne_X2_Alpha2 = f64_add(signAlpha2, expAlpha2, sigAlpha2, signB, expB, sigB, sign_partyOne_X2_Alpha2, expA_partyOne_X2_Alpha2, sigA_partyOne_X2_Alpha2, t_packingProcess1);

    // double temp2 = (X2 + alpha2);
    // doubleNumber t_temp2 = getSignExpSig(temp2);

    // cout << "(X2 + alpha2)" <<endl;
    // cout << "Computed: " << rePackForTesting(sign_partyOne_X2_Alpha2, expA_partyOne_X2_Alpha2, sigA_partyOne_X2_Alpha2) <<endl; 
    // cout << sign_partyOne_X2_Alpha2 << endl;
    // cout << expA_partyOne_X2_Alpha2 << endl;
    // cout << sigA_partyOne_X2_Alpha2 << endl;

    // cout << "Expected: " << temp2 << endl;
    // cout << t_temp2.sign << endl;
    // cout << t_temp2.exp << endl;
    // cout << t_temp2.sig << endl;


    // cout << "------------------------------------------------------------------------------------------------------------" <<endl;

    // cout <<endl;
    //------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Share computation of party1

    // double t_partyOne_X1_X2Alpha2 = t_partyTwo_X2_Alpha2 * in_firstPositRepresentation;

    bool sign_partyOne_X1_X2Alpha2;
    int_fast16_t expA_partyOne_X1_X2Alpha2;
    uint_fast64_t sigA_partyOne_X1_X2Alpha2;


    // cout << "Input1: " << temp2 <<endl;
    // cout << sign_partyOne_X2_Alpha2 << "  " <<expA_partyOne_X2_Alpha2 << "   " << sigA_partyOne_X2_Alpha2 <<endl;

    // cout << "Input2: " << X1 <<endl;
    // cout << signA << "  " <<expA << "   " << sigA <<endl;




    double64_t t_partyOne_X1_X2Alpha2 = f64_mul( signA, expA, sigA, sign_partyOne_X2_Alpha2, expA_partyOne_X2_Alpha2, sigA_partyOne_X2_Alpha2, sign_partyOne_X1_X2Alpha2, expA_partyOne_X1_X2Alpha2, sigA_partyOne_X1_X2Alpha2);

    // cout << "X1 * (X2 + alpha2)" <<endl;


    // double temp3 = X1 * (X2 + alpha2);
    // doubleNumber t_temp3 = getSignExpSig(temp3);


    // cout << "Computed: " << rePackForTesting(sign_partyOne_X1_X2Alpha2, expA_partyOne_X1_X2Alpha2, sigA_partyOne_X1_X2Alpha2) <<endl; 
    // cout << sign_partyOne_X1_X2Alpha2 << endl;
    // cout << expA_partyOne_X1_X2Alpha2 << endl;
    // cout << sigA_partyOne_X1_X2Alpha2 << endl;

    // cout << "Expected: " << temp3 << endl;
    // cout << t_temp3.sign << endl;
    // cout << t_temp3.exp << endl;
    // cout << t_temp3.sig << endl;

    // cout << "------------------------------------------------------------------------------------------------------------" <<endl;

    // cout <<endl;
    // double t_partyOne_X1Alpha1_X2Alpha2 = t_partyTwo_X2_Alpha2 * t_partyOne_X1_Alpha1; t_partyOne_X2_Alpha2 t_partyOne_X1_Alpha1

    bool sign_partyOne_X1Alpha1_X2Alpha2;
    int_fast16_t expA_partyOne_X1Alpha1_X2Alpha2;
    uint_fast64_t sigA_partyOne_X1Alpha1_X2Alpha2;

    // cout << "Input1: " << temp2 <<endl;
    // cout << sign_partyOne_X2_Alpha2 << "  " <<expA_partyOne_X2_Alpha2 << "   " << sigA_partyOne_X2_Alpha2 <<endl;

    // cout << "Input2: " << temp1 <<endl;
    // cout << sign_partyOne_X1_Alpha1 << "  " <<expA_partyOne_X1_Alpha1 << "   " << sigA_partyOne_X1_Alpha1 <<endl;


    double64_t t_partyOne_X1Alpha1_X2Alpha2 = f64_mul(sign_partyOne_X2_Alpha2, expA_partyOne_X2_Alpha2, sigA_partyOne_X2_Alpha2, sign_partyOne_X1_Alpha1, expA_partyOne_X1_Alpha1, sigA_partyOne_X1_Alpha1, sign_partyOne_X1Alpha1_X2Alpha2, expA_partyOne_X1Alpha1_X2Alpha2, sigA_partyOne_X1Alpha1_X2Alpha2);


    // cout << "(X1 + alpha1) * (X2 + alpha2)" <<endl;

    // double temp4 = (X1 + alpha1) * (X2 + alpha2);
    // doubleNumber t_temp4 = getSignExpSig(temp4);


    // cout << "Computed: " << rePackForTesting(sign_partyOne_X1Alpha1_X2Alpha2, expA_partyOne_X1Alpha1_X2Alpha2, sigA_partyOne_X1Alpha1_X2Alpha2) <<endl; 
    // cout << sign_partyOne_X1Alpha1_X2Alpha2 << endl;
    // cout << expA_partyOne_X1Alpha1_X2Alpha2 << endl;
    // cout << sigA_partyOne_X1Alpha1_X2Alpha2 << endl;

    // cout << "Expected: " << temp4 << endl;
    // cout << t_temp4.sign << endl;
    // cout << t_temp4.exp << endl;
    // cout << t_temp4.sig << endl;

    // cout << "------------------------------------------------------------------------------------------------------------" <<endl;


    // cout <<endl;

    // double t_partyOneShare = t_partyOne_X1_X2Alpha2 - t_partyOne_X1Alpha1_X2Alpha2; 309.429748535156250 - 431.697723388671875

    bool sign_partyOneShare;
    int_fast16_t expA_partyOneShare;
    uint_fast64_t sigA_partyOneShare;


    // cout << "Input1: " << temp3 <<endl;
    // cout << sign_partyOne_X1_X2Alpha2 << "  " <<expA_partyOne_X1_X2Alpha2 << "   " << sigA_partyOne_X1_X2Alpha2 <<endl;

    // cout << "Input2: " << temp4 <<endl;
    // cout << sign_partyOne_X1Alpha1_X2Alpha2 << "  " <<expA_partyOne_X1Alpha1_X2Alpha2 << "   " << sigA_partyOne_X1Alpha1_X2Alpha2 <<endl;


    int t_packingProcess2 = -1;

    sign_partyOne_X1Alpha1_X2Alpha2 = sign_partyOne_X1Alpha1_X2Alpha2 ^ 1;

    double64_t t_partyOneShare =  f64_add(sign_partyOne_X1_X2Alpha2, expA_partyOne_X1_X2Alpha2, sigA_partyOne_X1_X2Alpha2, sign_partyOne_X1Alpha1_X2Alpha2, expA_partyOne_X1Alpha1_X2Alpha2, sigA_partyOne_X1Alpha1_X2Alpha2 , sign_partyOneShare, expA_partyOneShare,  sigA_partyOneShare, t_packingProcess2);


    // cout << "11111111111111111111111111111111111111111111111: -((X1 + alpha1) * (X2 + alpha2)) + (X1*(X2+alpha2))" <<endl; 

    // double temp5 = -((X1 + alpha1) * (X2 + alpha2)) + (X1*(X2+alpha2));
    // doubleNumber t_temp5 = getSignExpSig(temp5);

    // cout << "Computed: " << rePackForTesting(sign_partyOneShare, expA_partyOneShare,  sigA_partyOneShare)  <<endl; 
    // cout << sign_partyOneShare << endl;
    // cout << expA_partyOneShare << endl;
    // cout << sigA_partyOneShare << endl;

    // cout << "Expected: " << temp5 << endl;
    // cout << t_temp5.sign << endl;
    // cout << t_temp5.exp << endl;
    // cout << t_temp5.sig << endl;

    // cout << "------------------------------------------------------------------------------------------------------------" <<endl;



    // cout <<endl;

    //------------------------------------------------------------------------------------------------------------------------------------------------------------------
  
    // double t_partyTwoShare = in_SecondPositRepresentation * t_partyOne_X1_Alpha1;

    bool sign_partyTwoShare;
    int_fast16_t expA_partyTwoShare;
    uint_fast64_t sigA_partyTwoShare;

    // cout << "Input1: " << X2 <<endl;
    // cout << signB << "  " <<expB << "   " << sigB <<endl;

    // cout << "Input2: " << temp1 <<endl;
    // cout << sign_partyOne_X1_Alpha1 << "  " <<expA_partyOne_X1_Alpha1 << "   " << sigA_partyOne_X1_Alpha1 <<endl;



    double64_t t_partyTwoShare = f64_mul(signB, expB, sigB,  sign_partyOne_X1_Alpha1, expA_partyOne_X1_Alpha1, sigA_partyOne_X1_Alpha1, sign_partyTwoShare, expA_partyTwoShare, sigA_partyTwoShare);

    // cout << "2222222222222222222222222222222222222222222222: X2*(X1+alpha1)" <<endl;

    // double temp6 = X2*(X1+alpha1);
    // doubleNumber t_temp6 = getSignExpSig(temp6);

    // cout << "Computed: " << rePackForTesting(sign_partyTwoShare, expA_partyTwoShare,  sigA_partyTwoShare)  <<endl;
    // cout << sign_partyTwoShare << endl;
    // cout << expA_partyTwoShare << endl;
    // cout << sigA_partyTwoShare << endl;

    // cout << "Expected: " << temp6 << endl;
    // cout << t_temp6.sign << endl;
    // cout << t_temp6.exp << endl;
    // cout << t_temp6.sig << endl;

    // cout << "---------------------------------------------- Checking Total Sum of Shares--------------------------------------------------------------" <<endl;


    // cout <<endl;
    //------------------------------------------------------------------------------------------------------------------------------------------------------------------

    bool sign_tempOutput1;
    int_fast16_t expA_tempOutput1;
    uint_fast64_t sigA_tempOutput1;
    // int_fast16_t expA_tempOutput1_1;
    // uint_fast32_t sigA_tempOutput1_1;
    int t_packingProcess3 = -1;

    union ui64_f64 uA1;
    uint_fast64_t uiA1;
    bool signA1;
    int_fast16_t expA1;
    uint_fast64_t sigA1;


    // cout << "Input1: " << temp5 <<endl;
    // cout << sign_partyOneShare << "  " <<expA_partyOneShare << "   " << sigA_partyOneShare <<endl;

    // cout << "Input2: " << output_Alpha1_Alpha2 <<endl;
    // cout << sign_Alpha1_Alpha2 << "  " <<expA_Alpha1_Alpha2 << "   " << sigA_Alpha1_Alpha2 <<endl;


   
    double64_t temp_output1 = f64_add(sign_partyOneShare, expA_partyOneShare,  sigA_partyOneShare, sign_Alpha1_Alpha2, expA_Alpha1_Alpha2,  sigA_Alpha1_Alpha2, sign_tempOutput1, expA_tempOutput1, sigA_tempOutput1, t_packingProcess3);

    // double temp7 = output_Alpha1_Alpha2_Expected + temp5;
    // doubleNumber t_temp7 = getSignExpSig(temp7);

    // cout << "Computed1: " << rePackForTesting(sign_tempOutput1, expA_tempOutput1, sigA_tempOutput1)  <<endl; 
    // cout << sign_tempOutput1 << endl;
    // cout << expA_tempOutput1 << endl;
    // cout << sigA_tempOutput1 << endl;

    // cout << "Expected1: " << temp7 << endl;
    // cout << t_temp7.sign << endl;
    // cout << t_temp7.exp << endl;
    // cout << t_temp7.sig << endl;

    // cout << "------------------------------------------------------------------------------------------------------------" <<endl;


    // cout <<endl;

    bool sign_tempOutput2;
    int_fast16_t expA_tempOutput2;
    uint_fast64_t sigA_tempOutput2;
    // int_fast16_t expA_tempOutput2_1;
    // uint_fast64_t sigA_tempOutput2_1;
    int t_packingProcess4 = -1;

    // cout << "Input1: " << temp7 <<endl;
    // cout << sign_tempOutput1 << "  " <<expA_tempOutput1 << "   " << sigA_tempOutput1 <<endl;

    // cout << "Input2: " << temp6 <<endl;
    // cout << sign_partyTwoShare << "  " <<expA_partyTwoShare << "   " << sigA_partyTwoShare <<endl;


    double64_t temp_output2 = f64_add(sign_tempOutput1, expA_tempOutput1, sigA_tempOutput1, sign_partyTwoShare, expA_partyTwoShare,  sigA_partyTwoShare, sign_tempOutput2, expA_tempOutput2, sigA_tempOutput2, t_packingProcess4);


    // double output_Expected =  temp7 + temp6;

    // doubleNumber t_output_Expected = getSignExpSig(output_Expected);

    // cout << "Computed2: " << rePackForTesting(sign_tempOutput2, expA_tempOutput2, sigA_tempOutput2)  <<endl; 
    // cout << sign_tempOutput2 << endl;
    // cout << expA_tempOutput2 << endl;
    // cout << sigA_tempOutput2 << endl;

    // cout << "Expected2: " << output_Expected << endl;
    // cout << t_output_Expected.sign << endl;
    // cout << t_output_Expected.exp << endl;
    // cout << t_output_Expected.sig << endl;

    // cout << "------------------------------------------------------------------------------------------------------------" <<endl;


    // cout <<endl;


    double originalSumExpected = X1 * X2 ;

    doubleNumber t_originalSumExpected = getSignExpSig(originalSumExpected);

    // cout << "Expected: " << originalSumExpected << endl;
    // cout << t_originalSumExpected.sign << endl;
    // cout << t_originalSumExpected.exp << endl;
    // cout << t_originalSumExpected.sig << endl;


   

    double output = *(double*) &temp_output2; 

    
    // cout << "Share1" <<endl;

    // cout <<endl;

    // cout << sign_partyOneShare <<endl;
    // cout << expA_partyOneShare <<endl;
    // cout << sigA_partyOneShare <<endl;

    // cout << "Share2" <<endl;

    // cout <<endl;

    // cout << sign_partyTwoShare <<endl;
    // cout << expA_partyTwoShare <<endl;
    // cout << sigA_partyTwoShare <<endl;

    // cout << "Share3" <<endl;

    // cout <<endl;

    // cout << sign_Alpha1_Alpha2 <<endl;
    // cout << expA_Alpha1_Alpha2 <<endl;
    // cout << sigA_Alpha1_Alpha2 <<endl;



    signZ = sign_tempOutput2;
    expZ = expA_tempOutput2;
    sigZ = sigA_tempOutput2;

     //------------------------------------------------------------------------------------------------------------------------------------------------------------------

    Share2.sign = sign_partyOneShare;
    Share2.exp  = expA_partyOneShare;
    Share2.sig  = sigA_partyOneShare;

    Share3.sign = sign_partyTwoShare;
    Share3.exp  = expA_partyTwoShare;
    Share3.sig  = sigA_partyTwoShare;

    Share1.sign = sign_Alpha1_Alpha2;
    Share1.exp  = expA_Alpha1_Alpha2;
    Share1.sig  = sigA_Alpha1_Alpha2;

    // cout << "------------------------------------------------------------------------------------------------------------" <<endl;

    // cout << "Share1: " << rePackForTesting(Share1) <<endl;

    // cout << "Share2: " << rePackForTesting(Share2) <<endl;

    // cout << "Share3: " << rePackForTesting(Share3) <<endl;

    // while(1);


    return temp_output2;

}

vector<doubleNumber> DuatallahMultiplicationProtocol(doubleNumber x1, doubleNumber x2, doubleNumber x3, doubleNumber y1, doubleNumber y2, doubleNumber y3)
{
    int packingProcess = 1;

    // cout << "------------------------------------------------------------------------- DuatallahMultiplicationProtocol -----------------------------------------------------------------------------" <<endl;

    // local computation

    // cout << "Inputs1" << endl;

    // cout << x1.sign <<endl;
    // cout << x1.exp <<endl;
    // cout << x1.sig <<endl;

    // cout << "Inputs2" << endl;

    // cout << y1.sign <<endl;
    // cout << y1.exp <<endl;
    // cout << y1.sig <<endl;

    doubleNumber x1y1;
    double64_t t_x1y1 = f64_mul(x1.sign , x1.exp, x1.sig, y1.sign, y1.exp, y1.sig, x1y1.sign, x1y1.exp, x1y1.sig);

    // cout << "Expected (x1y1): " << rePackForTesting(x1) * rePackForTesting(y1) <<endl;
    // cout << "Computed (x1y1): " << rePackForTesting(x1y1) <<endl;


    // cout << "----------------------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // cout << "Inputs1" << endl;

    // cout << x2.sign <<endl;
    // cout << x2.exp <<endl;
    // cout << x2.sig <<endl;

    // cout << "Inputs2" << endl;

    // cout << y2.sign <<endl;
    // cout << y2.exp <<endl;
    // cout << y2.sig <<endl;

    doubleNumber x2y2;
    double64_t t_x2y2 = f64_mul(x2.sign , x2.exp, x2.sig, y2.sign, y2.exp, y2.sig, x2y2.sign, x2y2.exp, x2y2.sig);

    // cout << "Expected (x2y2): " << rePackForTesting(x2) * rePackForTesting(y2) <<endl;
    // cout << "Computed (x2y2): " << rePackForTesting(x2y2) <<endl;

    //  cout << "----------------------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // cout << "Inputs1" << endl;

    // cout << x3.sign <<endl;
    // cout << x3.exp <<endl;
    // cout << x3.sig <<endl;

    // cout << "Inputs2" << endl;

    // cout << y3.sign <<endl;
    // cout << y3.exp <<endl;
    // cout << y3.sig <<endl;

    doubleNumber x3y3;
    double64_t t_x3y3 = f64_mul(x3.sign , x3.exp, x3.sig, y3.sign, y3.exp, y3.sig, x3y3.sign, x3y3.exp, x3y3.sig);

    // cout << "Expected (x3y3): " << rePackForTesting(x3) * rePackForTesting(y3) <<endl;
    // cout << "Computed (x3y3): " << rePackForTesting(x3y3) <<endl;

    //  cout << "----------------------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // // cross share multiplication

    // cout << "Inputs1" << endl;

    // cout << x1.sign <<endl;
    // cout << x1.exp <<endl;
    // cout << x1.sig <<endl;

    // cout << "Inputs2" << endl;

    // cout << y2.sign <<endl;
    // cout << y2.exp <<endl;
    // cout << y2.sig <<endl;

    doubleNumber x1y2, x1y2_Share1, x1y2_Share2, x1y2_Share3;
    double64_t t_x1y2 = CrossMultiplicationUsingDuatallahProtocol(x1.sign , x1.exp, x1.sig, y2.sign, y2.exp, y2.sig, x1y2.sign, x1y2.exp, x1y2.sig, x1y2_Share1, x1y2_Share2, x1y2_Share3);

    // cout << "Expected   (x1y2)              : " << rePackForTesting(x1) * rePackForTesting(y2) <<endl;
    // cout << "Computed   (x1y2)              : " << rePackForTesting(x1y2) <<endl;
    // cout << "Computed with Share sums (x1y2): " << rePackForTesting(x1y2_Share1) + rePackForTesting(x1y2_Share2) + rePackForTesting(x1y2_Share3) <<endl;


    //  cout << "----------------------------------------------------------------------------------------------------------------------------------------------" <<endl;


    // cout << "Inputs1" << endl;

    // cout << x1.sign <<endl;
    // cout << x1.exp <<endl;
    // cout << x1.sig <<endl;

    // cout << "Inputs2" << endl;

    // cout << y3.sign <<endl;
    // cout << y3.exp <<endl;
    // cout << y3.sig <<endl;


    doubleNumber x1y3, x1y3_Share1, x1y3_Share2, x1y3_Share3;
    double64_t t_x1y3 = CrossMultiplicationUsingDuatallahProtocol(x1.sign , x1.exp, x1.sig, y3.sign, y3.exp, y3.sig, x1y3.sign, x1y3.exp, x1y3.sig, x1y3_Share1, x1y3_Share2, x1y3_Share3);

    // cout << "Expected (x1y3)                : " << rePackForTesting(x1) * rePackForTesting(y3) <<endl;
    // cout << "Computed (x1y3)                : " << rePackForTesting(x1y3) <<endl;
    // cout << "Computed with Share sums (x1y3): " << rePackForTesting(x1y3_Share1) + rePackForTesting(x1y3_Share2) + rePackForTesting(x1y3_Share3) <<endl;


    //  cout << "----------------------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // cout << "Inputs1" << endl;

    // cout << x2.sign <<endl;
    // cout << x2.exp <<endl;
    // cout << x2.sig <<endl;

    // cout << "Inputs2" << endl;

    // cout << y1.sign <<endl;
    // cout << y1.exp <<endl;
    // cout << y1.sig <<endl;


    doubleNumber x2y1, x2y1_Share1, x2y1_Share2, x2y1_Share3;
    double64_t t_x2y1 = CrossMultiplicationUsingDuatallahProtocol(x2.sign , x2.exp, x2.sig, y1.sign, y1.exp, y1.sig, x2y1.sign, x2y1.exp, x2y1.sig, x2y1_Share1, x2y1_Share2, x2y1_Share3);

    // cout << "Expected (x2y1)               : " << rePackForTesting(x2) * rePackForTesting(y1) <<endl;
    // cout << "Computed (x2y1)               : " << rePackForTesting(x2y1) <<endl;
    // cout << "Computed with Share sums(x2y1): " << rePackForTesting(x2y1_Share1) + rePackForTesting(x2y1_Share2) + rePackForTesting(x2y1_Share3) <<endl;


    // cout << "----------------------------------------------------------------------------------------------------------------------------------------------" <<endl;


    // cout << "Inputs1" << endl;

    // cout << x2.sign <<endl;
    // cout << x2.exp <<endl;
    // cout << x2.sig <<endl;

    // cout << "Inputs2" << endl;

    // cout << y3.sign <<endl;
    // cout << y3.exp <<endl;
    // cout << y3.sig <<endl;

    doubleNumber x2y3, x2y3_Share1, x2y3_Share2, x2y3_Share3;
    double64_t t_x2y3 = CrossMultiplicationUsingDuatallahProtocol(x2.sign , x2.exp, x2.sig, y3.sign, y3.exp, y3.sig, x2y3.sign, x2y3.exp, x2y3.sig, x2y3_Share1, x2y3_Share2, x2y3_Share3);

    // cout << "Expected  (x2y3)              : " << rePackForTesting(x2) * rePackForTesting(y3) <<endl;
    // cout << "Computed  (x2y3)              : " << rePackForTesting(x2y3) <<endl;
    // cout << "Computed with Share sums(x2y3): " << rePackForTesting(x2y3_Share1) + rePackForTesting(x2y3_Share2) + rePackForTesting(x2y3_Share3) <<endl;


    //  cout << "----------------------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // cout << "Inputs1" << endl;

    // cout << x3.sign <<endl;
    // cout << x3.exp <<endl;
    // cout << x3.sig <<endl;

    // cout << "Inputs2" << endl;

    // cout << y1.sign <<endl;
    // cout << y1.exp <<endl;
    // cout << y1.sig <<endl;


    doubleNumber x3y1, x3y1_Share1, x3y1_Share2, x3y1_Share3;
    double64_t t_x3y1 = CrossMultiplicationUsingDuatallahProtocol(x3.sign , x3.exp, x3.sig, y1.sign, y1.exp, y1.sig, x3y1.sign, x3y1.exp, x3y1.sig, x3y1_Share1, x3y1_Share2, x3y1_Share3);

    // cout << "Expected  (x3y1)              : " << rePackForTesting(x3) * rePackForTesting(y1) <<endl;
    // cout << "Computed  (x3y1)              : " << rePackForTesting(x3y1) <<endl;
    // cout << "Computed with Share sums(x3y1): " << rePackForTesting(x3y1_Share1) + rePackForTesting(x3y1_Share2) + rePackForTesting(x3y1_Share3) <<endl;

    //  cout << "----------------------------------------------------------------------------------------------------------------------------------------------" <<endl;


    // cout << "Inputs1" << endl;

    // cout << x3.sign <<endl;
    // cout << x3.exp <<endl;
    // cout << x3.sig <<endl;

    // cout << "Inputs2" << endl;

    // cout << y2.sign <<endl;
    // cout << y2.exp <<endl;
    // cout << y2.sig <<endl;


    doubleNumber x3y2, x3y2_Share1, x3y2_Share2, x3y2_Share3;
    double64_t t_x3y2 = CrossMultiplicationUsingDuatallahProtocol(x3.sign , x3.exp, x3.sig, y2.sign, y2.exp, y2.sig, x3y2.sign, x3y2.exp, x3y2.sig, x3y2_Share1, x3y2_Share2, x3y2_Share3);

    // cout << "Expected (x3y2)               : " << rePackForTesting(x3) * rePackForTesting(y2) <<endl;
    // cout << "Computed (x3y2)               : " << rePackForTesting(x3y2) <<endl;
    // cout << "Computed with Share sums(x3y2): " << rePackForTesting(x3y2_Share1) + rePackForTesting(x3y2_Share2) + rePackForTesting(x3y2_Share3) <<endl;

    // cout << "----------------------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // while(1);


    // Share Creation for Each Party

    doubleNumber PartyOneShare_1;
    double64_t t_PartyOneShare_1 = MultiPartyAdditionProtocol(x1y2_Share1, x1y3_Share1, x2y1_Share1, x2y3_Share1, x3y1_Share1, x3y2_Share1, PartyOneShare_1);

    doubleNumber PartyOneShare;
    double64_t t_PartyOneShare = f64_add(x1y1.sign , x1y1.exp, x1y1.sig, PartyOneShare_1.sign, PartyOneShare_1.exp, PartyOneShare_1.sig, PartyOneShare.sign, PartyOneShare.exp, PartyOneShare.sig, packingProcess);

    // cout << "Expected PartyOneShare: "<< rePackForTesting(x1y1.sign , x1y1.exp, x1y1.sig)+rePackForTesting(PartyOneShare_1.sign, PartyOneShare_1.exp, PartyOneShare_1.sig)<<endl;
    // cout << "Computed PartyOneShare: "<< rePackForTesting(PartyOneShare.sign, PartyOneShare.exp, PartyOneShare.sig) <<endl;
    
    //----------------------------

    // cout << "*********************************************************************************************************************************************************************" <<endl;

    doubleNumber PartyTwoShare_1;
    double64_t t_PartyTwoShare_1 = MultiPartyAdditionProtocol(x1y2_Share2, x1y3_Share2, x2y1_Share2, x2y3_Share2, x3y1_Share2, x3y2_Share2, PartyTwoShare_1);


    doubleNumber PartyTwoShare;
    double64_t t_PartyTwoShare = f64_add(x2y2.sign , x2y2.exp, x2y2.sig, PartyTwoShare_1.sign, PartyTwoShare_1.exp, PartyTwoShare_1.sig, PartyTwoShare.sign, PartyTwoShare.exp, PartyTwoShare.sig, packingProcess);

    // cout<<"t_PartyTwoShare should be: "<< rePackForTesting(x2y2.sign , x2y2.exp, x2y2.sig)+rePackForTesting(PartyTwoShare_1.sign, PartyTwoShare_1.exp, PartyTwoShare_1.sig)<<"================================="<<rePackForTesting(PartyTwoShare.sign, PartyTwoShare.exp, PartyTwoShare.sig)<<endl;

    // cout << "Expected PartyTwoShare: "<< rePackForTesting(x2y2)+rePackForTesting(PartyTwoShare_1)<<endl;
    // cout << "Computed PartyTwoShare: "<< rePackForTesting(PartyTwoShare) <<endl;


    // cout << "x1y2_Share2:   " << rePackForTesting(x1y2_Share2) <<endl;

    // cout << "x1y3_Share2:   " << rePackForTesting(x1y3_Share2) <<endl;

    // cout << "x2y1_Share2:   " << rePackForTesting(x2y1_Share2) <<endl;

    // cout << "x2y3_Share2:   " << rePackForTesting(x2y3_Share2) <<endl;

    // cout << "x3y1_Share2:   " << rePackForTesting(x3y1_Share2) <<endl;

    // cout << "x3y2_Share2:   " << rePackForTesting(x3y2_Share2) <<endl;

    // cout << "x2y2:          " << rePackForTesting(x2y2) <<endl;


    // cout << "Expected   PartyTwoShare_1     : " << rePackForTesting(x1y2_Share2) +  rePackForTesting(x1y3_Share2) + rePackForTesting(x2y1_Share2) + rePackForTesting(x2y3_Share2) + rePackForTesting(x3y1_Share2) + rePackForTesting(x3y2_Share2) <<endl;
    // cout << "PartyTwoShare_1                : " << rePackForTesting(PartyTwoShare_1) <<endl;



    // cout << "Expected   PartyTwoShare     : " << rePackForTesting(x1y2_Share2) +  rePackForTesting(x1y3_Share2) + rePackForTesting(x2y1_Share2) + rePackForTesting(x2y3_Share2) + rePackForTesting(x3y1_Share2) + rePackForTesting(x3y2_Share2) + rePackForTesting(x2y2) <<endl;
    // cout << "PartyTwoShare                : " << rePackForTesting(PartyTwoShare) <<endl;


    // cout << "*********************************************************************************************************************************************************************" <<endl;

    // while(1);
        
    //----------------------------

    doubleNumber PartyThreeShare_1;
    double64_t t_PartyThreeShare_1 = MultiPartyAdditionProtocol(x1y2_Share3, x1y3_Share3, x2y1_Share3, x2y3_Share3, x3y1_Share3, x3y2_Share3, PartyThreeShare_1);

    doubleNumber PartyThreeShare;
    double64_t t_PartyThreeShare = f64_add(x3y3.sign , x3y3.exp, x3y3.sig, PartyThreeShare_1.sign, PartyThreeShare_1.exp, PartyThreeShare_1.sig, PartyThreeShare.sign, PartyThreeShare.exp, PartyThreeShare.sig, packingProcess);

    // cout<<"t_PartyThreeShare should be: "<< rePackForTesting(x3y3.sign , x3y3.exp, x3y3.sig)+rePackForTesting(PartyThreeShare_1.sign, PartyThreeShare_1.exp, PartyThreeShare_1.sig)<<"================================="<<rePackForTesting(PartyThreeShare.sign, PartyThreeShare.exp, PartyThreeShare.sig)<<endl;

    // cout << "Expected PartyThreeShare: "<< rePackForTesting(x3y3)+rePackForTesting(PartyThreeShare_1)<<endl;
    // cout << "Computed PartyThreeShare: "<< rePackForTesting(PartyThreeShare) <<endl;
    
    //---------------------------

    vector<doubleNumber> finalShares;
    finalShares.clear();

    finalShares.push_back(PartyOneShare);
    finalShares.push_back(PartyTwoShare);
    finalShares.push_back(PartyThreeShare);

    return finalShares;

}

vector<doubleNumber> MultiPartyDivisionProtocol(doubleNumber in_x1, doubleNumber in_x2, doubleNumber in_x3, doubleNumber y1, doubleNumber y2, doubleNumber y3, bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ)
{

    // cout << "------------------------------------------------------------------------------ MPC --------------------------------------------------------------------------------" <<endl;

    double X1 = rePackForTesting(in_x1);

    double X2 = rePackForTesting(in_x2);

    double X3 = rePackForTesting(in_x3);

    double Y1 = rePackForTesting(y1);

    double Y2 = rePackForTesting(y2);

    double Y3 = rePackForTesting(y3);


    // cout << "X1: " << X1 <<endl;

    // cout << "X2: " << X2 <<endl;

    // cout << "X3: " << X3 <<endl;

    // cout << "Y1: " << Y1 <<endl;

    // cout << "Y2: " << Y2 <<endl;

    // cout << "Y3: " << Y3 <<endl;

    // cout<<"X1 + X2 + X3: "<<X1+X2+X3<<endl;

    // cout<<"Y1 + Y2 + Y3: "<<Y1+Y2+Y3<<endl;

    double totalX = rePackForTesting(in_x1) + rePackForTesting(in_x2) + rePackForTesting(in_x3);

    double totalY = rePackForTesting(y1) + rePackForTesting(y2) + rePackForTesting(y3);

    // cout << totalX <<endl;
    // cout << totalY <<endl;
    // cout << "Output Expected:   " << totalX/totalY <<endl;

                                                                             /* Have to Be Computed Securely/Privately */
                                                                                             /* START */



        bool signA_10 = 0; int_fast16_t expA_10 = 1026; uint_fast64_t sigA_10 = 1125899906842624;

        bool signA_100 = 0; int_fast16_t expA_100 = 1029; uint_fast64_t sigA_100 = 2533274790395904;

        bool signA_1000 = 0; int_fast16_t expA_1000 = 1032; uint_fast64_t sigA_1000 = 4292493394837504;

        bool signA_10000 = 0; int_fast16_t expA_10000 = 1036; uint_fast64_t sigA_10000 = 993958511509504;

        bool signA_100000 = 0; int_fast16_t expA_100000 = 1039; uint_fast64_t sigA_100000 = 2368348046229504;

        bool signA_1000000 = 0; int_fast16_t expA_1000000 = 1042; uint_fast64_t sigA_1000000 = 4086334964629504;

        bool signA_10000000 = 0; int_fast16_t expA_10000000 = 1046; uint_fast64_t sigA_10000000 = 865109492629504;

        bool signA_100000000 = 0; int_fast16_t expA_100000000 = 1049; uint_fast64_t sigA_100000000 = 2207286772629504;

        bool signA_1000000000 = 0; int_fast16_t expA_1000000000 = 1052; uint_fast64_t sigA_1000000000 = 3885008372629504;

        bool signA_10000000000 = 0; int_fast16_t expA_10000000000 = 1056; uint_fast64_t sigA_10000000000 = 739280372629504;

        bool signA_100000000000 = 0; int_fast16_t expA_100000000000 = 1059; uint_fast64_t sigA_100000000000 = 2050000372629504;

        bool signA_1000000000000 = 0; int_fast16_t expA_1000000000000 = 1062; uint_fast64_t sigA_1000000000000 = 3688400372629504;

        bool signA_10000000000000 = 0; int_fast16_t expA_10000000000000 = 1066; uint_fast64_t sigA_10000000000000 = 616400372629504;

        bool signA_100000000000000 = 0; int_fast16_t expA_100000000000000 = 1069; uint_fast64_t sigA_100000000000000 = 1896400372629504;

        bool signA_1000000000000000 = 0; int_fast16_t expA_1000000000000000 = 1072; uint_fast64_t sigA_1000000000000000 = 3496400372629504;

        bool signA_10000000000000000 = 0; int_fast16_t expA_10000000000000000 = 1076; uint_fast64_t sigA_10000000000000000 = 496400372629504;

        double alpha1 = 10;
        union ui64_f64 uAlpha1;
        uint_fast64_t uiAlpha1;
        
        bool signAlpha1;
        int_fast16_t expAlpha1;
        uint_fast64_t sigAlpha1;
        
        double64_t alp1=*(double64_t*) &alpha1;
        
        uAlpha1.f = alp1;
        uiAlpha1 = uAlpha1.ui;
        signAlpha1 = signF64UI( uiAlpha1 );
        expAlpha1  = expF64UI( uiAlpha1 );
        sigAlpha1  = fracF64UI( uiAlpha1 );

        // cout<<signAlpha1<<" "<<expAlpha1<<" "<<sigAlpha1<<endl;

        bool signA_ScalingFactor = 0; 
        int_fast16_t expA_ScalingFactor = 1023; 
        uint_fast64_t sigA_ScalingFactor = 0;

        int_fast16_t exp_max= max(in_x1.exp, max(in_x2.exp, in_x3.exp));

        for(int x=1;expAlpha1<2000;x++){

            alpha1=pow(10,x);
            alp1=*(double64_t*) &alpha1;
            uAlpha1.f = alp1;
            uiAlpha1 = uAlpha1.ui;
            signAlpha1 = signF64UI( uiAlpha1 );
            expAlpha1  = expF64UI( uiAlpha1 );
            sigAlpha1  = fracF64UI( uiAlpha1 );
            // cout <<"expAlpha1 should be: "<<expAlpha1<<endl;
            if(expAlpha1>exp_max){
            signA_ScalingFactor = signAlpha1; 
            expA_ScalingFactor = expAlpha1; 
            sigA_ScalingFactor = sigAlpha1;
            // cout<<"x: "<<x<<endl;
            break;
            }

        }

        // cout<<"expAlpha1 should be finally: "<<expAlpha1<<endl;
        // int_fast16_t exp_max= max(in_x1.exp, max(in_x2.exp, in_x3.exp));

        // cout << "exp_max: " << exp_max <<endl;

        // cout<<"Before: "<<endl;
        // cout << "signA_ScalingFactor: " << signA_ScalingFactor <<endl;

        // cout << "expA_ScalingFactor: " << expA_ScalingFactor <<endl;

        // cout << "sigA_ScalingFactor: " << sigA_ScalingFactor <<endl;

        // bool signA_ScalingFactor = 0; 
        // int_fast16_t expA_ScalingFactor = 1023; 
        // uint_fast64_t sigA_ScalingFactor = 0;
/*
        if(expA_10 > exp_max)
        {
            signA_ScalingFactor = signA_10; 
            expA_ScalingFactor = expA_10; 
            sigA_ScalingFactor = sigA_10;
        }
        else if(expA_100 > exp_max)
        {
            signA_ScalingFactor = signA_100; 
            expA_ScalingFactor = expA_100; 
            sigA_ScalingFactor = sigA_100;
        }
        else if(expA_1000 > exp_max)
        {
            signA_ScalingFactor = signA_1000; 
            expA_ScalingFactor = expA_1000; 
            sigA_ScalingFactor = sigA_1000;
        }
        else if(expA_10000 > exp_max)
        {
            signA_ScalingFactor = signA_10000; 
            expA_ScalingFactor = expA_10000; 
            sigA_ScalingFactor = sigA_10000;
        }
        else if(expA_100000 > exp_max)
        {
            signA_ScalingFactor = signA_100000; 
            expA_ScalingFactor = expA_100000; 
            sigA_ScalingFactor = sigA_100000;
        }
        else if(expA_1000000 > exp_max)
        {
            signA_ScalingFactor = signA_1000000; 
            expA_ScalingFactor = expA_1000000; 
            sigA_ScalingFactor = sigA_1000000;
        }
        else if(expA_10000000 > exp_max)
        {
            signA_ScalingFactor = signA_10000000; 
            expA_ScalingFactor = expA_10000000; 
            sigA_ScalingFactor = sigA_10000000;
        }
        else if(expA_100000000 > exp_max)
        {
            signA_ScalingFactor = signA_100000000; 
            expA_ScalingFactor = expA_100000000; 
            sigA_ScalingFactor = sigA_100000000;
        }
        else if(expA_1000000000 > exp_max)
        {
            signA_ScalingFactor = signA_1000000000; 
            expA_ScalingFactor = expA_1000000000; 
            sigA_ScalingFactor = sigA_1000000000;
        }
        else if(expA_10000000000 > exp_max)
        {
            signA_ScalingFactor = signA_10000000000; 
            expA_ScalingFactor = expA_10000000000; 
            sigA_ScalingFactor = sigA_10000000000;
        }
        else if(expA_100000000000 > exp_max)
        {
            signA_ScalingFactor = signA_100000000000; 
            expA_ScalingFactor = expA_100000000000; 
            sigA_ScalingFactor = sigA_100000000000;
        }
        else if(expA_1000000000000 > exp_max)
        {
            signA_ScalingFactor = signA_1000000000000; 
            expA_ScalingFactor = expA_1000000000000; 
            sigA_ScalingFactor = sigA_1000000000000;
        }
        else if(expA_10000000000000 > exp_max)
        {
            signA_ScalingFactor = signA_10000000000000; 
            expA_ScalingFactor = expA_10000000000000; 
            sigA_ScalingFactor = sigA_10000000000000;
        }
        else if(expA_100000000000000 > exp_max)
        {
            signA_ScalingFactor = signA_100000000000000; 
            expA_ScalingFactor = expA_100000000000000; 
            sigA_ScalingFactor = sigA_100000000000000;
        }
        else if(expA_1000000000000000 > exp_max)
        {
            signA_ScalingFactor = signA_1000000000000000; 
            expA_ScalingFactor = expA_1000000000000000; 
            sigA_ScalingFactor = sigA_1000000000000000;
        }
        else
        {
            signA_ScalingFactor = signA_10000000000000000; 
            expA_ScalingFactor = expA_10000000000000000; 
            sigA_ScalingFactor = sigA_10000000000000000;
        }*/

        // cout<<"After: "<<endl;
        // cout << "signA_ScalingFactor: " << signA_ScalingFactor <<endl;

        // cout << "expA_ScalingFactor: " << expA_ScalingFactor <<endl;

        // cout << "sigA_ScalingFactor: " << sigA_ScalingFactor <<endl;

                                                                                                   /* END */


    /* SCALING OF x1, x2, x3 */



    doubleNumber x1;
    double64_t x1_temp = f64_div(in_x1.sign , in_x1.exp, in_x1.sig, signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor, x1.sign, x1.exp, x1.sig);

    doubleNumber x2;
    double64_t x2_temp = f64_div(in_x2.sign , in_x2.exp, in_x2.sig, signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor, x2.sign, x2.exp, x2.sig);

    doubleNumber x3;
    double64_t x3_temp = f64_div(in_x3.sign , in_x3.exp, in_x3.sig, signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor, x3.sign, x3.exp, x3.sig);


    // cout << "After Scaling x1: " << rePackForTesting(x1) << "================"<<rePackForTesting(in_x1.sign , in_x1.exp, in_x1.sig)/rePackForTesting(signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor) <<endl;

    // cout << "After Scaling x2: " << rePackForTesting(x2) << "================"<<rePackForTesting(in_x2.sign , in_x2.exp, in_x2.sig)/rePackForTesting(signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor) <<endl;

    // cout << "After Scaling x3: " << rePackForTesting(x3) << "================"<<rePackForTesting(in_x3.sign , in_x3.exp, in_x3.sig)/rePackForTesting(signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor) <<endl;


    uint_fast64_t uiZ;
    union ui64_f64 uZ;

    uiZ = packToF64UI(x1.sign, x1.exp, x1.sig);
    uZ.ui  = uiZ;
    double64_t x1_reconstruct =uZ.f;
    double t_x1 = *(double*) &x1_reconstruct; 


    uiZ = packToF64UI(x2.sign, x2.exp, x2.sig);
    uZ.ui  = uiZ;
    double64_t x2_reconstruct = uZ.f;
    double t_x2 = *(double*) &x2_reconstruct; 


    uiZ = packToF64UI(x3.sign, x3.exp, x3.sig);
    uZ.ui  = uiZ;
    double64_t x3_reconstruct = uZ.f;
    double t_x3 = *(double*) &x3_reconstruct; 

    uiZ = packToF64UI(y1.sign, y1.exp, y1.sig);
    uZ.ui  = uiZ;
    double64_t y1_reconstruct = uZ.f;
    double t_y1 = *(double*) &y1_reconstruct; 

    uiZ = packToF64UI(y2.sign, y2.exp, y2.sig);
    uZ.ui  = uiZ;
    double64_t y2_reconstruct = uZ.f;
    double t_y2 = *(double*) &y2_reconstruct; 

    uiZ = packToF64UI(y3.sign, y3.exp, y3.sig);
    uZ.ui  = uiZ;
    double64_t y3_reconstruct = uZ.f;
    double t_y3 = *(double*) &y3_reconstruct; 


    // cout << "x1: " << t_x1 << "     " << x1.sign  << "   " <<  x1.exp  << "   " << x1.sig   <<endl;
    // cout << "x2: " << t_x2 << "     " << x2.sign  << "   " <<  x2.exp  << "   " << x2.sig   <<endl;
    // cout << "x3: " << t_x3 << "     " << x3.sign  << "   " <<  x3.exp  << "   " << x3.sig   <<endl;
    // cout << "y1: " << t_y1 <<endl;
    // cout << "y2: " << t_y2 <<endl;
    // cout << "y3: " << t_y3 <<endl;


    int packingProcess = -1;

    double64_t flt1;
    int value1 = 1;

    flt1 = *(double64_t*) &value1;

    union ui64_f64 uA;
    uint_fast64_t uiA;
    bool signA;
    int_fast16_t expA;
    uint_fast64_t sigA;


    uA.f = flt1;
    uiA = uA.ui;

    signA = 0;              //signF32UI( uiA );
    expA  = 1023;            //expF32UI( uiA );
    sigA  = 0;              //fracF32UI( uiA );

                                                                            // t_inverseX1 = 1/x1;
                                                                            // t_inverseX2 = 1/x2;
                                                                            // t_inverseX3 = 1/x3;

    doubleNumber inverseX1;
    double64_t t_inverseX1 = f64_div(signA, expA, sigA, x1.sign , x1.exp, x1.sig, inverseX1.sign, inverseX1.exp, inverseX1.sig);

    doubleNumber inverseX2;
    double64_t t_inverseX2 = f64_div(signA, expA, sigA, x2.sign , x2.exp, x2.sig, inverseX2.sign, inverseX2.exp, inverseX2.sig);

    doubleNumber inverseX3;
    double64_t t_inverseX3 = f64_div(signA, expA, sigA, x3.sign , x3.exp, x3.sig, inverseX3.sign, inverseX3.exp, inverseX3.sig);


    // cout << "Should be: " << (1.0000000000/t_x1) << "   ===============     " <<"t_inverseX1: " << *(double*) &t_inverseX1 <<endl;
    // cout << "Should be: " << (1.0000000000/t_x2) << "   ===============     " <<"t_inverseX2: " << *(double*) &t_inverseX2 <<endl;
    // cout << "Should be: " << (1.0000000000/t_x3) << "   ===============     " <<"t_inverseX3: " << *(double*) &t_inverseX3 <<endl;

    // cout <<endl;

                                                                            // t_y1x1 = y1/x1;
                                                                            // t_y2x2 = y2/x2;
                                                                            // t_y3x3 = y3/x3;


    doubleNumber y1x1;
    double64_t t_y1x1 = f64_mul(y1.sign , y1.exp, y1.sig, inverseX1.sign, inverseX1.exp, inverseX1.sig, y1x1.sign, y1x1.exp, y1x1.sig);

    doubleNumber y2x2;
    double64_t t_y2x2 = f64_mul(y2.sign , y2.exp, y2.sig, inverseX2.sign, inverseX2.exp, inverseX2.sig, y2x2.sign, y2x2.exp, y2x2.sig);

    doubleNumber y3x3;
    double64_t t_y3x3 = f64_mul(y3.sign , y3.exp, y3.sig, inverseX3.sign, inverseX3.exp, inverseX3.sig, y3x3.sign, y3x3.exp, y3x3.sig);


    // cout << "Should be: " << (Y1/t_x1) << "   ===============     " <<"t_y1x1: " << *(double*) &t_y1x1 <<endl;
    // cout << "Should be: " << (Y2/t_x2) << "   ===============     " <<"t_y2x2: " << *(double*) &t_y2x2 <<endl;
    // cout << "Should be: " << (Y3/t_x3) << "   ===============     " <<"t_y3x3: " << *(double*) &t_y3x3 <<endl;


    // cout <<endl;



                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y1, t_inverseX2, t_y1x2, t_sharesOne); 
                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y1, t_inverseX3, t_y1x3, t_sharesTwo);

                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y2, t_inverseX1, t_y2x1, t_sharesThree);
                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y2, t_inverseX3, t_y2x3, t_sharesFour);

                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y3, t_inverseX1, t_y3x1, t_sharesFive);
                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y3, t_inverseX2, t_y3x2, t_sharesSix);

                                                                                    /* Dropping this part of the code */
    doubleNumber y1x2_temp, y1x2_Share1_temp, y1x2_Share2_temp, y1x2_Share3_temp;
    double64_t t_y1x2_temp = CrossMultiplicationUsingDuatallahProtocol(y1.sign , y1.exp, y1.sig, inverseX2.sign, inverseX2.exp, inverseX2.sig, y1x2_temp.sign, y1x2_temp.exp, y1x2_temp.sig, y1x2_Share1_temp, y1x2_Share2_temp, y1x2_Share3_temp);
                                                                                    /* Dropping this part of the code */


    // cout << y1.sign << "    " <<  y1.exp << "    " <<  y1.sig << "    " <<  inverseX2.sign << "    " <<  inverseX2.exp << "    " <<  inverseX2.sig  <<endl;


    doubleNumber y1x2, y1x2_Share1, y1x2_Share2, y1x2_Share3;
    double64_t t_y1x2 = CrossMultiplicationUsingDuatallahProtocol(y1.sign , y1.exp, y1.sig, inverseX2.sign, inverseX2.exp, inverseX2.sig, y1x2.sign, y1x2.exp, y1x2.sig, y1x2_Share1, y1x2_Share2, y1x2_Share3);

    // cout << "Should be: " << (Y1/t_x2) << "   ===============     " <<"t_y1x2: " << *(double*) &t_y1x2 <<endl;
    // cout <<endl;


    // cout << y1.sign << "    " <<  y1.exp << "    " <<  y1.sig << "    " <<  inverseX3.sign << "    " <<  inverseX3.exp << "    " <<  inverseX3.sig  <<endl;

    doubleNumber y1x3, y1x3_Share1, y1x3_Share2, y1x3_Share3;
    double64_t t_y1x3 = CrossMultiplicationUsingDuatallahProtocol(y1.sign , y1.exp, y1.sig, inverseX3.sign, inverseX3.exp, inverseX3.sig, y1x3.sign, y1x3.exp, y1x3.sig, y1x3_Share1, y1x3_Share2, y1x3_Share3);

    // cout << "Should be: " << (Y1/t_x3) << "   ===============     " <<"t_y1x3: " << *(double*) &t_y1x3 <<endl;
    // cout <<endl;


                                                                                    /* Dropping this part of the code */

    double64_t t_y1x2_temp_1 = CrossMultiplicationUsingDuatallahProtocol(y1.sign , y1.exp, y1.sig, inverseX2.sign, inverseX2.exp, inverseX2.sig, y1x2_temp.sign, y1x2_temp.exp, y1x2_temp.sig, y1x2_Share1_temp, y1x2_Share2_temp, y1x2_Share3_temp);
                                                                                    /* Dropping this part of the code */

    // cout << y2.sign << "    " <<  y2.exp << "    " <<  y2.sig << "    " <<  inverseX1.sign << "    " <<  inverseX1.exp << "    " <<  inverseX1.sig  <<endl;

    doubleNumber y2x1, y2x1_Share1, y2x1_Share2, y2x1_Share3;
    double64_t t_y2x1 = CrossMultiplicationUsingDuatallahProtocol(y2.sign , y2.exp, y2.sig, inverseX1.sign, inverseX1.exp, inverseX1.sig, y2x1.sign, y2x1.exp, y2x1.sig, y2x1_Share1, y2x1_Share2, y2x1_Share3);

    // cout << "Should be: " << (Y2/t_x1) << "   ===============     " <<"t_y2x1: " << *(double*) &t_y2x1 <<endl;
    // cout <<endl;

     // cout << y2.sign << "    " <<  y2.exp << "    " <<  y2.sig << "    " <<  inverseX3.sign << "    " <<  inverseX3.exp << "    " <<  inverseX3.sig  <<endl;

    doubleNumber y2x3, y2x3_Share1, y2x3_Share2, y2x3_Share3;
    double64_t t_y2x3 = CrossMultiplicationUsingDuatallahProtocol(y2.sign , y2.exp, y2.sig, inverseX3.sign, inverseX3.exp, inverseX3.sig, y2x3.sign, y2x3.exp, y2x3.sig, y2x3_Share1, y2x3_Share2, y2x3_Share3);

    // cout << "Should be: " << (Y2/t_x3) << "   ===============     " <<"t_y2x3: " << *(double*) &t_y2x3 <<endl;
    // cout <<endl;

    // cout << y3.sign << "    " <<  y3.exp << "    " <<  y3.sig << "    " <<  inverseX1.sign << "    " <<  inverseX1.exp << "    " <<  inverseX1.sig  <<endl;

    doubleNumber y3x1, y3x1_Share1, y3x1_Share2, y3x1_Share3;
    double64_t t_y3x1 = CrossMultiplicationUsingDuatallahProtocol(y3.sign , y3.exp, y3.sig, inverseX1.sign, inverseX1.exp, inverseX1.sig, y3x1.sign, y3x1.exp, y3x1.sig, y3x1_Share1, y3x1_Share2, y3x1_Share3);

    // cout << "Should be: " << (Y3/t_x1) << "   ===============     " <<"t_y3x1: " << *(double*) &t_y3x1 <<endl;
    // cout <<endl;

    // cout << y3.sign << "    " <<  y3.exp << "    " <<  y3.sig << "    " <<  inverseX2.sign << "    " <<  inverseX2.exp << "    " <<  inverseX2.sig  <<endl;

    doubleNumber y3x2, y3x2_Share1, y3x2_Share2, y3x2_Share3;
    double64_t t_y3x2 = CrossMultiplicationUsingDuatallahProtocol(y3.sign , y3.exp, y3.sig, inverseX2.sign, inverseX2.exp, inverseX2.sig, y3x2.sign, y3x2.exp, y3x2.sig, y3x2_Share1, y3x2_Share2, y3x2_Share3);

    // cout << "Should be: " << (Y3/t_x2) << "   ===============     " <<"t_y3x2: " << *(double*) &t_y3x2 <<endl;
    // cout <<endl;


    // Party 1 : y1x1,  y1x2[1], y1x3[1], y2x1[1], y2x3[1], y3x1[1], y3x2[1] ==> y1x1 + y2x1[1] + y3x1[1] , y1x2[1] + y3x2[1], y1x3[1] + y2x3[1]

    doubleNumber partyOneShare1_1;
    double64_t t_partyOneShare1_1 = f64_add(y1x1.sign, y1x1.exp, y1x1.sig, y2x1_Share1.sign, y2x1_Share1.exp, y2x1_Share1.sig, partyOneShare1_1.sign, partyOneShare1_1.exp, partyOneShare1_1.sig, packingProcess);


    double test_1 = rePackForTesting(y2x1_Share1);
    double test_2 = rePackForTesting(y1x1);

    // cout << "Should be: " << (test_1 + test_2) << "   ===============     " <<"Result1: " << *(double*) &t_partyOneShare1_1 <<endl;
    // cout <<endl;

    doubleNumber partyOneShare1;
    double64_t t_partyOneShare1 = f64_add(partyOneShare1_1.sign, partyOneShare1_1.exp, partyOneShare1_1.sig, y3x1_Share1.sign, y3x1_Share1.exp, y3x1_Share1.sig, partyOneShare1.sign, partyOneShare1.exp, partyOneShare1.sig, packingProcess);

    
    double test_4 = rePackForTesting(y3x1_Share1);
    double test_3 = rePackForTesting(partyOneShare1_1);


    // cout << "Should be: " << (test_3 + test_4) << "   ===============     " <<"Result2: " << *(double*) &t_partyOneShare1 <<endl;
    // cout <<endl;


    doubleNumber partyOneShare2;
    double64_t t_partyOneShare2 = f64_add(y1x2_Share1.sign, y1x2_Share1.exp, y1x2_Share1.sig, y3x2_Share3.sign, y3x2_Share3.exp, y3x2_Share3.sig, partyOneShare2.sign, partyOneShare2.exp, partyOneShare2.sig, packingProcess);

    double test_5 = rePackForTesting(y1x2_Share1);
    double test_6 = rePackForTesting(y3x2_Share3);

    // cout << "Should be: " << (test_5 + test_6) << "   ===============     " <<"Result3: " << *(double*) &t_partyOneShare2 <<endl;
    // cout <<endl;

    doubleNumber partyOneShare3;
    double64_t t_partyOneShare3 = f64_add(y1x3_Share1.sign, y1x3_Share1.exp, y1x3_Share1.sig, y2x3_Share3.sign, y2x3_Share3.exp, y2x3_Share3.sig, partyOneShare3.sign, partyOneShare3.exp, partyOneShare3.sig, packingProcess);

    double test_7 = rePackForTesting(y1x3_Share1);
    double test_8 = rePackForTesting(y2x3_Share3);

    // cout << "Should be: " << (test_7 + test_8) << "   ===============     " <<"Result4: " << *(double*) &t_partyOneShare3 <<endl;
    // cout <<endl;

    // Party 2 : y2x2,  y1x2[2], y1x3[2], y2x1[2], y2x3[2], y3x1[2], y3x2[2] ==> y2x1[2] + y3x1[2] , y2x2 + y1x2[2] + y3x2[2], y1x3[2] + y2x3[2]

    doubleNumber partyTwoShare1;
    double64_t t_partyTwoShare1 = f64_add(y2x1_Share2.sign, y2x1_Share2.exp, y2x1_Share2.sig, y3x1_Share3.sign, y3x1_Share3.exp, y3x1_Share3.sig, partyTwoShare1.sign, partyTwoShare1.exp, partyTwoShare1.sig, packingProcess);

    double test_9 = rePackForTesting(y2x1_Share2);
    double test_10 = rePackForTesting(y3x1_Share3);

    // cout << "Should be: " << (test_9 + test_10) << "   ===============     " <<"Result5: " << *(double*) &t_partyTwoShare1 <<endl;
    // cout <<endl;

    doubleNumber partyTwoShare2_1;
    double64_t t_partyTwoShare2_1 = f64_add(y2x2.sign, y2x2.exp, y2x2.sig, y1x2_Share2.sign, y1x2_Share2.exp, y1x2_Share2.sig, partyTwoShare2_1.sign, partyTwoShare2_1.exp, partyTwoShare2_1.sig, packingProcess);

    double test_11 = rePackForTesting(y2x2);
    double test_12 = rePackForTesting(y1x2_Share2);

    // cout << "Should be: " << (test_11 + test_12) << "   ===============     " <<"Result6: " << *(double*) &t_partyTwoShare2_1 <<endl;
    // cout <<endl;

    doubleNumber partyTwoShare2;
    double64_t t_partyTwoShare2 = f64_add(partyTwoShare2_1.sign, partyTwoShare2_1.exp, partyTwoShare2_1.sig, y3x2_Share2.sign, y3x2_Share2.exp, y3x2_Share2.sig, partyTwoShare2.sign, partyTwoShare2.exp, partyTwoShare2.sig, packingProcess);

    double test_13 = rePackForTesting(partyTwoShare2_1);
    double test_14 = rePackForTesting(y3x2_Share2);

    // cout << "Should be: " << (test_13 + test_14) << "   ===============     " <<"Result7: " << *(double*) &t_partyTwoShare2 <<endl;
    // cout <<endl;


    doubleNumber partyTwoShare3;
    double64_t t_partyTwoShare3 = f64_add(y1x3_Share3.sign, y1x3_Share3.exp, y1x3_Share3.sig, y2x3_Share2.sign, y2x3_Share2.exp, y2x3_Share2.sig, partyTwoShare3.sign, partyTwoShare3.exp, partyTwoShare3.sig, packingProcess);

    double test_15 = rePackForTesting(y1x3_Share3);
    double test_16 = rePackForTesting(y2x3_Share2);

    // cout << "Should be: " << (test_15 + test_16) << "   ===============     " <<"Result8: " << *(double*) &t_partyTwoShare3 <<endl;
    // cout <<endl;

    // Party 3 : y3x3,  y1x2[3], y1x3[3], y2x1[3], y2x3[3], y3x1[3], y3x2[3] ==> y2x1[3] + y3x1[3] , y1x2[3] + y3x2[3], y3x3 + y1x3[3] + y2x3[3]


    doubleNumber partyThreeShare1;
    double64_t t_partyThreeShare1 = f64_add(y2x1_Share3.sign, y2x1_Share3.exp, y2x1_Share3.sig, y3x1_Share2.sign, y3x1_Share2.exp, y3x1_Share2.sig, partyThreeShare1.sign, partyThreeShare1.exp, partyThreeShare1.sig, packingProcess);

    double test_17 = rePackForTesting(y2x1_Share3);
    double test_18 = rePackForTesting(y3x1_Share2);

    // cout << "Should be: " << (test_17 + test_18) << "   ===============     " <<"Result9: " << *(double*) &t_partyThreeShare1 <<endl;
    // cout <<endl;

    doubleNumber partyThreeShare2;
    double64_t t_partyThreeShare2 = f64_add(y1x2_Share3.sign, y1x2_Share3.exp, y1x2_Share3.sig, y3x2_Share1.sign, y3x2_Share1.exp, y3x2_Share1.sig, partyThreeShare2.sign, partyThreeShare2.exp, partyThreeShare2.sig, packingProcess);

    double test_19 = rePackForTesting(y1x2_Share3);
    double test_20 = rePackForTesting(y3x2_Share1);

    // cout << "Should be: " << (test_19 + test_20) << "   ===============     " <<"Result10: " << *(double*) &t_partyThreeShare2 <<endl;
    // cout <<endl;


    doubleNumber partyThreeShare3_1;
    double64_t t_partyThreeShare3_1 = f64_add(y3x3.sign, y3x3.exp, y3x3.sig, y1x3_Share2.sign, y1x3_Share2.exp, y1x3_Share2.sig, partyThreeShare3_1.sign, partyThreeShare3_1.exp, partyThreeShare3_1.sig, packingProcess);

    double test_21 = rePackForTesting(y3x3);
    double test_22 = rePackForTesting(y1x3_Share2);

    // cout << "Should be: " << (test_21 + test_22) << "   ===============     " <<"Result11: " << *(double*) &t_partyThreeShare3_1 <<endl;
    // cout <<endl;

    doubleNumber partyThreeShare3;
    double64_t t_partyThreeShare3 = f64_add(partyThreeShare3_1.sign, partyThreeShare3_1.exp, partyThreeShare3_1.sig, y2x3_Share1.sign, y2x3_Share1.exp, y2x3_Share1.sig, partyThreeShare3.sign, partyThreeShare3.exp, partyThreeShare3.sig, packingProcess);

    double test_23 = rePackForTesting(partyThreeShare3_1);
    double test_24 = rePackForTesting(y2x3_Share1);

    // cout << "Should be: " << (test_23 + test_24) << "   ===============     " <<"Result12: " << *(double*) &t_partyThreeShare3 <<endl;
    // cout <<endl;



    // cout << "---------------------------------------------------------------------Tested Till Here-----------------------------------------------------------------------------------" <<endl;


    // Party 1: partyOneShare3 + partytwoShare3 + partythreeShare3


    doubleNumber finalShareOne_1;
    double64_t t_finalShareOne_1 = f64_add(partyOneShare3.sign, partyOneShare3.exp, partyOneShare3.sig, partyTwoShare3.sign, partyTwoShare3.exp, partyTwoShare3.sig, finalShareOne_1.sign, finalShareOne_1.exp, finalShareOne_1.sig, packingProcess);

    test_23 = rePackForTesting(partyOneShare3);
    test_24 = rePackForTesting(partyTwoShare3);

    // cout << "Should be: " << (test_23 + test_24) << "   ===============     " <<"Result13: " << rePackForTesting(finalShareOne_1) <<endl;

    doubleNumber finalShareOne;
    double64_t t_finalShareOne = f64_add(finalShareOne_1.sign, finalShareOne_1.exp, finalShareOne_1.sig, partyThreeShare3.sign, partyThreeShare3.exp, partyThreeShare3.sig, finalShareOne.sign, finalShareOne.exp, finalShareOne.sig, packingProcess);

    test_23 = rePackForTesting(finalShareOne_1);
    test_24 = rePackForTesting(partyThreeShare3);

    // cout << "Should be: " << (test_23 + test_24) << "   ===============     " <<"Result14: " << rePackForTesting(finalShareOne) <<endl;
// 
    // Party 2: partyOneShare1 + partytwoShare1 + partythreeShare1

    doubleNumber finalShareTwo_1;
    double64_t t_finalShareTwo_1 = f64_add(partyOneShare1.sign, partyOneShare1.exp, partyOneShare1.sig, partyTwoShare1.sign, partyTwoShare1.exp, partyTwoShare1.sig, finalShareTwo_1.sign, finalShareTwo_1.exp, finalShareTwo_1.sig, packingProcess);

    test_23 = rePackForTesting(partyOneShare1);
    test_24 = rePackForTesting(partyTwoShare1);

    // cout << "Should be: " << (test_23 + test_24) << "   ===============     " <<"Result15: " << rePackForTesting(finalShareTwo_1) <<endl;


    doubleNumber finalShareTwo;
    double64_t t_finalShareTwo = f64_add(finalShareTwo_1.sign, finalShareTwo_1.exp, finalShareTwo_1.sig, partyThreeShare1.sign, partyThreeShare1.exp, partyThreeShare1.sig, finalShareTwo.sign, finalShareTwo.exp, finalShareTwo.sig, packingProcess);

    test_23 = rePackForTesting(finalShareTwo_1);
    test_24 = rePackForTesting(partyThreeShare1);

    // cout << "Should be: " << (test_23 + test_24) << "   ===============     " <<"Result16: " << rePackForTesting(finalShareTwo) <<endl;



    // Party 3: partyOneShare2 + partytwoShare2 + partythreeShare2

    doubleNumber finalShareThree_1;
    double64_t t_finalShareThree_1 = f64_add(partyOneShare2.sign, partyOneShare2.exp, partyOneShare2.sig, partyTwoShare2.sign, partyTwoShare2.exp, partyTwoShare2.sig, finalShareThree_1.sign, finalShareThree_1.exp, finalShareThree_1.sig, packingProcess);

     test_23 = rePackForTesting(partyOneShare2);
    test_24 = rePackForTesting(partyTwoShare2);

    // cout << "Should be: " << (test_23 + test_24) << "   ===============     " <<"Result17: " << rePackForTesting(finalShareThree_1) <<endl;


    doubleNumber finalShareThree;
    double64_t t_finalShareThree = f64_add(finalShareThree_1.sign, finalShareThree_1.exp, finalShareThree_1.sig, partyThreeShare2.sign, partyThreeShare2.exp, partyThreeShare2.sig, finalShareThree.sign, finalShareThree.exp, finalShareThree.sig, packingProcess);


    test_23 = rePackForTesting(finalShareThree_1);
    test_24 = rePackForTesting(partyThreeShare2);

    // cout << "Should be: " << (test_23 + test_24) << "   ===============     " <<"Result18: " << rePackForTesting(finalShareThree) <<endl;


    // Inverse finalShareOne, finalShareTwo, finalShareThree

    doubleNumber inverse_finalShareOne;
    double64_t t_inverse_finalShareOne = f64_div(signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor, finalShareOne.sign , finalShareOne.exp, finalShareOne.sig, inverse_finalShareOne.sign, inverse_finalShareOne.exp, inverse_finalShareOne.sig);

    test_23 = rePackForTesting(signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor);
    test_24 = rePackForTesting(finalShareOne.sign , finalShareOne.exp, finalShareOne.sig);

    // cout << "Should be: " << (test_23 / test_24) << "   ===============     " <<"Result19: " << rePackForTesting(inverse_finalShareOne) <<endl;



    doubleNumber inverse_finalShareTwo;
    double64_t t_inverse_finalShareTwo = f64_div(signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor, finalShareTwo.sign , finalShareTwo.exp, finalShareTwo.sig, inverse_finalShareTwo.sign, inverse_finalShareTwo.exp, inverse_finalShareTwo.sig);

     test_23 = rePackForTesting(signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor);
    test_24 = rePackForTesting(finalShareTwo.sign , finalShareTwo.exp, finalShareTwo.sig);

    // cout << "Should be: " << (test_23 / test_24) << "   ===============     " <<"Result20: " << rePackForTesting(inverse_finalShareTwo) <<endl;


    doubleNumber inverse_finalShareThree;
    double64_t t_inverse_finalShareThree = f64_div(signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor, finalShareThree.sign , finalShareThree.exp, finalShareThree.sig, inverse_finalShareThree.sign, inverse_finalShareThree.exp, inverse_finalShareThree.sig);

    test_23 = rePackForTesting(signA_ScalingFactor , expA_ScalingFactor, sigA_ScalingFactor);
    test_24 = rePackForTesting(finalShareThree.sign , finalShareThree.exp, finalShareThree.sig);

    // cout << "Should be: " << (test_23 / test_24) << "   ===============     " <<"Result21: " << rePackForTesting(inverse_finalShareThree) <<endl;



    // Share Total: finalShareOne + finalShareTwo + finalShareThree


    doubleNumber finalOutput_1;
    double64_t t_finalOutput_1 = f64_add(inverse_finalShareOne.sign, inverse_finalShareOne.exp, inverse_finalShareOne.sig, inverse_finalShareTwo.sign, inverse_finalShareTwo.exp, inverse_finalShareTwo.sig, finalOutput_1.sign, finalOutput_1.exp, finalOutput_1.sig, packingProcess);

     test_23 = rePackForTesting(inverse_finalShareOne.sign, inverse_finalShareOne.exp, inverse_finalShareOne.sig);
    test_24 = rePackForTesting(inverse_finalShareTwo.sign, inverse_finalShareTwo.exp, inverse_finalShareTwo.sig);

    // cout << "Should be: " << (test_23 + test_24) << "   ===============     " <<"Result22: " << rePackForTesting(finalOutput_1) <<endl;



    doubleNumber finalOutput;
    double64_t t_finalOutput = f64_add(finalOutput_1.sign, finalOutput_1.exp, finalOutput_1.sig, inverse_finalShareThree.sign, inverse_finalShareThree.exp, inverse_finalShareThree.sig, finalOutput.sign, finalOutput.exp, finalOutput.sig, packingProcess);

    test_23 = rePackForTesting(finalOutput_1.sign, finalOutput_1.exp, finalOutput_1.sig);
    test_24 = rePackForTesting(inverse_finalShareThree.sign, inverse_finalShareThree.exp, inverse_finalShareThree.sig);

    // cout << "Should be: " << (test_23 + test_24) << "   ===============     " <<"Result23: " << rePackForTesting(finalOutput) <<endl;


    // Output Assignment

    signZ = finalOutput.sign;
    expZ  = finalOutput.exp;
    sigZ  = finalOutput.sig;

    double output = *(double*) &t_finalOutput; 

    vector<doubleNumber> allDivisionSharesFinal;
    allDivisionSharesFinal.clear();


    double expected = totalX/totalY;

    double Computed = output;

    // cout << "relative error" << (abs(Computed - expected)/Computed) <<endl;


    allDivisionSharesFinal.push_back(inverse_finalShareOne);

    allDivisionSharesFinal.push_back(inverse_finalShareTwo);

    allDivisionSharesFinal.push_back(inverse_finalShareThree);


    // cout << "output:    " << output <<endl;


    // cout << "y1x1: " << rePackForTesting(y1x1) <<endl;
    // cout << y1x1.sign <<endl;
    // cout << y1x1.exp <<endl;
    // cout << y1x1.sig <<endl;


    // cout << "y2x2: " << rePackForTesting(y2x2) <<endl;

    // cout << y2x2.sign <<endl;
    // cout << y2x2.exp <<endl;
    // cout << y2x2.sig <<endl;

    // cout << "y3x3: " << rePackForTesting(y3x3) <<endl;

    // cout << y3x3.sign <<endl;
    // cout << y3x3.exp <<endl;
    // cout << y3x3.sig <<endl;

    // cout << "y1x2_Share1: " << rePackForTesting(y1x2_Share1) <<endl;

    // cout << y1x2_Share1.sign <<endl;
    // cout << y1x2_Share1.exp <<endl;
    // cout << y1x2_Share1.sig <<endl;

    // cout << "y1x2_Share2: " << rePackForTesting(y1x2_Share2) <<endl;

    // cout << y1x2_Share2.sign <<endl;
    // cout << y1x2_Share2.exp <<endl;
    // cout << y1x2_Share2.sig <<endl;


    // cout << "y1x2_Share3: " << rePackForTesting(y1x2_Share3) <<endl;

    // cout << y1x2_Share3.sign <<endl;
    // cout << y1x2_Share3.exp <<endl;
    // cout << y1x2_Share3.sig <<endl;
    
    // cout << "y1x3_Share1: " << rePackForTesting(y1x3_Share1) <<endl;

    // cout << y1x3_Share1.sign <<endl;
    // cout << y1x3_Share1.exp <<endl;
    // cout << y1x3_Share1.sig <<endl;

    // cout << "y1x3_Share2: " << rePackForTesting(y2x1_Share2) <<endl;

    // cout << y2x1_Share2.sign <<endl;
    // cout << y2x1_Share2.exp <<endl;
    // cout << y2x1_Share2.sig <<endl;

    // cout << "y1x3_Share3: " << rePackForTesting(y3x1_Share3) <<endl;

    // cout << y3x1_Share3.sign <<endl;
    // cout << y3x1_Share3.exp <<endl;
    // cout << y3x1_Share3.sig <<endl;

    // cout << "y2x1_Share1: " << rePackForTesting(y2x1_Share1) <<endl;

    // cout << y2x1_Share1.sign <<endl;
    // cout << y2x1_Share1.exp <<endl;
    // cout << y2x1_Share1.sig <<endl;


    // cout << "y2x1_Share2: " << rePackForTesting(y2x1_Share2) <<endl;

    // cout << y2x1_Share2.sign <<endl;
    // cout << y2x1_Share2.exp <<endl;
    // cout << y2x1_Share2.sig <<endl;


    // cout << "y2x1_Share3: " << rePackForTesting(y2x1_Share3) <<endl;

    // cout << y2x1_Share3.sign <<endl;
    // cout << y2x1_Share3.exp <<endl;
    // cout << y2x1_Share3.sig <<endl;

    
    // cout << "y2x3_Share1: " << rePackForTesting(y2x3_Share1) <<endl;

    // cout << y2x3_Share1.sign <<endl;
    // cout << y2x3_Share1.exp <<endl;
    // cout << y2x3_Share1.sig <<endl;

    // cout << "y2x3_Share2: " << rePackForTesting(y2x3_Share2) <<endl;

    // cout << y2x3_Share2.sign <<endl;
    // cout << y2x3_Share2.exp <<endl;
    // cout << y2x3_Share2.sig <<endl;

    // cout << "y2x3_Share3: " << rePackForTesting(y2x3_Share3) <<endl;

    // cout << y2x3_Share3.sign <<endl;
    // cout << y2x3_Share3.exp <<endl;
    // cout << y2x3_Share3.sig <<endl;

    // cout << "y3x1_Share1: " << rePackForTesting(y3x1_Share1) <<endl;

    // cout << y3x1_Share1.sign <<endl;
    // cout << y3x1_Share1.exp <<endl;
    // cout << y3x1_Share1.sig <<endl;

    // cout << "y3x1_Share2: " << rePackForTesting(y3x1_Share2) <<endl;

    // cout << y3x1_Share2.sign <<endl;
    // cout << y3x1_Share2.exp <<endl;
    // cout << y3x1_Share2.sig <<endl;

    // cout << "y3x1_Share3: " << rePackForTesting(y3x1_Share3) <<endl;

    // cout << y3x1_Share3.sign <<endl;
    // cout << y3x1_Share3.exp <<endl;
    // cout << y3x1_Share3.sig <<endl;
    
    // cout << "y3x2_Share1: " << rePackForTesting(y3x2_Share1) <<endl;

    // cout << y3x2_Share1.sign <<endl;
    // cout << y3x2_Share1.exp <<endl;
    // cout << y3x2_Share1.sig <<endl;

    // cout << "y3x2_Share2: " << rePackForTesting(y3x2_Share2) <<endl;

    // cout << y3x2_Share2.sign <<endl;
    // cout << y3x2_Share2.exp <<endl;
    // cout << y3x2_Share2.sig <<endl;

    // cout << "y3x2_Share3: " << rePackForTesting(y3x2_Share3) <<endl;

    // cout << y3x2_Share3.sign <<endl;
    // cout << y3x2_Share3.exp <<endl;
    // cout << y3x2_Share3.sig <<endl;

    // printf("MPC-DIV: Computed output is \t%.400f\n\n", output);


    // cout << "------------------------------------------------------------------------------ MPC END--------------------------------------------------------------------------------" <<endl;
// 
    // while(1);

    return allDivisionSharesFinal;

}

double CrossMultiplicationUsingDuatallahProtocol(double X1, double X2, double &Z, double &Share1, double &Share2, double &Share3)
{

    // double alpha1,alpha2;
    // alpha1 = (double)(rand()) / (double)(rand()) ; //(rand() % 10) + 1;
    // alpha2 = (double)(rand()) / (double)(rand()) ;

    // alpha1 = 2.04999713368002112190424668369814753532409667968750;
    // alpha2 = 0.36462958398842182372590059458161704242229461669922;


    double output_Alpha1_Alpha2_Expected = alpha1 * alpha2;

    //------------------------------------------------------------------------------------------------------------------------------------------------------------------

    double temp1 = (X1 + alpha1);

    //------------------------------------------------------------------------------------------------------------------------------------------------------------------

    double temp2 = (X2 + alpha2);
   
    //------------------------------------------------------------------------------------------------------------------------------------------------------------------

    double temp3 = X1 * (X2 + alpha2);

    double temp4 = (X1 + alpha1) * (X2 + alpha2);

    double temp5 = -((X1 + alpha1) * (X2 + alpha2)) + (X1*(X2+alpha2));

    //------------------------------------------------------------------------------------------------------------------------------------------------------------------

    double temp6 = X2*(X1+alpha1);
  
    //------------------------------------------------------------------------------------------------------------------------------------------------------------------

    double temp7 = output_Alpha1_Alpha2_Expected + temp5;
   
    double output_Expected =  temp7 + temp6;


    Z = output_Expected;

    Share1 = output_Alpha1_Alpha2_Expected;
    Share2 = temp5;
    Share3 = temp6;


    // cout << "Share1: " << Share1 << endl;

    // cout << "Share2: " << Share2 << endl;

    // cout << "Share3: " << Share3 << endl;

    return output_Expected;

}

vector<double> DuatallahMultiplicationProtocol(double x1, double x2, double x3, double y1, double y2, double y3)
{
    int packingProcess = 1;

    // cout << "------------------------------------------------------------------------- DuatallahMultiplicationProtocol FP-----------------------------------------------------------------------------" <<endl;

    // local computation

    double x1y1 = x1 * y1;

    // cout << "Expected And Computed (x1y1): " << x1 * y1 <<endl;
   
    double x2y2 = x2 * y2;

    // cout << "Expected And Computed (x2y2): " << x2 * y2 <<endl;
   
    double x3y3 = x3 * y3;

    // cout << "Expected And Computed (x3y3): " << x3 * y3 <<endl;
   
    // cout << "----------------------------------------------------------------------------------------------------------------------------------------------" <<endl;

    // cross share multiplication

    double x1y2, x1y2_Share1, x1y2_Share2, x1y2_Share3;
    double t_x1y2 = CrossMultiplicationUsingDuatallahProtocol(x1, y2, x1y2, x1y2_Share1, x1y2_Share2, x1y2_Share3);

    // cout << "Expected (x1y2): " << x1 * y2 <<endl;
    // cout << "Computed (x1y2): " << x1y2 <<endl;
    // cout << "SumShare (x1y2): " << x1y2_Share1 + x1y2_Share2 + x1y2_Share3 <<endl;

    double x1y3, x1y3_Share1, x1y3_Share2, x1y3_Share3;
    double t_x1y3 = CrossMultiplicationUsingDuatallahProtocol(x1, y3, x1y3, x1y3_Share1, x1y3_Share2, x1y3_Share3);

    // cout << "Expected (x1y3): " << x1 * y3 <<endl;
    // cout << "Computed (x1y3): " << x1y3 <<endl;
    // cout << "SumShare (x1y3): " << x1y3_Share1 + x1y3_Share2 + x1y3_Share3 <<endl;


    double x2y1, x2y1_Share1, x2y1_Share2, x2y1_Share3;
    double t_x2y1 = CrossMultiplicationUsingDuatallahProtocol(x2, y1, x2y1, x2y1_Share1, x2y1_Share2, x2y1_Share3);

    // cout << "Expected (x2y1): " << x2 * y1 <<endl;
    // cout << "Computed (x2y1): " << x2y1 <<endl;
    // cout << "SumShare (x2y1): " << x2y1_Share1 + x2y1_Share2 + x2y1_Share3 <<endl;

    double x2y3, x2y3_Share1, x2y3_Share2, x2y3_Share3;
    double t_x2y3 = CrossMultiplicationUsingDuatallahProtocol(x2, y3, x2y3, x2y3_Share1, x2y3_Share2, x2y3_Share3);

    // cout << "Expected (x2y3): " << x2 * y3 <<endl;
    // cout << "Computed (x2y3): " << x2y3 <<endl;
    // cout << "SumShare (x2y3): " << x2y3_Share1 + x2y3_Share2 + x2y3_Share3 <<endl;

    double x3y1, x3y1_Share1, x3y1_Share2, x3y1_Share3;
    double t_x3y1 = CrossMultiplicationUsingDuatallahProtocol(x3, y1, x3y1, x3y1_Share1, x3y1_Share2, x3y1_Share3);

    // cout << "Expected (x3y1): " << x3 * y1 <<endl;
    // cout << "Computed (x3y1): " << x3y1 <<endl;
    // cout << "SumShare (x3y1): " << x3y1_Share1 + x3y1_Share2 + x3y1_Share3 <<endl;

    double x3y2, x3y2_Share1, x3y2_Share2, x3y2_Share3;
    double t_x3y2 = CrossMultiplicationUsingDuatallahProtocol(x3, y2, x3y2, x3y2_Share1, x3y2_Share2, x3y2_Share3);

    // cout << "Expected (x3y2): " << x3 * y2 <<endl;
    // cout << "Computed (x3y2): " << x3y2 <<endl;
    // cout << "SumShare (x3y2): " << x3y2_Share1 + x3y2_Share2 + x3y2_Share3 <<endl;


    // cout << "Total: " << x1y1 + x2y2 + x3y3 + x1y2 + x1y3 + x2y1 + x2y3 + x3y1 + x3y2 <<endl;

    // Share Creation for Each Party

    double PartyOneShare = (x1y2_Share1 + x1y3_Share1 + x2y1_Share1 + x2y3_Share1 + x3y1_Share1 + x3y2_Share1 + x1y1);

    // cout << "PartyOneShare: " << PartyOneShare <<endl;
  
    //----------------------------

    double PartyTwoShare = (x1y2_Share2 + x1y3_Share2 + x2y1_Share2 + x2y3_Share2 + x3y1_Share2 + x3y2_Share2 + x2y2);
    double PartyTwoShare_1 = (x1y2_Share2 + x1y3_Share2 + x2y1_Share2 + x2y3_Share2 + x3y1_Share2 + x3y2_Share2);

    // cout << "x1y2_Share2:   " << x1y2_Share2 <<endl;

    // cout << "x1y3_Share2:   " << x1y3_Share2 <<endl;

    // cout << "x2y1_Share2:   " << x2y1_Share2 <<endl;

    // cout << "x2y3_Share2:   " << x2y3_Share2 <<endl;

    // cout << "x3y1_Share2:   " << x3y1_Share2 <<endl;

    // cout << "x3y2_Share2:   " << x3y2_Share2 <<endl;

    // cout << "x2y2:          " << x2y2 <<endl;

    // cout << "PartyTwoShare_1: " << PartyTwoShare_1 <<endl;

    // cout << "PartyTwoShare: " << PartyTwoShare <<endl;

     
    //----------------------------

    double PartyThreeShare = (x1y2_Share3 + x1y3_Share3 + x2y1_Share3 + x2y3_Share3 + x3y1_Share3 + x3y2_Share3 + x3y3);

    // cout << "PartyThreeShare: " << PartyThreeShare <<endl;
  
    //---------------------------

    vector<double> finalShares;
    finalShares.clear();

    finalShares.push_back(PartyOneShare);
    finalShares.push_back(PartyTwoShare);
    finalShares.push_back(PartyThreeShare);

    return finalShares;

}

vector<double> MultiPartyDivisionProtocol(double in_x1, double in_x2, double in_x3, double y1, double y2, double y3, double &output_value)
{

    double totalX = (in_x1) + (in_x2) + (in_x3);

    double totalY = (y1) + (y2) + (y3);

    // cout << "Output Expected:   " << totalX/totalY <<endl;

    double ScaleFactor = 10;
    double maxValue = fmaxf(in_x1, fmaxf(in_x2, in_x3));

    for(int x=1; x<2000; x++)
    {
        ScaleFactor = pow(10,x);

        if(ScaleFactor > maxValue)
        {
            break;
        }

    }

    double x1 = in_x1/ScaleFactor;
    double x2 = in_x2/ScaleFactor;
    double x3 = in_x3/ScaleFactor;



                                                                            // t_inverseX1 = 1/x1;
                                                                            // t_inverseX2 = 1/x2;
                                                                            // t_inverseX3 = 1/x3;

    double t_valueOne = 1.00000000;

    double inverseX1 = (t_valueOne/x1);
    double inverseX2 = (t_valueOne/x2);
    double inverseX3 = (t_valueOne/x3);


    // cout <<endl;

                                                                            // t_y1x1 = y1/x1;
                                                                            // t_y2x2 = y2/x2;
                                                                            // t_y3x3 = y3/x3;


    double y1x1 = y1 * inverseX1;
    double y2x2 = y2 * inverseX2;
    double y3x3 = y3 * inverseX3;



                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y1, t_inverseX2, t_y1x2, t_sharesOne); 
                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y1, t_inverseX3, t_y1x3, t_sharesTwo);

                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y2, t_inverseX1, t_y2x1, t_sharesThree);
                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y2, t_inverseX3, t_y2x3, t_sharesFour);

                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y3, t_inverseX1, t_y3x1, t_sharesFive);
                                                    // DuatallahMultiplicationProtocolUsingdoubleingPoint(y3, t_inverseX2, t_y3x2, t_sharesSix);

                                                                                    /* Dropping this part of the code */



    double y1x2, y1x2_Share1, y1x2_Share2, y1x2_Share3;
    double t_y1x2 = CrossMultiplicationUsingDuatallahProtocol(y1, inverseX2, y1x2, y1x2_Share1, y1x2_Share2, y1x2_Share3);


    double y1x3, y1x3_Share1, y1x3_Share2, y1x3_Share3;
    double t_y1x3 = CrossMultiplicationUsingDuatallahProtocol(y1, inverseX3, y1x3, y1x3_Share1, y1x3_Share2, y1x3_Share3);


    double y2x1, y2x1_Share1, y2x1_Share2, y2x1_Share3;
    double t_y2x1 = CrossMultiplicationUsingDuatallahProtocol(y2, inverseX1, y2x1, y2x1_Share1, y2x1_Share2, y2x1_Share3);

    double y2x3, y2x3_Share1, y2x3_Share2, y2x3_Share3;
    double t_y2x3 = CrossMultiplicationUsingDuatallahProtocol(y2, inverseX3, y2x3, y2x3_Share1, y2x3_Share2, y2x3_Share3);

    double y3x1, y3x1_Share1, y3x1_Share2, y3x1_Share3;
    double t_y3x1 = CrossMultiplicationUsingDuatallahProtocol(y3, inverseX1, y3x1, y3x1_Share1, y3x1_Share2, y3x1_Share3);

    double y3x2, y3x2_Share1, y3x2_Share2, y3x2_Share3;
    double t_y3x2 = CrossMultiplicationUsingDuatallahProtocol(y3, inverseX2, y3x2, y3x2_Share1, y3x2_Share2, y3x2_Share3);

    // cout << "y1x1: " << y1x1 <<endl;
    // cout << "y2x2: " << y2x2 <<endl;
    // cout << "y3x3: " << y3x3 <<endl;

    // cout << "y1x2_Share1: " << y1x2_Share1 <<endl;
    // cout << "y1x2_Share2: " << y1x2_Share2 <<endl;
    // cout << "y1x2_Share3: " << y1x2_Share3 <<endl;
    
    // cout << "y1x3_Share1: " << y1x3_Share1 <<endl;
    // cout << "y1x3_Share2: " << y2x1_Share2 <<endl;
    // cout << "y1x3_Share3: " << y3x1_Share3 <<endl;

    // cout << "y2x1_Share1: " << y2x1_Share1 <<endl;
    // cout << "y2x1_Share2: " << y2x1_Share2 <<endl;
    // cout << "y2x1_Share3: " << y2x1_Share3 <<endl;
    
    // cout << "y2x3_Share1: " << y2x3_Share1 <<endl;
    // cout << "y2x3_Share2: " << y2x3_Share2 <<endl;
    // cout << "y2x3_Share3: " << y2x3_Share3 <<endl;

    // cout << "y3x1_Share1: " << y3x1_Share1 <<endl;
    // cout << "y3x1_Share2: " << y3x1_Share2 <<endl;
    // cout << "y3x1_Share3: " << y3x1_Share3 <<endl;
    
    // cout << "y3x2_Share1: " << y3x2_Share1 <<endl;
    // cout << "y3x2_Share2: " << y3x2_Share2 <<endl;
    // cout << "y3x2_Share3: " << y3x2_Share3 <<endl;

    // Party 1 : y1x1,  y1x2[1], y1x3[1], y2x1[1], y2x3[1], y3x1[1], y3x2[1] ==> y1x1 + y2x1[1] + y3x1[1] , y1x2[1] + y3x2[1], y1x3[1] + y2x3[1]

    double partyOneShare1 = y1x1 + y2x1_Share1 + y3x1_Share1;

    double partyOneShare2 = y1x2_Share1 + y3x2_Share3;

    double partyOneShare3 = y1x3_Share1 + y2x3_Share3;

    // Party 2 : y2x2,  y1x2[2], y1x3[2], y2x1[2], y2x3[2], y3x1[2], y3x2[2] ==> y2x1[2] + y3x1[2] , y2x2 + y1x2[2] + y3x2[2], y1x3[2] + y2x3[2]

    double partyTwoShare1 = y2x1_Share2 + y3x1_Share3;

    double partyTwoShare2 = y2x2 + y1x2_Share2 + y3x2_Share2;

    double partyTwoShare3 = y1x3_Share3 + y2x3_Share2;

    // Party 3 : y3x3,  y1x2[3], y1x3[3], y2x1[3], y2x3[3], y3x1[3], y3x2[3] ==> y2x1[3] + y3x1[3] , y1x2[3] + y3x2[3], y3x3 + y1x3[3] + y2x3[3]


    double partyThreeShare1 = y2x1_Share3 + y3x1_Share2;

    double partyThreeShare2 = y1x2_Share3 + y3x2_Share1;
   
    double partyThreeShare3 = y3x3 + y1x3_Share2 + y2x3_Share1;



    // cout << "---------------------------------------------------------------------Tested Till Here-----------------------------------------------------------------------------------" <<endl;


    // Party 1: partyOneShare3 + partytwoShare3 + partythreeShare3

    double finalShareOne = partyOneShare3 + partyTwoShare3 + partyThreeShare3;

    // Party 2: partyOneShare1 + partytwoShare1 + partythreeShare1
  
    double finalShareTwo = partyOneShare1 + partyTwoShare1 + partyThreeShare1;

    // Party 3: partyOneShare2 + partytwoShare2 + partythreeShare2

    double finalShareThree = partyOneShare2 + partyTwoShare2 + partyThreeShare2;

    // Inverse finalShareOne, finalShareTwo, finalShareThree

    double inverse_finalShareOne = ScaleFactor/finalShareOne;

    double inverse_finalShareTwo = ScaleFactor/finalShareTwo;

    double inverse_finalShareThree = ScaleFactor/finalShareThree;

    // Share Total: finalShareOne + finalShareTwo + finalShareThree

    double finalOutput = inverse_finalShareOne + inverse_finalShareTwo + inverse_finalShareThree;

    // cout << "partyOneShare1: " << partyOneShare1 <<endl;
    // cout << "partyTwoShare1: " << partyTwoShare1 <<endl;
    // cout << "partyThreeShare1: " << partyThreeShare1 <<endl;

    // cout << "finalShareTwo: " << finalShareTwo <<endl;

    // cout << "inverse_finalShareTwo: " << inverse_finalShareTwo <<endl;

    // cout << "inverse_finalShareThree: " << inverse_finalShareThree <<endl;

    // Output Assignment

    vector<double> finalOutputShares;
    finalOutputShares.clear();

    finalOutputShares.push_back(inverse_finalShareOne);

    finalOutputShares.push_back(inverse_finalShareTwo);

    finalOutputShares.push_back(inverse_finalShareThree);

    output_value = finalOutput; 

    // printf("MPC-DIV: Computed output is \t%.400f\n\n", output_value);


    return finalOutputShares;

}


vector<double> computeConsumerPricingSingalUsingdoubleingpoint(vector<double> in_consumerPrice, vector<double> in_individualDemand, vector<double> in_individualSupply, double in_sDotLambdaZeroDivConsumer, vector<double> in_wDotLambdaZero, int counter)
{


    cout << "Price  (FP) : " << std::accumulate(in_consumerPrice.begin(), in_consumerPrice.end(), 0) <<endl;
    cout << "Supply  (FP) : " << std::accumulate(in_individualSupply.begin(), in_individualSupply.end(), 0) <<endl;

    vector<double> out_consumerPrice;

    out_consumerPrice.clear();

    int packingProcess = 1;

                                                                                    // Computations By Consumer
    

    // Constant Part Computation

    vector<double> t_constantTerm;
    t_constantTerm.clear();

    for(int i = 0; i < numconsumers ; i++)
    {
        double temp_denominator = in_sDotLambdaZeroDivConsumer - in_wDotLambdaZero[i];
        double temp_ConstantValue = (double)((double)(2.00000000000000000) * ETA)/temp_denominator;
        t_constantTerm.push_back(temp_ConstantValue);
    }       


    // Secret Share of in_consumerPrice

    vector<vector<doubleNumber>> conPriceShare;
    conPriceShare.clear();

    vector<vector<double>> conPriceShare_withDouble;
    conPriceShare_withDouble.clear();


    for(int i = 0; i < 10 ; i++)
    {   
        vector<double> temp_conPriceShare_withDouble;
        temp_conPriceShare_withDouble.clear();

        vector<doubleNumber> temp_conPriceShare = obtainShares(in_consumerPrice[i], -1);
        conPriceShare.push_back(temp_conPriceShare);

        temp_conPriceShare_withDouble.push_back(rePackForTesting(temp_conPriceShare[0]));

        temp_conPriceShare_withDouble.push_back(rePackForTesting(temp_conPriceShare[1]));

        temp_conPriceShare_withDouble.push_back(rePackForTesting(temp_conPriceShare[2]));

        conPriceShare_withDouble.push_back(temp_conPriceShare_withDouble);
    }

    for (int i = 10; i < numconsumers; i++) 
    {
        int index = i % 10;
        conPriceShare.push_back(conPriceShare[index]);
        conPriceShare_withDouble.push_back(conPriceShare_withDouble[index]);
    }

    // cout <<endl;

    // Computing Total Price

    vector<doubleNumber> totalPriceShares;
    totalPriceShares.clear();

    for(int i = 0; i < 3; i ++)
    {
        doubleNumber totalPriceShare;
        totalPriceShare.sign = 0;
        totalPriceShare.exp = 0;
        totalPriceShare.sig = 0;

        for(int j = 0; j < numconsumers ; j++)
        {        
            double64_t t_temp_totalPriceShare = f64_add(totalPriceShare.sign, totalPriceShare.exp, totalPriceShare.sig, conPriceShare[j][i].sign, conPriceShare[j][i].exp, conPriceShare[j][i].sig, totalPriceShare.sign, totalPriceShare.exp, totalPriceShare.sig, packingProcess); 
        }

        totalPriceShares.push_back(totalPriceShare);
    }

    vector<double> totalPriceShares_double;
    totalPriceShares_double.clear();

    for(int i = 0; i < 3; i ++)
    {
        double temp_before = 0;

        for(int j = 0; j < numconsumers ; j++)
        {        
            temp_before =  temp_before +  conPriceShare_withDouble[j][i];
        }

        totalPriceShares_double.push_back(temp_before);
    }

    cout << "total price MPC: " << (rePackForTesting(totalPriceShares[0]) + rePackForTesting(totalPriceShares[1]) + rePackForTesting(totalPriceShares[2])) <<endl;

    cout << "total price FP : " << (totalPriceShares_double[0] + totalPriceShares_double[1] + totalPriceShares_double[2]) <<endl;

    // Secret Share of in_individualSupply

    vector<vector<doubleNumber>> supSuppShare;
    supSuppShare.clear();

    vector<vector<double>> conSupplyShare_withDouble;
    conSupplyShare_withDouble.clear();

    for(int i = 0; i < numsuppliers ; i++)
    {
        vector<double> temp_conSupplyShare_withDouble;
        temp_conSupplyShare_withDouble.clear();

        vector<doubleNumber> temp_supSuppShare = obtainShares(in_individualSupply[i], 0);
        supSuppShare.push_back(temp_supSuppShare);

        temp_conSupplyShare_withDouble.push_back(rePackForTesting(temp_supSuppShare[0]));
        temp_conSupplyShare_withDouble.push_back(rePackForTesting(temp_supSuppShare[1]));
        temp_conSupplyShare_withDouble.push_back(rePackForTesting(temp_supSuppShare[2]));

        conSupplyShare_withDouble.push_back(temp_conSupplyShare_withDouble);
    }

    // Computing Total Supply

    vector<doubleNumber> totalSupplyShares;
    totalSupplyShares.clear();

    double totalSupply1=0;

    for(int i = 0; i < 3; i ++)
    {
        doubleNumber totalSupplyShare;
        totalSupplyShare.sign = 0;
        totalSupplyShare.exp = 0;
        totalSupplyShare.sig = 0;

        for(int j = 0; j < numsuppliers ; j++)
        {
            double64_t t_temp_totalSupplyShare = f64_add(totalSupplyShare.sign, totalSupplyShare.exp, totalSupplyShare.sig, supSuppShare[j][i].sign, supSuppShare[j][i].exp, supSuppShare[j][i].sig, totalSupplyShare.sign, totalSupplyShare.exp, totalSupplyShare.sig, packingProcess);
        }

        totalSupplyShares.push_back(totalSupplyShare);
    }

    vector<double> totalSupplyShares_double;
    totalSupplyShares_double.clear();

    for(int i = 0; i < 3; i ++)
    {
        double temp_before = 0;

        for(int j = 0; j < numsuppliers ; j++)
        {
            temp_before =  temp_before +  conSupplyShare_withDouble[j][i];
        }

        totalSupplyShares_double.push_back(temp_before);

    }

    cout << "total Supply MPC: " << (rePackForTesting(totalSupplyShares[0]) + rePackForTesting(totalSupplyShares[1]) + rePackForTesting(totalSupplyShares[2])) <<endl;

    cout << "total Supply FP : " << (totalSupplyShares_double[0] + totalSupplyShares_double[1] + totalSupplyShares_double[2]) <<endl;



    vector<vector<doubleNumber>> conDemandShare;
    conDemandShare.clear();

    vector<vector<double>> conDemandShare_withDouble;
    conDemandShare_withDouble.clear();

    for(int i = 0; i < numconsumers ; i++)
    {
        vector<double> temp_conDemandShare_withDouble;
        temp_conDemandShare_withDouble.clear();

        // cout << i << "::::" << in_individualDemand[i] <<endl;

        vector<doubleNumber> temp_conDemandShare = obtainShares(in_individualDemand[i], -1);
        conDemandShare.push_back(temp_conDemandShare);

        temp_conDemandShare_withDouble.push_back(rePackForTesting(temp_conDemandShare[0]));
        temp_conDemandShare_withDouble.push_back(rePackForTesting(temp_conDemandShare[1]));
        temp_conDemandShare_withDouble.push_back(rePackForTesting(temp_conDemandShare[2]));

        conDemandShare_withDouble.push_back(temp_conDemandShare_withDouble);
    }


    // cout << "Done" <<endl;


    for(int i = 0; i < numconsumers ; i++)
    {

        // cout << "Consumer ID: " << i <<endl;


        // cout  << rePackForTesting(conPriceShare[i][0]) << "::::" << rePackForTesting(conPriceShare[i][1]) << "::::" << rePackForTesting(conPriceShare[i][2]) << "::::" << rePackForTesting(totalPriceShares[0]) << "::::"  << rePackForTesting(totalPriceShares[1]) << "::::"  << rePackForTesting(totalPriceShares[2]) << "::::" <<endl;

        doubleNumber t_divisionShare;
        vector<doubleNumber> divisionShare = MultiPartyDivisionProtocol(conPriceShare[i][0], conPriceShare[i][1], conPriceShare[i][2], totalPriceShares[0], totalPriceShares[1], totalPriceShares[2], t_divisionShare.sign, t_divisionShare.exp, t_divisionShare.sig);

        double t_divisionShareFP;
        vector<double> divisionShareFP = MultiPartyDivisionProtocol(conPriceShare_withDouble[i][0], conPriceShare_withDouble[i][1], conPriceShare_withDouble[i][2], totalPriceShares_double[0], totalPriceShares_double[1], totalPriceShares_double[2], t_divisionShareFP);



        // cout << "***************** inputs: ***********************" <<endl;

        // cout << "MPC: " << rePackForTesting(totalSupplyShares[0]) <<endl;

        // cout << "FP : " << (totalSupplyShares_double[0]) <<endl;

        // cout << "MPC: " << rePackForTesting(totalSupplyShares[1]) <<endl;

        // cout << "FP : " << (totalSupplyShares_double[1]) <<endl;

        // cout << "MPC: " << rePackForTesting(totalSupplyShares[2]) <<endl;

        // cout << "FP : " << (totalSupplyShares_double[2]) <<endl;

        // cout << "MPC: " << rePackForTesting(divisionShare[0]) <<endl;

        // cout << "FP : " << (divisionShareFP[0]) <<endl;

        // cout << "MPC: " << rePackForTesting(divisionShare[1]) <<endl;

        // cout << "FP : " << (divisionShareFP[1]) <<endl;

        // cout << "MPC: " << rePackForTesting(divisionShare[2]) <<endl;

        // cout << "FP : " << (divisionShareFP[2]) <<endl;

        // cout << "***********************************************" <<endl;



        vector<doubleNumber> supplyTimesDivisionShares = DuatallahMultiplicationProtocol(totalSupplyShares[0], totalSupplyShares[1], totalSupplyShares[2], divisionShare[0], divisionShare[1], divisionShare[2]);

        vector<double> supplyTimesDivisionSharesFP = DuatallahMultiplicationProtocol(totalSupplyShares_double[0], totalSupplyShares_double[1], totalSupplyShares_double[2], divisionShareFP[0], divisionShareFP[1], divisionShareFP[2]);


        conDemandShare[i][0].sign = conDemandShare[i][0].sign ^ 1;

        conDemandShare[i][1].sign = conDemandShare[i][1].sign ^ 1;

        conDemandShare[i][2].sign = conDemandShare[i][2].sign ^ 1;



        double finalComputedValue = 0;
        vector<doubleNumber> allfinalSharesOfRTP = MultiPartyAdditionProtocolForFinalComputation(supplyTimesDivisionShares[0], conDemandShare[i][0],  supplyTimesDivisionShares[1], conDemandShare[i][1], supplyTimesDivisionShares[2], conDemandShare[i][2], finalComputedValue);


        double t_generationErrorForConsumer = finalComputedValue;

        double t_generationErrorForConsumerFP = (supplyTimesDivisionSharesFP[0] - conDemandShare_withDouble[i][0]) + (supplyTimesDivisionSharesFP[1] - conDemandShare_withDouble[i][1]) + (supplyTimesDivisionSharesFP[2] - conDemandShare_withDouble[i][2]) ;


        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        // cout << "t_generationErrorForConsumer FP : " <<  t_generationErrorForConsumerFP <<endl;

        // cout << "t_generationErrorForConsumer MPC: " << t_generationErrorForConsumer <<endl;

        
        double changeInPrice = t_constantTerm[i] * t_generationErrorForConsumer;
        double changeInPriceFP = t_constantTerm[i] * t_generationErrorForConsumerFP;

        // cout << "changeInPrice MPC: " << changeInPrice <<endl;
        // cout << "changeInPrice FP : " << changeInPriceFP <<endl <<endl;

        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        out_consumerPrice.push_back(in_consumerPrice[i] - changeInPrice);




        if(i == 0)
        {
            
            cout << "MPC SCENARIO" <<endl;
           if((counter == 1) || (counter == 2) || (counter == 3))
            {
                cout << "Counter: " << counter <<endl;
                
                cout << "Price (MPC) : " << rePackForTesting(totalPriceShares[0]) + rePackForTesting(totalPriceShares[1]) + rePackForTesting(totalPriceShares[2]) <<endl;
                cout << "Supply (MPC) : " << rePackForTesting(totalSupplyShares[0]) + rePackForTesting(totalSupplyShares[1]) + rePackForTesting(totalSupplyShares[2]) <<endl;


                

                // cout << "Division  (FP) : " << std::accumulate(supplyTimesDivisionSharesFP.begin(), supplyTimesDivisionSharesFP.end(), 0) <<endl;
                cout << "Division (MPC) : " << (rePackForTesting(totalSupplyShares[0]) + rePackForTesting(totalSupplyShares[1]) + rePackForTesting(totalSupplyShares[2]))/(rePackForTesting(totalPriceShares[0]) + rePackForTesting(totalPriceShares[1]) + rePackForTesting(totalPriceShares[2])) <<endl;
 

                // cout << "t_generationErrorForConsumer FP : " <<  t_generationErrorForConsumerFP <<endl;

                cout << "t_generationErrorForConsumer MPC: " << t_generationErrorForConsumer <<endl;

                cout << "changeInPrice MPC: " << changeInPrice <<endl;
                // cout << "changeInPrice FP : " << changeInPriceFP <<endl <<endl;

                cout << "***********************************************" <<endl;

            }

            // cout << "bye" <<endl;
        }





        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        // cout << "Total Con Price MPC: " << (rePackForTesting(conPriceShare[i][0]) + rePackForTesting(conPriceShare[i][1]) + rePackForTesting(conPriceShare[i][2])) <<endl;
        // cout << "Total Con Price FP : " << (conPriceShare_withDouble[i][0] + conPriceShare_withDouble[i][1] + conPriceShare_withDouble[i][2]) << endl;


        // cout << "total price MPC: " << (rePackForTesting(totalSupplyShares[0]) + rePackForTesting(totalSupplyShares[1]) + rePackForTesting(totalSupplyShares[2])) <<endl;
        // cout << "Total Supply FP: " << (totalSupplyShares_double[0] + totalSupplyShares_double[1] + totalSupplyShares_double[2]) << endl <<endl;

        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        // vector<doubleNumber> supplyTimesIndividualPrice = DuatallahMultiplicationProtocol(totalSupplyShares[0], totalSupplyShares[1], totalSupplyShares[2], conPriceShare[i][0], conPriceShare[i][1], conPriceShare[i][2]);

        // vector<double> supplyTimesIndividualPriceFP = DuatallahMultiplicationProtocol(totalSupplyShares_double[0], totalSupplyShares_double[1], totalSupplyShares_double[2], conPriceShare_withDouble[i][0], conPriceShare_withDouble[i][1], conPriceShare_withDouble[i][2]);


        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        // // cout << "Expected Value    FP : " << (conPriceShare_withDouble[i][0] + conPriceShare_withDouble[i][1] + conPriceShare_withDouble[i][2]) * (totalSupplyShares_double[0] + totalSupplyShares_double[1] + totalSupplyShares_double[2]) <<endl;

        // // cout << "Expected Value   MPC : " << (rePackForTesting(totalSupplyShares[0]) + rePackForTesting(totalSupplyShares[1]) + rePackForTesting(totalSupplyShares[2])) * (rePackForTesting(conPriceShare[i][0]) + rePackForTesting(conPriceShare[i][1]) + rePackForTesting(conPriceShare[i][2]))<<endl;

        // cout << "Total Numerator 1 FP : " << (supplyTimesIndividualPriceFP[0] + supplyTimesIndividualPriceFP[1] + supplyTimesIndividualPriceFP[2]) <<endl;

        // cout << "Total Numerator 1 MPC: " << (rePackForTesting(supplyTimesIndividualPrice[0]) + rePackForTesting(supplyTimesIndividualPrice[1]) + rePackForTesting(supplyTimesIndividualPrice[2])) <<endl <<endl;


        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        // // while(1);


        // vector<doubleNumber> individualDemandTimestotalPrice = DuatallahMultiplicationProtocol(totalPriceShares[0], totalPriceShares[1], totalPriceShares[2], conDemandShare[i][0], conDemandShare[i][1], conDemandShare[i][2]);

        // vector<double> individualDemandTimestotalPriceFP = DuatallahMultiplicationProtocol(totalPriceShares_double[0], totalPriceShares_double[1], totalPriceShares_double[2], conDemandShare_withDouble[i][0], conDemandShare_withDouble[i][1], conDemandShare_withDouble[i][2]);


        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        // // cout << "Expected Value    FP : " << (conDemandShare_withDouble[i][0] + conDemandShare_withDouble[i][1] + conDemandShare_withDouble[i][2]) * (totalPriceShares_double[0] + totalPriceShares_double[1] + totalPriceShares_double[2]) <<endl;

        // // cout << "Expected Value   MPC : " << (rePackForTesting(totalPriceShares[0]) + rePackForTesting(totalPriceShares[1]) + rePackForTesting(totalPriceShares[2])) * (rePackForTesting(conDemandShare[i][0]) + rePackForTesting(conDemandShare[i][1]) + rePackForTesting(conDemandShare[i][2]))<<endl;

        // cout << "Total Numerator 2 FP : " << (individualDemandTimestotalPriceFP[0] + individualDemandTimestotalPriceFP[1] + individualDemandTimestotalPriceFP[2]) <<endl;

        // cout << "Total Numerator 2 MPC: " << (rePackForTesting(individualDemandTimestotalPrice[0]) + rePackForTesting(individualDemandTimestotalPrice[1]) + rePackForTesting(individualDemandTimestotalPrice[2])) <<endl <<endl;


        // // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;


        // individualDemandTimestotalPrice[0].sign = individualDemandTimestotalPrice[0].sign ^ 1;
        // individualDemandTimestotalPrice[1].sign = individualDemandTimestotalPrice[1].sign ^ 1;
        // individualDemandTimestotalPrice[2].sign = individualDemandTimestotalPrice[2].sign ^ 1;

        // doubleNumber numeratorShareOne;
        // double64_t t_numeratorShareOne = f64_add(supplyTimesIndividualPrice[0].sign, supplyTimesIndividualPrice[0].exp, supplyTimesIndividualPrice[0].sig, individualDemandTimestotalPrice[0].sign, individualDemandTimestotalPrice[0].exp, individualDemandTimestotalPrice[0].sig, numeratorShareOne.sign, numeratorShareOne.exp, numeratorShareOne.sig, packingProcess);


        // doubleNumber numeratorShareTwo;
        // double64_t t_numeratorShareTwo = f64_add(supplyTimesIndividualPrice[1].sign, supplyTimesIndividualPrice[1].exp, supplyTimesIndividualPrice[1].sig, individualDemandTimestotalPrice[1].sign, individualDemandTimestotalPrice[1].exp, individualDemandTimestotalPrice[1].sig, numeratorShareTwo.sign, numeratorShareTwo.exp, numeratorShareTwo.sig, packingProcess);


        // doubleNumber numeratorShareThree;
        // double64_t t_numeratorShareThree = f64_add(supplyTimesIndividualPrice[2].sign, supplyTimesIndividualPrice[2].exp, supplyTimesIndividualPrice[2].sig, individualDemandTimestotalPrice[2].sign, individualDemandTimestotalPrice[2].exp, individualDemandTimestotalPrice[2].sig, numeratorShareThree.sign, numeratorShareThree.exp, numeratorShareThree.sig, packingProcess);


        // double numeratorShareOneFP = supplyTimesIndividualPriceFP[0] - individualDemandTimestotalPriceFP[0];
        // double numeratorShareTwoFP = supplyTimesIndividualPriceFP[1] - individualDemandTimestotalPriceFP[1];
        // double numeratorShareThreeFP = supplyTimesIndividualPriceFP[2] - individualDemandTimestotalPriceFP[2];


        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        // cout << "Total Numerator FP : " << (numeratorShareOneFP + numeratorShareTwoFP + numeratorShareThreeFP) <<endl;

        // // // cout << "Expected Value  MPC: " << (rePackForTesting(supplyTimesIndividualPrice[0]) + rePackForTesting(supplyTimesIndividualPrice[1]) + rePackForTesting(supplyTimesIndividualPrice[2])) + (rePackForTesting(individualDemandTimestotalPrice[0]) + rePackForTesting(individualDemandTimestotalPrice[1]) + rePackForTesting(individualDemandTimestotalPrice[2])) <<endl;

        // cout << "Total Numerator MPC: " << (rePackForTesting(numeratorShareOne) + rePackForTesting(numeratorShareTwo) + rePackForTesting(numeratorShareThree)) <<endl <<endl;


        // // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;


        // doubleNumber finalShare;
        // double64_t t_finalShare = MultiPartyDivisionProtocol(numeratorShareOne, numeratorShareTwo, numeratorShareThree, totalPriceShares[0], totalPriceShares[1], totalPriceShares[2], finalShare.sign, finalShare.exp, finalShare.sig);

        // double finalShareFP;
        // double t_finalShareFP = MultiPartyDivisionProtocol(numeratorShareOneFP, numeratorShareTwoFP, numeratorShareThreeFP, totalPriceShares_double[0], totalPriceShares_double[1], totalPriceShares_double[2], finalShareFP);


        // Reconstruction and Final Price Computation, DONE by Consumers

        // double t_generationErrorForConsumer = rePackForTesting(finalShare);
        // double t_generationErrorForConsumerFP = t_finalShareFP;


        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        // cout << "GSE FP Expected                 : " <<  ((numeratorShareOneFP + numeratorShareTwoFP + numeratorShareThreeFP)/(totalPriceShares_double[0] + totalPriceShares_double[1] + totalPriceShares_double[2])) <<endl;

        // cout << "GSE MPC Expected                : " <<  ((rePackForTesting(numeratorShareOne) + rePackForTesting(numeratorShareTwo) + rePackForTesting(numeratorShareThree))/((rePackForTesting(totalPriceShares[0]) + rePackForTesting(totalPriceShares[1]) + rePackForTesting(totalPriceShares[2])))) <<endl;

        // cout << "t_generationErrorForConsumer FP : " <<  t_generationErrorForConsumerFP <<endl;

        // cout << "t_generationErrorForConsumer MPC: " << t_generationErrorForConsumer <<endl;

        


        // double changeInPrice = t_constantTerm[i] * t_generationErrorForConsumer;
        // double changeInPriceFP = t_constantTerm[i] * t_generationErrorForConsumerFP;

        // cout << "changeInPrice MPC: " << changeInPrice <<endl;
        // cout << "changeInPrice FP : " << changeInPriceFP <<endl <<endl;

        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        // out_consumerPrice.push_back(in_consumerPrice[i] - changeInPrice);

        // cout << "out_consumerPrice: "<<endl;

        // cout <<  out_consumerPrice[i] << ",";

        // while(1);
    }

    // cout <<endl;

    return out_consumerPrice;

}


vector<double> computeSupplierPricingSingal(vector<double> in_supplierPrice, vector<double> in_individualSupply, vector<double> in_individualDemand, double t_wDotLambdaZeroDivSupplier, vector<double> in_sDotLambdaZero)
{

    vector<double> out_supplierPrice;
    out_supplierPrice.clear();

    int packingProcess = 1;

                                                                                    // Computations By Consumer
    

    // Constant Part Computation

    vector<double> t_constantTerm;
    t_constantTerm.clear();

    for(int i = 0; i < numsuppliers ; i++)
    {
        double temp_denominator = in_sDotLambdaZero[i] - t_wDotLambdaZeroDivSupplier;
        double temp_ConstantValue = (double)((double)(2.00000000000000000) * ETA)/temp_denominator;
        t_constantTerm.push_back(temp_ConstantValue);
    }       

    // Secret Share of in_consumerPrice

    vector<vector<doubleNumber>> supPriceShare;
    supPriceShare.clear();

    vector<vector<double>> supPriceShareFP;
    supPriceShareFP.clear();

    for(int i = 0; i < numsuppliers ; i++)
    {
        vector<double> supPriceShare_withDouble;
        supPriceShare_withDouble.clear();

        vector<doubleNumber> temp_supPriceShare = obtainShares(in_supplierPrice[i], -1);
        supPriceShare.push_back(temp_supPriceShare);

        supPriceShare_withDouble.push_back(rePackForTesting(temp_supPriceShare[0]));

        supPriceShare_withDouble.push_back(rePackForTesting(temp_supPriceShare[1]));

        supPriceShare_withDouble.push_back(rePackForTesting(temp_supPriceShare[2]));

        supPriceShareFP.push_back(supPriceShare_withDouble);
    }

    // Secret Share of in_individualDemand

    vector<vector<doubleNumber>> conDemandShare;
    conDemandShare.clear();

    vector<vector<double>> conDemandShareFP;
    conDemandShareFP.clear();

    for(int i = 0; i < numconsumers ; i++)
    {
        vector<double> conDemandShare_withDouble;
        conDemandShare_withDouble.clear();

        vector<doubleNumber> temp_conDemandShare = obtainShares(in_individualDemand[i], -1);

        conDemandShare.push_back(temp_conDemandShare);

        conDemandShare_withDouble.push_back(rePackForTesting(temp_conDemandShare[0]));

        conDemandShare_withDouble.push_back(rePackForTesting(temp_conDemandShare[1]));

        conDemandShare_withDouble.push_back(rePackForTesting(temp_conDemandShare[2]));

        conDemandShareFP.push_back(conDemandShare_withDouble);
    }

                                                                                    // Computations By Supplier

    // Secret Share of in_individualSupply

    vector<vector<doubleNumber>> supSuppShare;
    supSuppShare.clear();

    vector<vector<double>> supSuppShareFP;
    supSuppShareFP.clear();

    for(int i = 0; i < numsuppliers ; i++)
    {
        vector<double> supSuppShare_withDouble;
        supSuppShare_withDouble.clear();

        vector<doubleNumber> temp_supSuppShare = obtainShares(in_individualSupply[i], -1);

        supSuppShare.push_back(temp_supSuppShare);

        supSuppShare_withDouble.push_back(rePackForTesting(temp_supSuppShare[0]));

        supSuppShare_withDouble.push_back(rePackForTesting(temp_supSuppShare[1]));

        supSuppShare_withDouble.push_back(rePackForTesting(temp_supSuppShare[2]));

        supSuppShareFP.push_back(supSuppShare_withDouble);
    }

                                                                                    // 3-Party MPC Computation Starts


    // Computing Total Price

    vector<doubleNumber> totalPriceShares;
    totalPriceShares.clear();

    for(int i = 0; i < 3; i ++)
    {
        doubleNumber totalPriceShare;
        totalPriceShare.sign = 0;
        totalPriceShare.exp = 0;
        totalPriceShare.sig = 0;

        for(int j = 0; j < numsuppliers ; j++)
        {
           double64_t t_temp_totalPriceShare = f64_add(totalPriceShare.sign, totalPriceShare.exp, totalPriceShare.sig, supPriceShare[j][i].sign, supPriceShare[j][i].exp, supPriceShare[j][i].sig, totalPriceShare.sign, totalPriceShare.exp, totalPriceShare.sig, packingProcess);
        }

        totalPriceShares.push_back(totalPriceShare);
    }

    vector<double> totalPriceShares_double;
    totalPriceShares_double.clear();

     double totalPrice1 = 0;

    for(int i = 0; i < 3; i ++)
    {
        double temp_before = 0; 
        for(int j = 0; j < numsuppliers ; j++)
        {
            temp_before =  temp_before +  supPriceShareFP[j][i];
        }

        totalPriceShares_double.push_back(temp_before);
        totalPrice1 += temp_before ;
    }


    // Computing Total Demand

    vector<doubleNumber> totalDemandShares;
    totalDemandShares.clear();

    for(int i = 0; i < 3; i ++)
    {
        doubleNumber totalDemandShare;
        totalDemandShare.sign = 0;
        totalDemandShare.exp = 0;
        totalDemandShare.sig = 0;

        for(int j = 0; j < numconsumers ; j++)
        {
            double64_t t_temp_totalDemandShare = f64_add(totalDemandShare.sign, totalDemandShare.exp, totalDemandShare.sig, conDemandShare[j][i].sign, conDemandShare[j][i].exp, conDemandShare[j][i].sig, totalDemandShare.sign, totalDemandShare.exp, totalDemandShare.sig, packingProcess);
        }

        totalDemandShares.push_back(totalDemandShare);
    }

    vector<double> totalDemandShares_double;
    totalDemandShares_double.clear();

    for(int i = 0; i < 3; i ++)
    {
        double temp_before = 0;  

        for(int j = 0; j < numconsumers ; j++)
        {
            temp_before =  temp_before +  conDemandShareFP[j][i];

        }

        totalDemandShares_double.push_back(temp_before);
    }


    // Compute MPC Multiplications 

    for(int i = 0; i < numsuppliers ; i++)
    {


        // cout << "supplier ID: " << i <<endl;

        doubleNumber t_divisionShare;
        vector<doubleNumber> divisionShare = MultiPartyDivisionProtocol(supPriceShare[i][0], supPriceShare[i][1], supPriceShare[i][2], totalPriceShares[0], totalPriceShares[1], totalPriceShares[2], t_divisionShare.sign, t_divisionShare.exp, t_divisionShare.sig);

        double t_divisionShareFP;
        vector<double> divisionShareFP = MultiPartyDivisionProtocol(supPriceShareFP[i][0], supPriceShareFP[i][1], supPriceShareFP[i][2], totalPriceShares_double[0], totalPriceShares_double[1], totalPriceShares_double[2], t_divisionShareFP);



        // cout << "***************** inputs: ***********************" <<endl;

        // cout << "MPC: " << rePackForTesting(totalDemandShares[0]) <<endl;

        // cout << "FP : " << (totalDemandShares_double[0]) <<endl;

        // cout << "MPC: " << rePackForTesting(totalDemandShares[1]) <<endl;

        // cout << "FP : " << (totalDemandShares_double[1]) <<endl;

        // cout << "MPC: " << rePackForTesting(totalDemandShares[2]) <<endl;

        // cout << "FP : " << (totalDemandShares_double[2]) <<endl;

        // cout << "MPC: " << rePackForTesting(divisionShare[0]) <<endl;

        // cout << "FP : " << (divisionShareFP[0]) <<endl;

        // cout << "MPC: " << rePackForTesting(divisionShare[1]) <<endl;

        // cout << "FP : " << (divisionShareFP[1]) <<endl;

        // cout << "MPC: " << rePackForTesting(divisionShare[2]) <<endl;

        // cout << "FP : " << (divisionShareFP[2]) <<endl;

        // cout << "***********************************************" <<endl;

        


        vector<doubleNumber> supplyTimesDivisionShares = DuatallahMultiplicationProtocol(totalDemandShares[0], totalDemandShares[1], totalDemandShares[2], divisionShare[0], divisionShare[1], divisionShare[2]);

        vector<double> supplyTimesDivisionSharesFP = DuatallahMultiplicationProtocol(totalDemandShares_double[0], totalDemandShares_double[1], totalDemandShares_double[2], divisionShareFP[0], divisionShareFP[1], divisionShareFP[2]);


        supplyTimesDivisionShares[0].sign = supplyTimesDivisionShares[0].sign ^ 1;

        supplyTimesDivisionShares[1].sign = supplyTimesDivisionShares[1].sign ^ 1;

        supplyTimesDivisionShares[2].sign = supplyTimesDivisionShares[2].sign ^ 1;

      
        double finalComputedValue = 0;
        vector<doubleNumber> allfinalSharesOfRTP = MultiPartyAdditionProtocolForFinalComputation(supSuppShare[i][0], supplyTimesDivisionShares[0],  supSuppShare[i][1], supplyTimesDivisionShares[1], supSuppShare[i][2], supplyTimesDivisionShares[2], finalComputedValue);

        double t_generationErrorForSupplier = finalComputedValue;

        double t_generationErrorForSupplierFP = (supPriceShareFP[i][0] - supplyTimesDivisionSharesFP[0]) + ( supPriceShareFP[i][1] - supplyTimesDivisionSharesFP[1]) + (supPriceShareFP[i][2] - supplyTimesDivisionSharesFP[2] ) ;


        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        // cout << "t_generationErrorForSuppiler FP : " <<  t_generationErrorForSupplier <<endl;

        // cout << "t_generationErrorForSuppiler MPC: " << t_generationErrorForSupplierFP <<endl;

        
        double changeInPrice = t_constantTerm[i] * t_generationErrorForSupplier;
        double changeInPriceFP = t_constantTerm[i] * t_generationErrorForSupplierFP;

        // cout << "changeInPrice MPC: " << changeInPrice <<endl;
        // cout << "changeInPrice FP : " << changeInPriceFP <<endl <<endl;

        // cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl <<endl;

        out_supplierPrice.push_back(in_supplierPrice[i] - changeInPrice);




        // vector<doubleNumber> demandTimesIndividualPrice = DuatallahMultiplicationProtocol(totalDemandShares[0], totalDemandShares[1], totalDemandShares[2], supPriceShare[i][0], supPriceShare[i][1], supPriceShare[i][2]);

        // vector<doubleNumber> individualSupplyTimestotalPrice = DuatallahMultiplicationProtocol(totalPriceShares[0], totalPriceShares[1], totalPriceShares[2], supSuppShare[i][0], supSuppShare[i][1], supSuppShare[i][2]);

        // vector<double> demandTimesIndividualPriceFP = DuatallahMultiplicationProtocol(totalDemandShares_double[0], totalDemandShares_double[1], totalDemandShares_double[2], supPriceShareFP[i][0], supPriceShareFP[i][1], supPriceShareFP[i][2]);

        // vector<double> individualSupplyTimestotalPriceFP = DuatallahMultiplicationProtocol(totalPriceShares_double[0], totalPriceShares_double[1], totalPriceShares_double[2], supSuppShareFP[i][0], supSuppShareFP[i][1], supSuppShareFP[i][2]);

        // //Subtraction 

        // demandTimesIndividualPrice[0].sign = demandTimesIndividualPrice[0].sign ^ 1;
        // demandTimesIndividualPrice[1].sign = demandTimesIndividualPrice[1].sign ^ 1;
        // demandTimesIndividualPrice[2].sign = demandTimesIndividualPrice[2].sign ^ 1;


        // doubleNumber numeratorShareOne;
        // double64_t t_numeratorShareOne = f64_add(individualSupplyTimestotalPrice[0].sign, individualSupplyTimestotalPrice[0].exp, individualSupplyTimestotalPrice[0].sig, demandTimesIndividualPrice[0].sign, demandTimesIndividualPrice[0].exp, demandTimesIndividualPrice[0].sig, numeratorShareOne.sign, numeratorShareOne.exp, numeratorShareOne.sig, packingProcess);


        // doubleNumber numeratorShareTwo;
        // double64_t t_numeratorShareTwo = f64_add(individualSupplyTimestotalPrice[1].sign, individualSupplyTimestotalPrice[1].exp, individualSupplyTimestotalPrice[1].sig, demandTimesIndividualPrice[1].sign, demandTimesIndividualPrice[1].exp, demandTimesIndividualPrice[1].sig, numeratorShareTwo.sign, numeratorShareTwo.exp, numeratorShareTwo.sig, packingProcess);

        // doubleNumber numeratorShareThree;
        // double64_t t_numeratorShareThree = f64_add(individualSupplyTimestotalPrice[2].sign, individualSupplyTimestotalPrice[2].exp, individualSupplyTimestotalPrice[2].sig, demandTimesIndividualPrice[2].sign, demandTimesIndividualPrice[2].exp, demandTimesIndividualPrice[2].sig, numeratorShareThree.sign, numeratorShareThree.exp, numeratorShareThree.sig, packingProcess);

        //  double numeratorShareOneFP = individualSupplyTimestotalPriceFP[0] - demandTimesIndividualPriceFP[0];

        // double numeratorShareTwoFP = individualSupplyTimestotalPriceFP[1] - demandTimesIndividualPriceFP[1];

        // double numeratorShareThreeFP = individualSupplyTimestotalPriceFP[2] - demandTimesIndividualPriceFP[2];


        // //Division


        // doubleNumber finalShare;
        // double64_t t_finalShare = MultiPartyDivisionProtocol(numeratorShareOne, numeratorShareTwo, numeratorShareThree, totalPriceShares[0], totalPriceShares[1], totalPriceShares[2], finalShare.sign, finalShare.exp, finalShare.sig);


        // double finalShareFP;
        // double t_finalShareFP = MultiPartyDivisionProtocol(numeratorShareOneFP, numeratorShareTwoFP, numeratorShareThreeFP, totalPriceShares_double[0], totalPriceShares_double[1], totalPriceShares_double[2], finalShareFP);


        // // Reconstruction and Final Price Computation, DONE by Suppliers

        // double t_generationErrorForSupplier = rePackForTesting(finalShare);

        // double changeInPrice = t_constantTerm[i] * t_generationErrorForSupplier;

        // double t_generationErrorForSupplierFP = (finalShareFP);

        // double changeInPriceFP = t_constantTerm[i] * t_generationErrorForSupplierFP;


        // out_supplierPrice.push_back(in_supplierPrice[i] - changeInPrice);

    }

    return out_supplierPrice;
}


int main()
{


    std::cout << std::setprecision(400);

    // From supplier side
    vector<double> t_sDotLambdaZero;

    double t_sDotLambdaZeroConsumer;

    vector<double> t_totalSupplyFordouble;
    vector<double> t_totalSupplyForMPCdouble;
    
    vector<double> t_doubleSupplierPrice;
    vector<double> t_supplierPriceforMPC;


    // From Consumer side
    vector<double> t_wDotLambdaZero;

    vector<double> t_individualDemandFordouble;
    vector<double> t_individualDemandForMPC;

    vector<double> t_consumerPricesUSingdoubleingPoint;
    vector<double> t_doubleConsumerPrices;


    for(int i=0;i<numsuppliers;i++)
    {
        t_supplierPriceforMPC.push_back(LAMBDA);
        t_doubleSupplierPrice.push_back(LAMBDA);
    }

    double t_totalWlambdaZeroForSupplier = 0;

    for(int i=0;i<numconsumers;i++)
    {
        t_consumerPricesUSingdoubleingPoint.push_back(LAMBDA);
        t_doubleConsumerPrices.push_back(LAMBDA);

        double wDotLambdaZero = d[i%10]*eph[i%10]*(pow(LAMBDA ,eph[i%10]-1));

        t_wDotLambdaZero.push_back(wDotLambdaZero);
        t_totalWlambdaZeroForSupplier = t_totalWlambdaZeroForSupplier + wDotLambdaZero;
    }

    double p = (0.57 * 152.0 * numconsumers)/2800000;
    double q = (0.57 * 4503.0 * numconsumers)/2800000;


    //compute individual p and q

    vector<double> t_individualSupplierP;
    vector<double> t_individualSupplierQ;

    divideSupplyValuesIntoShares(p, t_individualSupplierP, 1);
    divideSupplyValuesIntoShares(q, t_individualSupplierQ, 2);

    t_sDotLambdaZeroConsumer = p / (double)numconsumers;

    t_totalWlambdaZeroForSupplier = t_totalWlambdaZeroForSupplier / (double)numsuppliers; 
    vector<double> t_wDotLambdaZeroSharesForSupplier;

    vector<vector<double>> t_allConsumerPricesOriginal;
    vector<vector<double>> t_allConsumerPricesdouble;

    vector<vector<double>> t_supplierPriceOriginal;
    vector<vector<double>> t_supplierPriceUsingMPC;

    vector<vector<double>> t_SchedulingErrorOfIndividualConsumer;
    t_SchedulingErrorOfIndividualConsumer.clear();

    vector<double> t_overallGenerationSchedulingError; 
    t_overallGenerationSchedulingError.clear();
    

    vector<double> t_overallGenerationSchedulingError_org; 
    t_overallGenerationSchedulingError_org.clear();

    int counter = 0;


    // cout << "Starting While loop.................................." <<endl;

    // vector<double> t_overallGenerationSchedulingError;


    double randomDemandIntroduce = bk + d[1]*pow(LAMBDA,eph[4]);

    while(1)
    {
        // cout << endl;

        counter++;

        cout << "Consumers price original (without MPC): " <<endl;

        vector<double> t_schedulingErrorOfEachIteration;
        t_individualDemandFordouble.clear();

        for(int i=0;i<numconsumers;i++)
        {
            if(i < 10)
            {
                cout << t_doubleConsumerPrices[i] << ",";
            }

            double d_k = bk + (d[i%10]*pow(t_doubleConsumerPrices[i],eph[i%10]));

            if(counter % 10 == 0)
            {
                if(i % 10 == 0)
                {
                    d_k = d_k + randomDemandIntroduce;
                }
            }


            t_individualDemandFordouble.push_back(d_k);
        }
        cout <<endl;

        t_allConsumerPricesOriginal.push_back(t_doubleConsumerPrices);
    
        cout << "Consumers price calculated using doubleing point MPC:    " <<endl;

        t_individualDemandForMPC.clear();

        for(int i=0;i<numconsumers;i++)
        {
            if(i < 10)
            {
                cout << t_consumerPricesUSingdoubleingPoint[i] << ",";
            }

            double d_k = bk + (d[i%10]*pow(t_consumerPricesUSingdoubleingPoint[i],eph[i%10]));


            if(counter % 10 == 0)
            {
                if(i % 10 == 0)
                {
                    d_k = d_k + randomDemandIntroduce;
                }
            }


            t_individualDemandForMPC.push_back(d_k);


            // {

            //     cout << i << "::::" << t_consumerPricesUSingdoubleingPoint[i] << "::::" << t_individualDemandForMPC[i] <<endl;

            // }

        }

        cout <<endl;

        t_allConsumerPricesdouble.push_back(t_consumerPricesUSingdoubleingPoint);


        // supplier starts here

        cout << "Suppliers price original (without MPC):                 " << endl;


        t_totalSupplyFordouble.clear();

        for(int i=0;i<numsuppliers;i++)
        {
            cout << t_doubleSupplierPrice[i]  << ",";

            double s_i = t_individualSupplierP[i] * t_doubleSupplierPrice[i] + t_individualSupplierQ[i];

            t_totalSupplyFordouble.push_back(s_i);
        }
        cout <<endl;

        t_supplierPriceOriginal.push_back(t_doubleSupplierPrice);


        cout << "Suppliers price calculated using doubleing point MPC:    "  << endl;


        t_totalSupplyForMPCdouble.clear();

        for(int i=0;i<numsuppliers;i++)
        {
            cout << t_supplierPriceforMPC[i] << ",";

            double s_i = t_individualSupplierP[i] * t_supplierPriceforMPC[i] + t_individualSupplierQ[i];

            t_totalSupplyForMPCdouble.push_back(s_i);
        }
        cout <<endl;

        t_supplierPriceUsingMPC.push_back(t_supplierPriceforMPC);


        // cout << endl;

        double t_totalDemandFordouble = 0;

        for(int i=0;i<t_individualDemandFordouble.size();i++)
        {
            t_totalDemandFordouble = t_totalDemandFordouble + t_individualDemandFordouble[i];
        }


        double t_totalSupplyFordoubleWithoutMPC = 0;

        for(int i=0;i<t_totalSupplyFordouble.size();i++)
        {
            t_totalSupplyFordoubleWithoutMPC = t_totalSupplyFordoubleWithoutMPC + t_totalSupplyFordouble[i];
        }


        double generationSchedulingError = abs(t_totalDemandFordouble - t_totalSupplyFordoubleWithoutMPC);

        
        double t_totalDemandForMPC = 0;

        for(int i=0;i<t_individualDemandForMPC.size();i++)
        {
            t_totalDemandForMPC = t_totalDemandForMPC + t_individualDemandForMPC[i];
        }


        double t_totalSupplyFordoubleWithMPC = 0;

        for(int i=0;i<t_totalSupplyForMPCdouble.size();i++)
        {
            t_totalSupplyFordoubleWithMPC = t_totalSupplyFordoubleWithMPC + t_totalSupplyForMPCdouble[i];
        }

        double generationSchedulingError1 = abs(t_totalDemandForMPC - t_totalSupplyFordoubleWithMPC);

        t_overallGenerationSchedulingError.push_back(generationSchedulingError1);

        t_overallGenerationSchedulingError_org.push_back(generationSchedulingError);


        


        cout << "Generation Scheduling Error For without MPC                   : " << generationSchedulingError <<endl;
        cout << "Generation Scheduling Error For MPC using 64-bit doubleing point    : " << generationSchedulingError1<<endl;

    

        cout << "Error For without MPC (Generation Error / total Demand)            : " << (generationSchedulingError/t_totalDemandFordouble)*100 <<endl;
        cout << "Error For with MPC (Generation Error / total Demand)               : " << (generationSchedulingError1/t_totalDemandForMPC)*100  <<endl;

        cout << endl <<endl <<endl;



        if(counter > 100)
        {
            break;
        }

    
        // NON MPC

        computeConsumerPricingSingaldouble(t_doubleConsumerPrices, t_individualDemandFordouble, t_wDotLambdaZero, t_totalSupplyFordoubleWithoutMPC, t_sDotLambdaZeroConsumer, t_doubleConsumerPrices,t_schedulingErrorOfEachIteration, counter);

        computeSupplierPricingSingaldouble(t_doubleSupplierPrice, t_totalSupplyFordouble, t_individualSupplierP, t_totalDemandFordouble, t_totalWlambdaZeroForSupplier, t_doubleSupplierPrice);


        t_SchedulingErrorOfIndividualConsumer.push_back(t_schedulingErrorOfEachIteration);


        //MPC

        // cout << "Started Computing Consumers' Prices ......................" <<endl;

        // for(int i=0;i<numconsumers;i++)
        // {
        //     cout << t_consumerPricesUSingdoubleingPoint[i]    << ",";
        // }
        // cout <<endl;

        auto start = std::chrono::high_resolution_clock::now();


        vector<double> out_consumerPricesUSingdoubleingPoint = computeConsumerPricingSingalUsingdoubleingpoint(t_consumerPricesUSingdoubleingPoint, t_individualDemandForMPC, t_totalSupplyForMPCdouble, t_sDotLambdaZeroConsumer, t_wDotLambdaZero, counter);


        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        // Output the execution time in milliseconds
        std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;



        t_consumerPricesUSingdoubleingPoint.clear();
        t_consumerPricesUSingdoubleingPoint = out_consumerPricesUSingdoubleingPoint;



        // cout << "Started Computing Suppliers' Prices ......................" <<endl;

        vector<double> out_supplierPriceforMPC = computeSupplierPricingSingal(t_supplierPriceforMPC, t_totalSupplyForMPCdouble, t_individualDemandForMPC, t_totalWlambdaZeroForSupplier, t_individualSupplierP);


        t_supplierPriceforMPC.clear();
        t_supplierPriceforMPC = out_supplierPriceforMPC;

    }


    cout << "--------------------------------------------------------------------Stability Achieved--------------------------------------------------------------------------------------" << endl <<endl;



    cout << "--------------------------------------------------------------------Overall Generation Scheduling Error---------------------------------------------------------------------" <<endl <<endl;


    for (int i = 0; i < t_overallGenerationSchedulingError.size(); ++i)
    {
        cout << t_overallGenerationSchedulingError[i] << ",";
        
    }

    cout <<endl <<endl;

    


    for (int i = 0; i < t_overallGenerationSchedulingError_org.size(); ++i)
    {
        cout << t_overallGenerationSchedulingError_org [i] << ",";
        
    }

    cout <<endl <<endl;

    cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl;

    cout << "--------------------------------------------------------------------Individual Generation Scheduling Error---------------------------------------------------------------------" <<endl;

    int iteratioNumber = counter - 1;

    for (int i = 0; i < 10; ++i)
    {
        cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl;


        cout << "Consumer ID: " << i <<endl;

        string t_value = "";

        // cout << "t_SchedulingErrorOfIndividualConsumer[i].size():    " << t_SchedulingErrorOfIndividualConsumer[i].size() <<endl;


        for(int j = 0 ; j < iteratioNumber;j++)
        {
            cout << t_SchedulingErrorOfIndividualConsumer[j][i] << ",";

            t_value = t_value + to_string(abs(t_SchedulingErrorOfIndividualConsumer[j][i]))  + ",";

        }

        cout <<endl;

        cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------" <<endl;
        

        t_value.erase(t_value.size() - 1);


        // if(i < 10)
        // {

        //  string t_tempSystemCall = "/usr/bin/python3 plotGrpah.py " + to_string(i+1) + " " + t_value + " " + to_string(iteratioNumber);
        //  int result = system(t_tempSystemCall.c_str());

        //  cout <<endl;
        // }
        
    }

    cout << "-----------------------------------------------------------------------------ERROR PERCENTAGE-----------------------------------------------------------------------------" <<endl;


    vector<vector<double>> t_allConsumerPricesErrorAverage;

    std::vector<string> t_allConsumerPricesErrorAverageString;

    vector<double> t_vectorOfConsumersMaxErrors;


    for (int i = 0; i < numconsumers; ++i)
    {
        
        if(i < 10)
        {
            cout << endl <<endl <<endl;
            cout << "Consumer ID:   " << i <<endl;
        }

        string t_value = "";

        vector<double> t_allConsumerPricesErrorAveragePer;

        t_allConsumerPricesErrorAveragePer.clear();

        // for(int j = 0 ; j < t_allConsumerPricesdouble.size();j++)
        for(int j = 0 ; j < iteratioNumber;j++)
        {
            // cout << (abs(t_allConsumerPricesdouble[j][i] - t_allConsumerPricesOriginal[j][i])/t_allConsumerPricesOriginal[j][i])  * 100<< ",";

            double temp_____ = (((abs(t_allConsumerPricesdouble[j][i] - t_allConsumerPricesOriginal[j][i])/t_allConsumerPricesOriginal[j][i])  * 100)) ;

            if(i < 10)
            {
                if(j == (iteratioNumber - 1))
                {
                    cout << temp_____ << endl;
                }
                else
                {
                    cout << temp_____ << ",";
                }
            }

            t_value = t_value + to_string(((abs(t_allConsumerPricesdouble[j][i] - t_allConsumerPricesOriginal[j][i])/t_allConsumerPricesOriginal[j][i])  * 100)) + ",";
            t_allConsumerPricesErrorAveragePer.push_back(temp_____);
        
        }

        // cout <<endl;


        t_vectorOfConsumersMaxErrors.push_back(*max_element(t_allConsumerPricesErrorAveragePer.begin(), t_allConsumerPricesErrorAveragePer.end()));

        if(i < 10)
        {
            cout << "max error: "<< double(*max_element(t_allConsumerPricesErrorAveragePer.begin(), t_allConsumerPricesErrorAveragePer.end())) << endl;

            // cout << "min error: "<< double(*min_element(t_allConsumerPricesErrorAveragePer.begin(), t_allConsumerPricesErrorAveragePer.end())) << endl;
        }

        t_allConsumerPricesErrorAverage.push_back(t_allConsumerPricesErrorAveragePer);

        t_value.erase(t_value.size() - 1);

        t_allConsumerPricesErrorAverageString.push_back(t_value);

        // cout << t_value <<endl;
        // cout << "t_allConsumerPricesdouble.size():    " << t_allConsumerPricesdouble.size() <<endl;

        if(i < 10)
        {

            string t_tempSystemCall = "/usr/bin/python3 plotGrpah.py " + to_string(i+1) + " " + t_value + " " + to_string(iteratioNumber);
            // int result = system(t_tempSystemCall.c_str());

            cout <<endl;
        }

    }


        vector<vector<double>> t_allSupplierPricesErrorAverage;

    std::vector<string> t_allSupplierPricesErrorAverageString;

    vector<double> t_vectorOfSuppliersMaxErrors;

    for (int i = 0; i < numsuppliers; ++i)
    {

        // cout << endl <<endl <<endl;
        // cout << "Supplier ID:    " << i <<endl;

        string t_value = "";

        vector<double> t_allSupplierPricesErrorAveragePer; 

        t_allSupplierPricesErrorAveragePer.clear();

        // for(int j = 0 ; j < t_allConsumerPricesdouble.size();j++)
        for(int j = 0 ; j < iteratioNumber;j++)
        {
            // cout << (abs(t_allConsumerPricesdouble[j][i] - t_allConsumerPricesOriginal[j][i])/t_allConsumerPricesOriginal[j][i])  * 100<< ",";

            double temp_____ = (((abs(t_supplierPriceUsingMPC[j][i] - t_supplierPriceOriginal[j][i])/t_supplierPriceOriginal[j][i])  * 100)) ;

            if(i < 10)
            {
                // cout << temp_____ << ",";
            }

            t_value = t_value + to_string(((abs(t_supplierPriceUsingMPC[j][i] - t_supplierPriceOriginal[j][i])/t_supplierPriceOriginal[j][i])  * 100)) + ",";
            t_allSupplierPricesErrorAveragePer.push_back(temp_____);
        
        }

        t_vectorOfSuppliersMaxErrors.push_back(*max_element(t_allSupplierPricesErrorAveragePer.begin(), t_allSupplierPricesErrorAveragePer.end()));

  
        // cout << "max error: "<< double(*max_element(t_allSupplierPricesErrorAveragePer.begin(), t_allSupplierPricesErrorAveragePer.end())) << endl;

        // cout << "min error: "<< double(*min_element(t_allSupplierPricesErrorAveragePer.begin(), t_allSupplierPricesErrorAveragePer.end())) << endl;


        t_allSupplierPricesErrorAverage.push_back(t_allSupplierPricesErrorAveragePer);

        t_value.erase(t_value.size() - 1);

        t_allSupplierPricesErrorAverageString.push_back(t_value);

        // cout << t_value <<endl;
        // cout << "t_supplierPriceUsingMPC.size(): " << t_supplierPriceUsingMPC.size() <<endl;

        string t_tempSystemCall = "/usr/bin/python3 plotGrpahSupplier.py " + to_string(i+1) + " " + t_value + " " + to_string(iteratioNumber);
        // int result = system(t_tempSystemCall.c_str());

        // cout <<endl;
    }


    cout << "MAX ERRORS OF THE CONSUMERS-----------------------------------------------------------------------------" <<endl <<endl;


    for(int i = 0 ; i < t_vectorOfConsumersMaxErrors.size();i++)
    {
        cout << t_vectorOfConsumersMaxErrors[i] << ",";

    }

    cout << endl <<endl;


    cout << "MAX ERRORS OF THE Suppliers-----------------------------------------------------------------------------" <<endl <<endl;


    for(int i = 0 ; i < t_vectorOfSuppliersMaxErrors.size();i++)
    {
        cout << t_vectorOfSuppliersMaxErrors[i] << ",";

    }

    cout << endl <<endl;



    /*----------------------------Average-------------------------------------------------*/


    // cout << "before average..............." <<endl;


    // for(int i = 0 ; i < t_allConsumerPricesErrorAverageString.size();i++)
    // {
    //  cout << t_allConsumerPricesErrorAverageString[i];

    //  cout << endl;

    // }



    cout << "Average Errors of the Consumers-----------------------------------------------------------------------------" <<endl <<endl;


    vector<double> t_allConsumerPricesErrorAveragePerFinal;

    string t_valueConAverage = "";


    // for(int j = 0 ; j < t_allConsumerPricesErrorAverage[0].size();j++)
    for(int j = 0 ; j < iteratioNumber;j++)
    {

        double temp_aaa = 0;
        for(int i = 0 ; i < t_allConsumerPricesErrorAverage.size(); i++)
        {

            temp_aaa = temp_aaa + t_allConsumerPricesErrorAverage[i][j];

        }

        temp_aaa = (double)(temp_aaa/(double)(numconsumers));



        cout << temp_aaa << ",";



        t_valueConAverage = t_valueConAverage + to_string(temp_aaa)  + ",";

        t_allConsumerPricesErrorAveragePerFinal.push_back(temp_aaa);

    }

    cout <<endl <<endl;

        cout << "Average Max Error: "<< double(*max_element(t_allConsumerPricesErrorAveragePerFinal.begin(), t_allConsumerPricesErrorAveragePerFinal.end())) << endl <<endl;

        // cout << "min error: "<< double(*min_element(t_allConsumerPricesErrorAveragePerFinal.begin(), t_allConsumerPricesErrorAveragePerFinal.end())) << endl;

    t_valueConAverage.erase(t_valueConAverage.size() - 1);

    // cout << t_valueConAverage <<endl;

    string t_tempSystemCallAvrg= "/usr/bin/python3 plotGrpahAvgConsumer.py " + to_string(1) + " " + t_valueConAverage + " " + to_string(iteratioNumber);
    // int resultAvrg = system(t_tempSystemCallAvrg.c_str());


/*----------------------------------------------------------------------------------------------------------------*/

    cout << "Average Errors of the Suppliers-----------------------------------------------------------------------------" <<endl <<endl;


    vector<double> t_allSupplierPricesErrorAveragePerFinal;

    string t_valueConAverage1 = "";


    // for(int j = 0 ; j < t_allConsumerPricesErrorAverage[0].size();j++)
    for(int j = 0 ; j < iteratioNumber;j++)
    {

        double temp_aaa = 0;
        for(int i = 0 ; i < t_allSupplierPricesErrorAverage.size(); i++)
        {

            temp_aaa = temp_aaa + t_allSupplierPricesErrorAverage[i][j];

        }

        temp_aaa = (double)(temp_aaa/(double)(numsuppliers));

        cout << temp_aaa << ",";

        t_valueConAverage1 = t_valueConAverage1 + to_string(temp_aaa)  + ",";

        t_allSupplierPricesErrorAveragePerFinal.push_back(temp_aaa);

    }

    cout <<endl <<endl;
          
        cout << "Average Max Error: "<< double(*max_element(t_allSupplierPricesErrorAveragePerFinal.begin(), t_allSupplierPricesErrorAveragePerFinal.end())) << endl <<endl;

        // cout << "min error: "<< double(*min_element(t_allSupplierPricesErrorAveragePerFinal.begin(), t_allSupplierPricesErrorAveragePerFinal.end())) << endl;

    t_valueConAverage1.erase(t_valueConAverage1.size() - 1);

    // cout << t_valueConAverage1 <<endl;

    string t_tempSystemCallAvrg1= "/usr/bin/python3 plotGrpahAvgSupplier.py " + to_string(1) + " " + t_valueConAverage1 + " " + to_string(iteratioNumber);
    // int resultAvrg1 = system(t_tempSystemCallAvrg1.c_str());







}



                                                                            /*  For Individual Funtional Testing Purpuses */


/*

int addmod(int in_a1, int in_a2, int in_mod)
{
    int temp = in_a1;

    if(in_a2 >= 0)
    {
        temp += in_a2;
    }
    else
    {
        temp += (in_mod + in_a2);
    }

    return temp % in_mod;
}

*/

// signed main()
// {

//     // double in_n = 10.123;

//     // obtainShares(in_n, 1);

//     // while(1);

//     // int exp = 128;

//     // int exp_1 = (rand() % 256);
//     // int exp_2 = (rand() % 256);

//     // cout << exp_1 <<endl; 
//     // cout << exp_2 <<endl; 
        

//     // int exp_3 = addmod(exp, -exp_1, 256);

//     // cout << exp_3 <<endl;

//     // exp_3 = addmod(exp_3, -exp_2, 256);

//     // cout << exp_3 <<endl;

//     // cout << (exp_1 + exp_2 + exp_3) % 256 <<endl;

//     // while(1);


//     // double value1  = -5.321;

//     // double value2 = 0.0488806;

//     // cout << "Original X1: " << value1 <<endl;

//     // cout << "Original X2: " << value2 <<endl;

//     // cout << "Multiplication Value: " << value1 * value2 <<endl;

//     // double64_t flt1, flt2;

//     // flt1 = *(double64_t*) &value1;
//     // flt2 = *(double64_t*) &value2;

//     // // double64_t f32mul =  f32_mul(flt1, flt2);
//     // // double output = *(double*) &f32mul; 

//     // // printf("MUL: Computed output is \t%.15f\n\n", output);

//     // double outputOrg = value1 * value2;

//     // // printf("MUL: double output \t%.15f\n\n", outputOrg);


//     // union ui64_f64 uA;
//     // uint_fast32_t uiA;
//     // bool signA;
//     // int_fast16_t expA;
//     // uint_fast32_t sigA;
   
//     // union ui64_f64 uB;
//     // uint_fast32_t uiB;
//     // bool signB;
//     // int_fast16_t expB;
//     // uint_fast32_t sigB;
    
//     // bool signZ;
//     // int_fast16_t expZ;
//     // uint_fast32_t sigZ;

//     // //------------------------------------------------------------------------
//     // //------------------------------------------------------------------------
//     // uA.f = flt1;
//     // uiA = uA.ui;
//     // signA = signF32UI( uiA );
//     // expA  = expF32UI( uiA );
//     // sigA  = fracF32UI( uiA );
//     // uB.f = flt2;
//     // uiB = uB.ui;
//     // signB = signF32UI( uiB );
//     // expB  = expF32UI( uiB );
//     // sigB  = fracF32UI( uiB );


//     // double64_t f32mul = f32_mul(signA, expA, sigA, signB, expB, sigB, signZ, expZ, sigZ);

//     // double output = *(double*) &f32mul; 

//     // // printf("MUL: Computed output is \t%.15f\n\n", output);

//     // double64_t f32mul_reconstruct = sdouble_roundPackToF32(signZ, expZ, sigZ);

//     // double output1 = *(double*) &f32mul_reconstruct; 

//     // // printf("MUL: Computed output after reconstruction is \t%.15f\n\n", output1);

//     // doubleNumber share1, share2, share3;

//     // double64_t f32mul11 = CrossMultiplicationUsingDuatallahProtocol(signA, expA, sigA, signB, expB, sigB, signZ, expZ, sigZ, share1, share2, share3);


//     // // while(1);


//     // cout << "---------------------------------------------------- CrossMultiplicationUsingDuatallahProtocol Testing Successful ---------------------------------------------------------------" <<endl;


//     doubleNumber x1, x2, x3, y1, y2, y3;

//     // double value11  = 10.123;

//     //     union ui64_f64 uA;
//     // uint_fast32_t uiA;
//     // bool signA;
//     // int_fast16_t expA;
//     // uint_fast32_t sigA;

//     // uA.f = *(double64_t*) &value11;
//     // uiA = uA.ui;
//     // signA = signF32UI(uiA);
//     // expA  = expF32UI(uiA);
//     // sigA  = fracF32UI(uiA);



//     // cout << "-----------------------------------------------------------------------------------" <<endl;

//     // vector<doubleNumber> Shares1 =  obtainShares(value11, 1);



//     // double value11_1 = -140.3312;
//     // double value11_2 = 20.458;
//     // double value11_3 = value11 - value11_1 - value11_2;

//     // double value11_1 = 1364799782912.00;
//     // double value11_2 = -2729641246720.00;
//     // double value11_3 = 1364841070592.00;

//     double X1,X2,X3,Y1,Y2,Y3;
//     X1= 1388374013550413568.000000000000000;
//     X2= -9944710713059887104.000000000000000;
//     X3= 8556336699509474304.000000000000000;
//     Y1= 469681921.960737049579620;
//     Y2= 1132970178.522562026977539;
//     Y3= -1602651890.483299016952515;

//     double value11_1=1;
//     double value11_2=X2;
//     double value11_3=X3;
//     // double value11_1 = -10745667463791482880.000000000000000;
//     // double value11_2 = 27731062891174039552.000000000000000;
//     // double value11_3 = -16985395427382560768.000000000000000;
//     //X1: -10745667463791482880.000000000000000
//     //X2: 27731062891174039552.000000000000000
//     //X3: -16985395427382560768.000000000000000
    


//     double64_t flt1_1 = *(double64_t*) &value11_1;

//     union ui64_f64 uA_value11_1;
//     uint_fast64_t uiA_value11_1;
//     uA_value11_1.f = flt1_1;
//     uiA_value11_1 = uA_value11_1.ui;

//     x1.sign = signF64UI( uiA_value11_1 );
//     x1.exp  = expF64UI( uiA_value11_1 );
//     x1.sig  = fracF64UI( uiA_value11_1 );

//     // cout << "Original Secret x: " << value11_1 <<endl;
//     // cout <<  x1.sign <<endl;
//     // cout <<  x1.exp <<endl;
//     // cout <<  x1.sig <<endl;


//     double64_t flt1_2 = *(double64_t*) &value11_2;

//     union ui64_f64 uA_value11_2;
//     uint_fast64_t uiA_value11_2;
//     uA_value11_2.f = flt1_2;
//     uiA_value11_2 = uA_value11_2.ui;

//     x2.sign = signF64UI( uiA_value11_2 );
//     x2.exp  = expF64UI( uiA_value11_2 );
//     x2.sig  = fracF64UI( uiA_value11_2 );


//     double64_t flt1_3 = *(double64_t*) &value11_3;

//     union ui64_f64 uA_value11_3;
//     uint_fast64_t uiA_value11_3;
//     uA_value11_3.f = flt1_3;
//     uiA_value11_3 = uA_value11_3.ui;

//     x3.sign = signF64UI( uiA_value11_3 );
//     x3.exp  = expF64UI( uiA_value11_3 );
//     x3.sig  = fracF64UI( uiA_value11_3 );


//     // double value22 = 23.567;

//     // union ui64_f64 uA1;
//     // uint_fast32_t uiA1;
//     // bool signA1;
//     // int_fast16_t expA1;
//     // uint_fast32_t sigA1;

//     // uA1.f = *(double64_t*) &value22;
//     // uiA1 = uA1.ui;
//     // signA1 = signF32UI(uiA1);
//     // expA1  = expF32UI(uiA1);
//     // sigA1  = fracF32UI(uiA1);

//     // cout << "Original Secret y: " << value22 <<endl;
//     // cout << signA1 <<endl;
//     // cout << expA1 <<endl;
//     // cout << sigA1 <<endl;


//     // cout << "-----------------------------------------------------------------------------------" <<endl;

//     // vector<doubleNumber> Shares2 =  obtainShares(value22, 2);

//     // double value22_1 = 13.74;
//     // double value22_2 = -50.321;
//     // double value22_3 = value22 - value22_1 - value22_2;

//     double value22_1=100;
//     double value22_2=Y2;
//     double value22_3=Y3;

//     // double value22_1 = 2504004560.725543975830078;
//     // double value22_2 = 1031793031.059835433959961;
//     // double value22_3 = -3535797381.785379886627197; 

//     // Y1: 2504004560.725543975830078
//     // Y2: 1031793031.059835433959961
//     // Y3: -3535797381.785379886627197 

//     // double value22_1 = 100;
//     // double value22_2 = -50;
//     // double value22_3 = 50;  


//     double64_t flt1_4 = *(double64_t*) &value22_1;

//     union ui64_f64 uA_value22_1;
//     uint_fast64_t uiA_value22_1;
//     uA_value22_1.f = flt1_4;
//     uiA_value22_1 = uA_value22_1.ui;

//     y1.sign = signF64UI( uiA_value22_1 );
//     y1.exp  = expF64UI( uiA_value22_1 );
//     y1.sig  = fracF64UI( uiA_value22_1 );


//     double64_t flt1_5 = *(double64_t*) &value22_2;

//     union ui64_f64 uA_value22_2;
//     uint_fast64_t uiA_value22_2;
//     uA_value22_2.f = flt1_5;
//     uiA_value22_2 = uA_value22_2.ui;

//     y2.sign = signF64UI( uiA_value22_2 );
//     y2.exp  = expF64UI( uiA_value22_2 );
//     y2.sig  = fracF64UI( uiA_value22_2 );


//     double64_t flt1_6 = *(double64_t*) &value22_3;

//     union ui64_f64 uA_value22_3;
//     uint_fast64_t uiA_value22_3;
//     uA_value22_3.f = flt1_6;
//     uiA_value22_3 = uA_value22_3.ui;

//     y3.sign = signF64UI( uiA_value22_3 );
//     y3.exp  = expF64UI( uiA_value22_3 );
//     y3.sig  = fracF64UI( uiA_value22_3 );

//     // bool MPCDIV_sign;
//     // int_fast16_t MPCDIV_exp;
//     // uint_fast32_t MPCDIV_sig;



//     // bool signA2 = 0;
//     // int_fast16_t expA2 = (expA + expA1) % 256;

//     // long long int modValue_ = pow(2,23);

//     // uint_fast32_t sigA2 = (sigA + sigA1) % modValue_;


//     // cout << "Expected output:" << rePackForTesting(signA2, expA2, sigA2) <<endl;

//     // doubleNumber f32_addOutput;
//     // int packingProcess = 1;

//     // double64_t t_temp_add = f32_add(signA, expA, sigA, signA1, expA1, sigA1, f32_addOutput.sign, f32_addOutput.exp, f32_addOutput.sig, packingProcess);

//     // cout << "Output using f32_add directly on x+y:" << rePackForTesting(f32_addOutput) <<endl;

//     // bool signA_10000000000000000 = 0; int_fast16_t expA_10000000000000000 = 180; uint_fast64_t sigA_10000000000000000 = 924618;

//     cout << "---------------------" <<endl;

//    //  double scaleValue = 10000000000000000;

//    //  double64_t flt1_7 = *(double64_t*) &scaleValue;

//    //  union ui64_f64 uA_value22_31;
//    //  uint_fast64_t uiA_value22_31;
//    //  uA_value22_31.f = flt1_7;
//    //  uiA_value22_31 = uA_value22_31.ui;

//    // bool signA_10000000000000000 = signF64UI( uiA_value22_31 );
//    // int_fast16_t expA_10000000000000000  = expF64UI( uiA_value22_31 );
//    // uint_fast64_t sigA_10000000000000000  = fracF64UI( uiA_value22_31 );

//    //  cout << signA_10000000000000000 <<endl;

//    //  cout << expA_10000000000000000 <<endl;

//    //  cout << sigA_10000000000000000 <<endl;

//    //  doubleNumber output64_div;

//    //  double64_t x1_temp = f64_div(x1.sign, x1.exp, x1.sig, signA_10000000000000000, expA_10000000000000000, sigA_10000000000000000, output64_div.sign, output64_div.exp, output64_div.sig);

//    //  double output = *(double*) &x1_temp; 

//    //  printf("MUL: Computed output is \t%.15f\n\n", output);


//    //  cout << "output64_div.sign: " << output64_div.sign <<endl;

//    //  cout << "output64_div.exp: " << output64_div.exp <<endl;

//    //  cout << "output64_div.sig: " << output64_div.sig <<endl;

//    //  cout << "After Scaling x1: " << rePackForTesting(output64_div) <<endl;

//    //  while(1); 1    1047    1827453555179520    1    1024    3745835408794285



//     // bool signA = 1 ;
//     // int_fast16_t expA = 1047;
//     // uint_fast32_t sigA = 1827453555179520;

//     // bool signB = 0  ;
//     // int_fast16_t expB = 1024;
//     // uint_fast32_t sigB = 3745835408794285;

//     // bool signA = 1 ;
//     // int_fast16_t expA = 1026;
//     // uint_fast32_t sigA = 1125899906842624;

//     // bool signB = 0 ;
//     // int_fast16_t expB = 1029;
//     // uint_fast32_t sigB = 2533274790395904;

//     // double input1 = rePackForTesting(signA, expA, sigA); 

//     // cout << "input1: " << input1 <<endl;



//     // double input2 = rePackForTesting(signB, expB, sigB); 

//     // cout << "input2: " << input2 <<endl;

//     // cout << "Expected: " << input1 * input2 <<endl;


//     // doubleNumber share1, share2, share3, output_add;

//     // int packingProcess = -1;

//     // double64_t f32mul11 = CrossMultiplicationUsingDuatallahProtocol(signA, expA, sigA, signB, expB, sigB, output_add.sign, output_add.exp, output_add.sig, share1, share2, share3);

//     // double output2 = rePackForTesting(output_add); 

//     for(int i=0;i<10;i++){
//         value22_1=rand();   
//         flt1_4 = *(double64_t*) &value22_1;
//         uA_value22_1.f = flt1_4;
//         uiA_value22_1 = uA_value22_1.ui;

//         y1.sign = signF64UI( uiA_value22_1 );
//         y1.exp  = expF64UI( uiA_value22_1 );
//         y1.sig  = fracF64UI( uiA_value22_1 );

//         doubleNumber output_add;
//         double f64division_doubleing = value11_1/value22_1;
//         double f64multiplication_doubleing = f64division_doubleing*value22_1;
//         double64_t f64division_MPC = f64_div(x1.sign, x1.exp, x1.sig,  y1.sign, y1.exp, y1.sig, y3.sign, y3.exp,y3.sig);
//         double64_t f64multiplication_MPC = f64_mul(y1.sign, y1.exp, y1.sig, y3.sign, y3.exp,y3.sig, x2.sign, x2.exp, x2.sig);
//         // double64_t div_output1 = MultiPartyDivisionProtocol(x1, x2, x3, y1, y2, y3, output_add.sign, output_add.exp, output_add.sig);

//         double output2 = rePackForTesting(y3); 

//         double output3 = rePackForTesting(x2);
//         double org_output = (value11_1 + value11_2 + value11_3) / (value22_1 + value22_2 + value22_3)  ;

//         // cout << "Output Should be: " << org_output <<endl;

//         // printf("MPC DIV: Computed output  is \t%.15f\n\n", output2);
//         double input1= rePackForTesting(x1);
//         double input2= rePackForTesting(y1);
//         cout<<"input1 and input2 are: "<< input1 <<" "<<input2<<endl;
//         cout<< "doubleing division output should be: "<<f64division_doubleing<<endl;
//         cout<< "Division in IEEE754 should be: "<<output2<<endl;
//         cout<< "doubleing multiplication should be: "<<f64multiplication_doubleing<<endl;
//         cout<<"multiplication in IEEE754 should be: " << output3<<endl;
//     }
    


//     // double64_t f32add =  f32_add(flt1, flt2);
//     // double output1 = *(double*) &f32add; 

//     // printf("Add: Computed output is \t%.15f\n\n", output1);

//     // double outputOrg1 = value1 + value2;

//     // printf("Add: double output \t%.15f\n\n", outputOrg1);


//     // double value11  = 10.123;

//     // double value22 = -20.567;

//     // double64_t flt11, flt22;

//     // flt11 = *(double64_t*) &value11;
//     // flt22 = *(double64_t*) &value22;


//     // double64_t f32sub =  f32_add(flt11, flt22);
//     // double output11 = *(double*) &f32sub; 

//     // printf("Sub: Computed output is \t%.15f\n\n", output11);

//     // double outputOrg11 = value11 + value22;

//     // printf("Sub: double output \t%.15f\n\n", outputOrg11);




//     // double64_t f32div =  f32_div(flt1, flt2);
//     // double output2 = *(double*) &f32div; 

//     // printf("Div: Computed output is \t%.15f\n\n", output2);

//     // double outputOrg2 = value1 / value2;

//     // printf("Div: double output \t%.15f\n\n", outputOrg2);


// }
