#include "MPCProtocol.hpp"
#include "FloatingPoint.hpp"
#include <vector>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <iostream>

using namespace std;

double MPCProtocol::maxInputValue = -std::numeric_limits<double>::infinity();


//Developed to get the maximum value of the system. 
double MPCProtocol::getMaxInputValue()
{
    return maxInputValue;
}


void MPCProtocol::ScalingOperands(int_fast16_t in_exp1, int_fast16_t in_exp2, int_fast16_t in_exp3, 
                                bool &sign_ScalingFactor, int_fast16_t &exp_ScalingFactor, 
                              uint_fast64_t &sig_ScalingFactor)
{
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


    double alpha1_ = 10;
    union ui64_f64 uAlpha1;
    uint_fast64_t uiAlpha1;
    bool signAlpha1;
    int_fast16_t expAlpha1;
    uint_fast64_t sigAlpha1;
    double64_t alp1 = *(double64_t*)&alpha1_;
    uAlpha1.f = alp1;
    uiAlpha1 = uAlpha1.ui;
    signAlpha1 = signF64UI(uiAlpha1);
    expAlpha1 = expF64UI(uiAlpha1);
    sigAlpha1 = fracF64UI(uiAlpha1);

    sign_ScalingFactor = 0;
    exp_ScalingFactor = 1023;
    sig_ScalingFactor = 0;

    int_fast16_t exp_max = max(in_exp1, max(in_exp2, in_exp3));

    for (int x = 1; expAlpha1 < 2000; x++)
    {
        alpha1_ = pow(10, x);
        alp1 = *(double64_t*)&alpha1_;
        uAlpha1.f = alp1;
        uiAlpha1 = uAlpha1.ui;
        signAlpha1 = signF64UI(uiAlpha1);
        expAlpha1 = expF64UI(uiAlpha1);
        sigAlpha1 = fracF64UI(uiAlpha1);

        if (expAlpha1 > exp_max)
        {
            sign_ScalingFactor = signAlpha1;
            exp_ScalingFactor = expAlpha1;
            sig_ScalingFactor = sigAlpha1;
            break;
        }
    }
}

double64_t MPCProtocol::MultiPartyAdditionProtocol( doubleNumber x1,  doubleNumber x2,  doubleNumber x3,
                                                doubleNumber y1,  doubleNumber y2,  doubleNumber y3,
                                               doubleNumber &out_totalSum)
{
    int packingProcess = -1;

    doubleNumber partyOneShare1;
    double64_t t_partyOneShare1 = FloatingPoint::f64_add(x1.sign, x1.exp, x1.sig, x2.sign, x2.exp, x2.sig,
                                                       partyOneShare1.sign, partyOneShare1.exp, partyOneShare1.sig, packingProcess);
    doubleNumber partyOneShare2;
    double64_t t_partyOneShare2 = FloatingPoint::f64_add(x3.sign, x3.exp, x3.sig, partyOneShare1.sign, partyOneShare1.exp, partyOneShare1.sig,
                                                       partyOneShare2.sign, partyOneShare2.exp, partyOneShare2.sig, packingProcess);
    doubleNumber partyOneShare3;
    double64_t t_partyOneShare3 = FloatingPoint::f64_add(y1.sign, y1.exp, y1.sig, partyOneShare2.sign, partyOneShare2.exp, partyOneShare2.sig,
                                                       partyOneShare3.sign, partyOneShare3.exp, partyOneShare3.sig, packingProcess);
    doubleNumber temp_add_1;
    double64_t t_temp_add_1 = FloatingPoint::f64_add(y2.sign, y2.exp, y2.sig, partyOneShare3.sign, partyOneShare3.exp, partyOneShare3.sig,
                                                    temp_add_1.sign, temp_add_1.exp, temp_add_1.sig, packingProcess);
    double64_t t_temp_add = FloatingPoint::f64_add(temp_add_1.sign, temp_add_1.exp, temp_add_1.sig, y3.sign, y3.exp, y3.sig,
                                                  out_totalSum.sign, out_totalSum.exp, out_totalSum.sig, packingProcess);
    return t_temp_add;
}

vector<doubleNumber> MPCProtocol::MultiPartyAdditionProtocolForFinalComputation(doubleNumber x1, doubleNumber y1,
                                                                              doubleNumber x2, doubleNumber y2,
                                                                              doubleNumber x3, doubleNumber y3,
                                                                              double &out_totalSum)
{
    int packingProcess = -1;

    doubleNumber partyOneShare1;
    double64_t t_partyOneShare1 = FloatingPoint::f64_add(x1.sign, x1.exp, x1.sig, y1.sign, y1.exp, y1.sig,
                                                       partyOneShare1.sign, partyOneShare1.exp, partyOneShare1.sig, packingProcess);
    doubleNumber partyOneShare2;
    double64_t t_partyOneShare2 = FloatingPoint::f64_add(x2.sign, x2.exp, x2.sig, y2.sign, y2.exp, y2.sig,
                                                       partyOneShare2.sign, partyOneShare2.exp, partyOneShare2.sig, packingProcess);
    doubleNumber partyOneShare3;
    double64_t t_partyOneShare3 = FloatingPoint::f64_add(x3.sign, x3.exp, x3.sig, y3.sign, y3.exp, y3.sig,
                                                       partyOneShare3.sign, partyOneShare3.exp, partyOneShare3.sig, packingProcess);

    vector<doubleNumber> finalShares;
    finalShares.push_back(partyOneShare1);
    finalShares.push_back(partyOneShare2);
    finalShares.push_back(partyOneShare3);

    out_totalSum = FloatingPoint::rePackForTesting(partyOneShare1) + FloatingPoint::rePackForTesting(partyOneShare2) + FloatingPoint::rePackForTesting(partyOneShare3);

    return finalShares;
}

double64_t MPCProtocol::CrossMultiplicationUsingDuatallahProtocol(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                                                                 bool signB, int_fast16_t expB, uint_fast64_t sigB,
                                                                 bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ,
                                                                 doubleNumber &Share1, doubleNumber &Share2, doubleNumber &Share3)
{
    double X1 = FloatingPoint::rePackForTesting(signA, expA, sigA);
    double sum1 = X1 + alpha1;
    double X2 = FloatingPoint::rePackForTesting(signB, expB, sigB);
    double sum2 = X2 + alpha2;

    double64_t alp1 = *(double64_t*)&alpha1;
    double64_t alp2 = *(double64_t*)&alpha2;

    union ui64_f64 uAlpha1, uAlpha2;
    uint_fast64_t uiAlpha1, uiAlpha2;
    bool signAlpha1, signAlpha2;
    int_fast16_t expAlpha1, expAlpha2;
    uint_fast64_t sigAlpha1, sigAlpha2;

    uAlpha1.f = alp1;
    uiAlpha1 = uAlpha1.ui;
    signAlpha1 = signF64UI(uiAlpha1);
    expAlpha1 = expF64UI(uiAlpha1);
    sigAlpha1 = fracF64UI(uiAlpha1);

    uAlpha2.f = alp2;
    uiAlpha2 = uAlpha2.ui;
    signAlpha2 = signF64UI(uiAlpha2);
    expAlpha2 = expF64UI(uiAlpha2);
    sigAlpha2 = fracF64UI(uiAlpha2);

    bool sign_Alpha1_Alpha2;
    int_fast16_t expA_Alpha1_Alpha2;
    uint_fast64_t sigA_Alpha1_Alpha2;

    double64_t t_Alpha1_Alpha2 = FloatingPoint::f64_mul(signAlpha1, expAlpha1, sigAlpha1, signAlpha2, expAlpha2, sigAlpha2,
                                                       sign_Alpha1_Alpha2, expA_Alpha1_Alpha2, sigA_Alpha1_Alpha2);
    double output_Alpha1_Alpha2 = FloatingPoint::rePackForTesting(sign_Alpha1_Alpha2, expA_Alpha1_Alpha2, sigA_Alpha1_Alpha2);

    double output_Alpha1_Alpha2_Expected = alpha1 * alpha2;
    doubleNumber t_output_Alpha1_Alpha2_Expected = FloatingPoint::getSignExpSig(output_Alpha1_Alpha2_Expected);

    int t_packingProcess = -1;

    bool sign_partyOne_X1_Alpha1;
    int_fast16_t expA_partyOne_X1_Alpha1;
    uint_fast64_t sigA_partyOne_X1_Alpha1;
    double64_t t_partyOne_X1_Alpha1 = FloatingPoint::f64_add(signAlpha1, expAlpha1, sigAlpha1, signA, expA, sigA,
                                                           sign_partyOne_X1_Alpha1, expA_partyOne_X1_Alpha1, sigA_partyOne_X1_Alpha1, t_packingProcess);

    bool sign_partyOne_X2_Alpha2;
    int_fast16_t expA_partyOne_X2_Alpha2;
    uint_fast64_t sigA_partyOne_X2_Alpha2;
    double64_t t_partyOne_X2_Alpha2 = FloatingPoint::f64_add(signAlpha2, expAlpha2, sigAlpha2, signB, expB, sigB,
                                                           sign_partyOne_X2_Alpha2, expA_partyOne_X2_Alpha2, sigA_partyOne_X2_Alpha2, t_packingProcess);

    bool sign_partyOne_X1_X2Alpha2;
    int_fast16_t expA_partyOne_X1_X2Alpha2;
    uint_fast64_t sigA_partyOne_X1_X2Alpha2;
    double64_t t_partyOne_X1_X2Alpha2 = FloatingPoint::f64_mul(signA, expA, sigA, sign_partyOne_X2_Alpha2, expA_partyOne_X2_Alpha2,
                                                              sigA_partyOne_X2_Alpha2, sign_partyOne_X1_X2Alpha2, expA_partyOne_X1_X2Alpha2, sigA_partyOne_X1_X2Alpha2);

    bool sign_partyOne_X1Alpha1_X2Alpha2;
    int_fast16_t expA_partyOne_X1Alpha1_X2Alpha2;
    uint_fast64_t sigA_partyOne_X1Alpha1_X2Alpha2;
    double64_t t_partyOne_X1Alpha1_X2Alpha2 = FloatingPoint::f64_mul(sign_partyOne_X2_Alpha2, expA_partyOne_X2_Alpha2, sigA_partyOne_X2_Alpha2,
                                                                    sign_partyOne_X1_Alpha1, expA_partyOne_X1_Alpha1, sigA_partyOne_X1_Alpha1,
                                                                    sign_partyOne_X1Alpha1_X2Alpha2, expA_partyOne_X1Alpha1_X2Alpha2, sigA_partyOne_X1Alpha1_X2Alpha2);

    bool sign_partyOneShare;
    int_fast16_t expA_partyOneShare;
    uint_fast64_t sigA_partyOneShare;

    int t_packingProcess2 = -1;

    sign_partyOne_X1Alpha1_X2Alpha2 = sign_partyOne_X1Alpha1_X2Alpha2 ^ 1;

    double64_t t_partyOneShare = FloatingPoint::f64_add(sign_partyOne_X1_X2Alpha2, expA_partyOne_X1_X2Alpha2, sigA_partyOne_X1_X2Alpha2,
                                                       sign_partyOne_X1Alpha1_X2Alpha2, expA_partyOne_X1Alpha1_X2Alpha2, sigA_partyOne_X1Alpha1_X2Alpha2,
                                                       sign_partyOneShare, expA_partyOneShare, sigA_partyOneShare, t_packingProcess2);

    bool sign_partyTwoShare;
    int_fast16_t expA_partyTwoShare;
    uint_fast64_t sigA_partyTwoShare;
    double64_t t_partyTwoShare = FloatingPoint::f64_mul(signB, expB, sigB,
                                                       sign_partyOne_X1_Alpha1, expA_partyOne_X1_Alpha1, sigA_partyOne_X1_Alpha1,
                                                       sign_partyTwoShare, expA_partyTwoShare, sigA_partyTwoShare);

    bool sign_tempOutput1;
    int_fast16_t expA_tempOutput1;
    uint_fast64_t sigA_tempOutput1;
    int t_packingProcess3 = -1;

    double64_t temp_output1 = FloatingPoint::f64_add(sign_partyOneShare, expA_partyOneShare, sigA_partyOneShare,
                                                    sign_Alpha1_Alpha2, expA_Alpha1_Alpha2, sigA_Alpha1_Alpha2,
                                                    sign_tempOutput1, expA_tempOutput1, sigA_tempOutput1, t_packingProcess3);

    bool sign_tempOutput2;
    int_fast16_t expA_tempOutput2;
    uint_fast64_t sigA_tempOutput2;
    int t_packingProcess4 = -1;

    double64_t temp_output2 = FloatingPoint::f64_add(sign_tempOutput1, expA_tempOutput1, sigA_tempOutput1,
                                                    sign_partyTwoShare, expA_partyTwoShare, sigA_partyTwoShare,
                                                    sign_tempOutput2, expA_tempOutput2, sigA_tempOutput2, t_packingProcess4);

    double originalSumExpected = X1 * X2;
    doubleNumber t_originalSumExpected = FloatingPoint::getSignExpSig(originalSumExpected);
    double output = *(double*)&temp_output2;

    signZ = sign_tempOutput2;
    expZ = expA_tempOutput2;
    sigZ = sigA_tempOutput2;

    Share2.sign = sign_partyOneShare;
    Share2.exp = expA_partyOneShare;
    Share2.sig = sigA_partyOneShare;

    Share3.sign = sign_partyTwoShare;
    Share3.exp = expA_partyTwoShare;
    Share3.sig = sigA_partyTwoShare;

    Share1.sign = sign_Alpha1_Alpha2;
    Share1.exp = expA_Alpha1_Alpha2;
    Share1.sig = sigA_Alpha1_Alpha2;

    return temp_output2;
}

vector<doubleNumber> MPCProtocol::DuatallahMultiplicationProtocol(doubleNumber x1, doubleNumber x2, doubleNumber x3,
                                                                 doubleNumber y1, doubleNumber y2, doubleNumber y3)
{
    int packingProcess = 1;

    doubleNumber x1y1;
    double64_t t_x1y1 = FloatingPoint::f64_mul(x1.sign, x1.exp, x1.sig, y1.sign, y1.exp, y1.sig,
                                               x1y1.sign, x1y1.exp, x1y1.sig);
    doubleNumber x2y2;
    double64_t t_x2y2 = FloatingPoint::f64_mul(x2.sign, x2.exp, x2.sig, y2.sign, y2.exp, y2.sig,
                                               x2y2.sign, x2y2.exp, x2y2.sig);
    doubleNumber x3y3;
    double64_t t_x3y3 = FloatingPoint::f64_mul(x3.sign, x3.exp, x3.sig, y3.sign, y3.exp, y3.sig,
                                               x3y3.sign, x3y3.exp, x3y3.sig);

    doubleNumber x1y2, x1y2_Share1, x1y2_Share2, x1y2_Share3;
    double64_t t_x1y2 = CrossMultiplicationUsingDuatallahProtocol(x1.sign, x1.exp, x1.sig,
                                                                 y2.sign, y2.exp, y2.sig,
                                                                 x1y2.sign, x1y2.exp, x1y2.sig,
                                                                 x1y2_Share1, x1y2_Share2, x1y2_Share3);
    doubleNumber x1y3, x1y3_Share1, x1y3_Share2, x1y3_Share3;
    double64_t t_x1y3 = CrossMultiplicationUsingDuatallahProtocol(x1.sign, x1.exp, x1.sig,
                                                                 y3.sign, y3.exp, y3.sig,
                                                                 x1y3.sign, x1y3.exp, x1y3.sig,
                                                                 x1y3_Share1, x1y3_Share2, x1y3_Share3);
    doubleNumber x2y1, x2y1_Share1, x2y1_Share2, x2y1_Share3;
    double64_t t_x2y1 = CrossMultiplicationUsingDuatallahProtocol(x2.sign, x2.exp, x2.sig,
                                                                 y1.sign, y1.exp, y1.sig,
                                                                 x2y1.sign, x2y1.exp, x2y1.sig,
                                                                 x2y1_Share1, x2y1_Share2, x2y1_Share3);
    doubleNumber x2y3, x2y3_Share1, x2y3_Share2, x2y3_Share3;
    double64_t t_x2y3 = CrossMultiplicationUsingDuatallahProtocol(x2.sign, x2.exp, x2.sig,
                                                                 y3.sign, y3.exp, y3.sig,
                                                                 x2y3.sign, x2y3.exp, x2y3.sig,
                                                                 x2y3_Share1, x2y3_Share2, x2y3_Share3);
    doubleNumber x3y1, x3y1_Share1, x3y1_Share2, x3y1_Share3;
    double64_t t_x3y1 = CrossMultiplicationUsingDuatallahProtocol(x3.sign, x3.exp, x3.sig,
                                                                 y1.sign, y1.exp, y1.sig,
                                                                 x3y1.sign, x3y1.exp, x3y1.sig,
                                                                 x3y1_Share1, x3y1_Share2, x3y1_Share3);
    doubleNumber x3y2, x3y2_Share1, x3y2_Share2, x3y2_Share3;
    double64_t t_x3y2 = CrossMultiplicationUsingDuatallahProtocol(x3.sign, x3.exp, x3.sig,
                                                                 y2.sign, y2.exp, y2.sig,
                                                                 x3y2.sign, x3y2.exp, x3y2.sig,
                                                                 x3y2_Share1, x3y2_Share2, x3y2_Share3);

    doubleNumber PartyOneShare_1;
    double64_t t_PartyOneShare_1 = MultiPartyAdditionProtocol(x1y2_Share1, x1y3_Share1, x2y1_Share1, x2y3_Share1, x3y1_Share1, x3y2_Share1, PartyOneShare_1);
    doubleNumber PartyOneShare;
    double64_t t_PartyOneShare = FloatingPoint::f64_add(x1y1.sign, x1y1.exp, x1y1.sig, PartyOneShare_1.sign, PartyOneShare_1.exp, PartyOneShare_1.sig,
                                                       PartyOneShare.sign, PartyOneShare.exp, PartyOneShare.sig, packingProcess);
    doubleNumber PartyTwoShare_1;
    double64_t t_PartyTwoShare_1 = MultiPartyAdditionProtocol(x1y2_Share2, x1y3_Share2, x2y1_Share2, x2y3_Share2, x3y1_Share2, x3y2_Share2, PartyTwoShare_1);
    doubleNumber PartyTwoShare;
    double64_t t_PartyTwoShare = FloatingPoint::f64_add(x2y2.sign, x2y2.exp, x2y2.sig, PartyTwoShare_1.sign, PartyTwoShare_1.exp, PartyTwoShare_1.sig,
                                                       PartyTwoShare.sign, PartyTwoShare.exp, PartyTwoShare.sig, packingProcess);
    doubleNumber PartyThreeShare_1;
    double64_t t_PartyThreeShare_1 = MultiPartyAdditionProtocol(x1y2_Share3, x1y3_Share3, x2y1_Share3, x2y3_Share3, x3y1_Share3, x3y2_Share3, PartyThreeShare_1);
    doubleNumber PartyThreeShare;
    double64_t t_PartyThreeShare = FloatingPoint::f64_add(x3y3.sign, x3y3.exp, x3y3.sig, PartyThreeShare_1.sign, PartyThreeShare_1.exp, PartyThreeShare_1.sig,
                                                         PartyThreeShare.sign, PartyThreeShare.exp, PartyThreeShare.sig, packingProcess);

    vector<doubleNumber> finalShares;
    finalShares.push_back(PartyOneShare);
    finalShares.push_back(PartyTwoShare);
    finalShares.push_back(PartyThreeShare);

    return finalShares;
}
vector<doubleNumber> MPCProtocol::MultiPartyDivisionProtocol(doubleNumber in_x1, doubleNumber in_x2, doubleNumber in_x3,
                                                             doubleNumber y1, doubleNumber y2, doubleNumber y3,
                                                             bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ)
{
    double X1 = FloatingPoint::rePackForTesting(in_x1);
    double X2 = FloatingPoint::rePackForTesting(in_x2);
    double X3 = FloatingPoint::rePackForTesting(in_x3);
    double Y1 = FloatingPoint::rePackForTesting(y1);
    double Y2 = FloatingPoint::rePackForTesting(y2);
    double Y3 = FloatingPoint::rePackForTesting(y3);

    double totalX = X1 + X2 + X3;
    double totalY = Y1 + Y2 + Y3;


    bool signA_ScalingFactor = 0;
    int_fast16_t expA_ScalingFactor = 1023;
    uint_fast64_t sigA_ScalingFactor = 0;

    ScalingOperands(in_x1.exp, in_x2.exp, in_x3.exp, signA_ScalingFactor, expA_ScalingFactor, sigA_ScalingFactor);

    doubleNumber x1;
    double64_t x1_temp = FloatingPoint::f64_div(in_x1.sign, in_x1.exp, in_x1.sig,
                                                signA_ScalingFactor, expA_ScalingFactor, sigA_ScalingFactor,
                                                x1.sign, x1.exp, x1.sig);
    doubleNumber x2;
    double64_t x2_temp = FloatingPoint::f64_div(in_x2.sign, in_x2.exp, in_x2.sig,
                                                signA_ScalingFactor, expA_ScalingFactor, sigA_ScalingFactor,
                                                x2.sign, x2.exp, x2.sig);
    doubleNumber x3;
    double64_t x3_temp = FloatingPoint::f64_div(in_x3.sign, in_x3.exp, in_x3.sig,
                                                signA_ScalingFactor, expA_ScalingFactor, sigA_ScalingFactor,
                                                x3.sign, x3.exp, x3.sig);

    uint_fast64_t uiZ;
    union ui64_f64 uZ;

    uiZ = packToF64UI(x1.sign, x1.exp, x1.sig);
    uZ.ui = uiZ;
    double64_t x1_reconstruct = uZ.f;
    double t_x1 = *(double*)&x1_reconstruct;
    uiZ = packToF64UI(x2.sign, x2.exp, x2.sig);
    uZ.ui = uiZ;
    double64_t x2_reconstruct = uZ.f;
    double t_x2 = *(double*)&x2_reconstruct;
    uiZ = packToF64UI(x3.sign, x3.exp, x3.sig);
    uZ.ui = uiZ;
    double64_t x3_reconstruct = uZ.f;
    double t_x3 = *(double*)&x3_reconstruct;
    uiZ = packToF64UI(y1.sign, y1.exp, y1.sig);
    uZ.ui = uiZ;
    double64_t y1_reconstruct = uZ.f;
    double t_y1 = *(double*)&y1_reconstruct;
    uiZ = packToF64UI(y2.sign, y2.exp, y2.sig);
    uZ.ui = uiZ;
    double64_t y2_reconstruct = uZ.f;
    double t_y2 = *(double*)&y2_reconstruct;
    uiZ = packToF64UI(y3.sign, y3.exp, y3.sig);
    uZ.ui = uiZ;
    double64_t y3_reconstruct = uZ.f;
    double t_y3 = *(double*)&y3_reconstruct;

    int packingProcess = -1;
    double64_t flt1;
    int value1 = 1;
    flt1 = *(double64_t*)&value1;
    union ui64_f64 uA;
    uint_fast64_t uiA;
    bool signA;
    int_fast16_t expA;
    uint_fast64_t sigA;

    uA.f = flt1;
    uiA = uA.ui;
    signA = 0;
    expA = 1023;
    sigA = 0;

    doubleNumber inverseX1;
    double64_t t_inverseX1 = FloatingPoint::f64_div(signA, expA, sigA, x1.sign, x1.exp, x1.sig,
                                                   inverseX1.sign, inverseX1.exp, inverseX1.sig);
    doubleNumber inverseX2;
    double64_t t_inverseX2 = FloatingPoint::f64_div(signA, expA, sigA, x2.sign, x2.exp, x2.sig,
                                                   inverseX2.sign, inverseX2.exp, inverseX2.sig);
    doubleNumber inverseX3;
    double64_t t_inverseX3 = FloatingPoint::f64_div(signA, expA, sigA, x3.sign, x3.exp, x3.sig,
                                                   inverseX3.sign, inverseX3.exp, inverseX3.sig);

    doubleNumber y1x1;
    double64_t t_y1x1 = FloatingPoint::f64_mul(y1.sign, y1.exp, y1.sig,
                                               inverseX1.sign, inverseX1.exp, inverseX1.sig,
                                               y1x1.sign, y1x1.exp, y1x1.sig);
    doubleNumber y2x2;
    double64_t t_y2x2 = FloatingPoint::f64_mul(y2.sign, y2.exp, y2.sig,
                                               inverseX2.sign, inverseX2.exp, inverseX2.sig,
                                               y2x2.sign, y2x2.exp, y2x2.sig);
    doubleNumber y3x3;
    double64_t t_y3x3 = FloatingPoint::f64_mul(y3.sign, y3.exp, y3.sig,
                                               inverseX3.sign, inverseX3.exp, inverseX3.sig,
                                               y3x3.sign, y3x3.exp, y3x3.sig);

    doubleNumber y1x2_temp, y1x2_Share1_temp, y1x2_Share2_temp, y1x2_Share3_temp;
    double64_t t_y1x2_temp = CrossMultiplicationUsingDuatallahProtocol(y1.sign, y1.exp, y1.sig,
                                                                      inverseX2.sign, inverseX2.exp, inverseX2.sig,
                                                                      y1x2_temp.sign, y1x2_temp.exp, y1x2_temp.sig,
                                                                      y1x2_Share1_temp, y1x2_Share2_temp, y1x2_Share3_temp);
    doubleNumber y1x2, y1x2_Share1, y1x2_Share2, y1x2_Share3;
    double64_t t_y1x2 = CrossMultiplicationUsingDuatallahProtocol(y1.sign, y1.exp, y1.sig,
                                                                 inverseX2.sign, inverseX2.exp, inverseX2.sig,
                                                                 y1x2.sign, y1x2.exp, y1x2.sig,
                                                                 y1x2_Share1, y1x2_Share2, y1x2_Share3);
    doubleNumber y1x3, y1x3_Share1, y1x3_Share2, y1x3_Share3;
    double64_t t_y1x3 = CrossMultiplicationUsingDuatallahProtocol(y1.sign, y1.exp, y1.sig,
                                                                 inverseX3.sign, inverseX3.exp, inverseX3.sig,
                                                                 y1x3.sign, y1x3.exp, y1x3.sig,
                                                                 y1x3_Share1, y1x3_Share2, y1x3_Share3);
    double64_t t_y1x2_temp_1 = CrossMultiplicationUsingDuatallahProtocol(y1.sign, y1.exp, y1.sig,
                                                                        inverseX2.sign, inverseX2.exp, inverseX2.sig,
                                                                        y1x2_temp.sign, y1x2_temp.exp, y1x2_temp.sig,
                                                                        y1x2_Share1_temp, y1x2_Share2_temp, y1x2_Share3_temp);
    doubleNumber y2x1, y2x1_Share1, y2x1_Share2, y2x1_Share3;
    double64_t t_y2x1 = CrossMultiplicationUsingDuatallahProtocol(y2.sign, y2.exp, y2.sig,
                                                                 inverseX1.sign, inverseX1.exp, inverseX1.sig,
                                                                 y2x1.sign, y2x1.exp, y2x1.sig,
                                                                 y2x1_Share1, y2x1_Share2, y2x1_Share3);
    doubleNumber y2x3, y2x3_Share1, y2x3_Share2, y2x3_Share3;
    double64_t t_y2x3 = CrossMultiplicationUsingDuatallahProtocol(y2.sign, y2.exp, y2.sig,
                                                                 inverseX3.sign, inverseX3.exp, inverseX3.sig,
                                                                 y2x3.sign, y2x3.exp, y2x3.sig,
                                                                 y2x3_Share1, y2x3_Share2, y2x3_Share3);
    doubleNumber y3x1, y3x1_Share1, y3x1_Share2, y3x1_Share3;
    double64_t t_y3x1 = CrossMultiplicationUsingDuatallahProtocol(y3.sign, y3.exp, y3.sig,
                                                                 inverseX1.sign, inverseX1.exp, inverseX1.sig,
                                                                 y3x1.sign, y3x1.exp, y3x1.sig,
                                                                 y3x1_Share1, y3x1_Share2, y3x1_Share3);
    doubleNumber y3x2, y3x2_Share1, y3x2_Share2, y3x2_Share3;
    double64_t t_y3x2 = CrossMultiplicationUsingDuatallahProtocol(y3.sign, y3.exp, y3.sig,
                                                                 inverseX2.sign, inverseX2.exp, inverseX2.sig,
                                                                 y3x2.sign, y3x2.exp, y3x2.sig,
                                                                 y3x2_Share1, y3x2_Share2, y3x2_Share3);

    doubleNumber partyOneShare1_1;
    double64_t t_partyOneShare1_1 = FloatingPoint::f64_add(y1x1.sign, y1x1.exp, y1x1.sig,
                                                          y2x1_Share1.sign, y2x1_Share1.exp, y2x1_Share1.sig,
                                                          partyOneShare1_1.sign, partyOneShare1_1.exp, partyOneShare1_1.sig, packingProcess);
    doubleNumber partyOneShare1;
    double64_t t_partyOneShare1 = FloatingPoint::f64_add(partyOneShare1_1.sign, partyOneShare1_1.exp, partyOneShare1_1.sig,
                                                        y3x1_Share1.sign, y3x1_Share1.exp, y3x1_Share1.sig,
                                                        partyOneShare1.sign, partyOneShare1.exp, partyOneShare1.sig, packingProcess);
    doubleNumber partyOneShare2;
    double64_t t_partyOneShare2 = FloatingPoint::f64_add(y1x2_Share1.sign, y1x2_Share1.exp, y1x2_Share1.sig,
                                                        y3x2_Share3.sign, y3x2_Share3.exp, y3x2_Share3.sig,
                                                        partyOneShare2.sign, partyOneShare2.exp, partyOneShare2.sig, packingProcess);
    doubleNumber partyOneShare3;
    double64_t t_partyOneShare3 = FloatingPoint::f64_add(y1x3_Share1.sign, y1x3_Share1.exp, y1x3_Share1.sig,
                                                        y2x3_Share3.sign, y2x3_Share3.exp, y2x3_Share3.sig,
                                                        partyOneShare3.sign, partyOneShare3.exp, partyOneShare3.sig, packingProcess);
    doubleNumber partyTwoShare1;
    double64_t t_partyTwoShare1 = FloatingPoint::f64_add(y2x1_Share2.sign, y2x1_Share2.exp, y2x1_Share2.sig,
                                                        y3x1_Share3.sign, y3x1_Share3.exp, y3x1_Share3.sig,
                                                        partyTwoShare1.sign, partyTwoShare1.exp, partyTwoShare1.sig, packingProcess);
    doubleNumber partyTwoShare2_1;
    double64_t t_partyTwoShare2_1 = FloatingPoint::f64_add(y2x2.sign, y2x2.exp, y2x2.sig,
                                                          y1x2_Share2.sign, y1x2_Share2.exp, y1x2_Share2.sig,
                                                          partyTwoShare2_1.sign, partyTwoShare2_1.exp, partyTwoShare2_1.sig, packingProcess);
    doubleNumber partyTwoShare2;
    double64_t t_partyTwoShare2 = FloatingPoint::f64_add(partyTwoShare2_1.sign, partyTwoShare2_1.exp, partyTwoShare2_1.sig,
                                                        y3x2_Share2.sign, y3x2_Share2.exp, y3x2_Share2.sig,
                                                        partyTwoShare2.sign, partyTwoShare2.exp, partyTwoShare2.sig, packingProcess);
    doubleNumber partyTwoShare3;
    double64_t t_partyTwoShare3 = FloatingPoint::f64_add(y1x3_Share3.sign, y1x3_Share3.exp, y1x3_Share3.sig,
                                                        y2x3_Share2.sign, y2x3_Share2.exp, y2x3_Share2.sig,
                                                        partyTwoShare3.sign, partyTwoShare3.exp, partyTwoShare3.sig, packingProcess);
    doubleNumber partyThreeShare1;
    double64_t t_partyThreeShare1 = FloatingPoint::f64_add(y2x1_Share3.sign, y2x1_Share3.exp, y2x1_Share3.sig,
                                                          y3x1_Share2.sign, y3x1_Share2.exp, y3x1_Share2.sig,
                                                          partyThreeShare1.sign, partyThreeShare1.exp, partyThreeShare1.sig, packingProcess);
    doubleNumber partyThreeShare2;
    double64_t t_partyThreeShare2 = FloatingPoint::f64_add(y1x2_Share3.sign, y1x2_Share3.exp, y1x2_Share3.sig,
                                                          y3x2_Share1.sign, y3x2_Share1.exp, y3x2_Share1.sig,
                                                          partyThreeShare2.sign, partyThreeShare2.exp, partyThreeShare2.sig, packingProcess);
    doubleNumber partyThreeShare3_1;
    double64_t t_partyThreeShare3_1 = FloatingPoint::f64_add(y3x3.sign, y3x3.exp, y3x3.sig,
                                                            y1x3_Share2.sign, y1x3_Share2.exp, y1x3_Share2.sig,
                                                            partyThreeShare3_1.sign, partyThreeShare3_1.exp, partyThreeShare3_1.sig, packingProcess);
    doubleNumber partyThreeShare3;
    double64_t t_partyThreeShare3 = FloatingPoint::f64_add(partyThreeShare3_1.sign, partyThreeShare3_1.exp, partyThreeShare3_1.sig,
                                                          y2x3_Share1.sign, y2x3_Share1.exp, y2x3_Share1.sig,
                                                          partyThreeShare3.sign, partyThreeShare3.exp, partyThreeShare3.sig, packingProcess);
    doubleNumber finalShareOne_1;
    double64_t t_finalShareOne_1 = FloatingPoint::f64_add(partyOneShare3.sign, partyOneShare3.exp, partyOneShare3.sig,
                                                         partyTwoShare3.sign, partyTwoShare3.exp, partyTwoShare3.sig,
                                                         finalShareOne_1.sign, finalShareOne_1.exp, finalShareOne_1.sig, packingProcess);
    doubleNumber finalShareOne;
    double64_t t_finalShareOne = FloatingPoint::f64_add(finalShareOne_1.sign, finalShareOne_1.exp, finalShareOne_1.sig,
                                                       partyThreeShare3.sign, partyThreeShare3.exp, partyThreeShare3.sig,
                                                       finalShareOne.sign, finalShareOne.exp, finalShareOne.sig, packingProcess);
    doubleNumber finalShareTwo_1;
    double64_t t_finalShareTwo_1 = FloatingPoint::f64_add(partyOneShare1.sign, partyOneShare1.exp, partyOneShare1.sig,
                                                         partyTwoShare1.sign, partyTwoShare1.exp, partyTwoShare1.sig,
                                                         finalShareTwo_1.sign, finalShareTwo_1.exp, finalShareTwo_1.sig, packingProcess);
    doubleNumber finalShareTwo;
    double64_t t_finalShareTwo = FloatingPoint::f64_add(finalShareTwo_1.sign, finalShareTwo_1.exp, finalShareTwo_1.sig,
                                                       partyThreeShare1.sign, partyThreeShare1.exp, partyThreeShare1.sig,
                                                       finalShareTwo.sign, finalShareTwo.exp, finalShareTwo.sig, packingProcess);
    doubleNumber finalShareThree_1;
    double64_t t_finalShareThree_1 = FloatingPoint::f64_add(partyOneShare2.sign, partyOneShare2.exp, partyOneShare2.sig,
                                                           partyTwoShare2.sign, partyTwoShare2.exp, partyTwoShare2.sig,
                                                           finalShareThree_1.sign, finalShareThree_1.exp, finalShareThree_1.sig, packingProcess);
    doubleNumber finalShareThree;
    double64_t t_finalShareThree = FloatingPoint::f64_add(finalShareThree_1.sign, finalShareThree_1.exp, finalShareThree_1.sig,
                                                         partyThreeShare2.sign, partyThreeShare2.exp, partyThreeShare2.sig,
                                                         finalShareThree.sign, finalShareThree.exp, finalShareThree.sig, packingProcess);
    doubleNumber inverse_finalShareOne;
    double64_t t_inverse_finalShareOne = FloatingPoint::f64_div(signA_ScalingFactor, expA_ScalingFactor, sigA_ScalingFactor,
                                                               finalShareOne.sign, finalShareOne.exp, finalShareOne.sig,
                                                               inverse_finalShareOne.sign, inverse_finalShareOne.exp, inverse_finalShareOne.sig);
    doubleNumber inverse_finalShareTwo;
    double64_t t_inverse_finalShareTwo = FloatingPoint::f64_div(signA_ScalingFactor, expA_ScalingFactor, sigA_ScalingFactor,
                                                               finalShareTwo.sign, finalShareTwo.exp, finalShareTwo.sig,
                                                               inverse_finalShareTwo.sign, inverse_finalShareTwo.exp, inverse_finalShareTwo.sig);
    doubleNumber inverse_finalShareThree;
    double64_t t_inverse_finalShareThree = FloatingPoint::f64_div(signA_ScalingFactor, expA_ScalingFactor, sigA_ScalingFactor,
                                                                 finalShareThree.sign, finalShareThree.exp, finalShareThree.sig,
                                                                 inverse_finalShareThree.sign, inverse_finalShareThree.exp, inverse_finalShareThree.sig);
    doubleNumber finalOutput_1;
    double64_t t_finalOutput_1 = FloatingPoint::f64_add(inverse_finalShareOne.sign, inverse_finalShareOne.exp, inverse_finalShareOne.sig,
                                                       inverse_finalShareTwo.sign, inverse_finalShareTwo.exp, inverse_finalShareTwo.sig,
                                                       finalOutput_1.sign, finalOutput_1.exp, finalOutput_1.sig, packingProcess);
    doubleNumber finalOutput;
    double64_t t_finalOutput = FloatingPoint::f64_add(finalOutput_1.sign, finalOutput_1.exp, finalOutput_1.sig,
                                                     inverse_finalShareThree.sign, inverse_finalShareThree.exp, inverse_finalShareThree.sig,
                                                     finalOutput.sign, finalOutput.exp, finalOutput.sig, packingProcess);

    signZ = finalOutput.sign;
    expZ = finalOutput.exp;
    sigZ = finalOutput.sig;

    vector<doubleNumber> allDivisionSharesFinal;
    allDivisionSharesFinal.clear();
    allDivisionSharesFinal.push_back(inverse_finalShareOne);
    allDivisionSharesFinal.push_back(inverse_finalShareTwo);
    allDivisionSharesFinal.push_back(inverse_finalShareThree);

    return allDivisionSharesFinal;
}
