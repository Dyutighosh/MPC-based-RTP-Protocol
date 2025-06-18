#include "PricingProtocol.hpp"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

extern const int numconsumers;
extern const int numsuppliers;
extern const double ETA; 

int64_t quantizeDoubleKey(double val, int decimals = 16) 
{
    double scale = std::pow(10, decimals);
    return static_cast<int64_t>(std::round(val * scale));
}

void PricingProtocol::computeConsumerPricingSingaldouble(vector<double> in_consumerPrices, vector<double> in_demand, vector<double> in_wDotLambdaZero, double in_supply, double in_sDotLambdaZero, vector<double> &out_consumerPrices, vector<double> &out_schedulingError)
{
    out_consumerPrices.clear();
    out_schedulingError.clear();

    double t_totalPrice = 0;
    for (int i = 0; i < numconsumers; i++) {
        t_totalPrice += in_consumerPrices[i];
    }

    vector<double> t_ChangedPrice;
    t_ChangedPrice.clear();

    for (int i = 0; i < numconsumers; i++) {
        double term2 = in_demand[i];
        double term1_1 = in_consumerPrices[i] / t_totalPrice;
        double term1 = in_supply * term1_1;
        double temp1 = term1 - term2;
        double constant_1 = in_sDotLambdaZero - in_wDotLambdaZero[i];
        double constant = (2.0 * ETA) / constant_1;
        double temp = temp1 * constant;

        t_ChangedPrice.push_back(temp);
        out_schedulingError.push_back(temp1);
    }

    for (int i = 0; i < numconsumers; i++) {
        out_consumerPrices.push_back(in_consumerPrices[i] - t_ChangedPrice[i]);
    }
}

void PricingProtocol::computeSupplierPricingSingaldouble(vector<double> in_consumerPrices, vector<double> in_demand, vector<double> in_wDotLambdaZero, double in_supply, double in_sDotLambdaZero, vector<double> &out_consumerPrices)
{
    out_consumerPrices.clear();

    double t_totalPrice = 0;
    for (int i = 0; i < numsuppliers; i++) {
        t_totalPrice += in_consumerPrices[i];
    }

    vector<double> t_ChangedPrice;
    t_ChangedPrice.clear();

    for (int i = 0; i < numsuppliers; i++) {
        double term1 = in_demand[i];
        double term2_1 = in_consumerPrices[i] / t_totalPrice;
        double term2 = in_supply * term2_1;
        double temp1 = term1 - term2;
        double constant_1 = in_wDotLambdaZero[i] - in_sDotLambdaZero;
        double constant = (2.0 * ETA) / constant_1;
        double temp = temp1 * constant;

        t_ChangedPrice.push_back(temp);
    }

    for (int i = 0; i < numsuppliers; i++) {
        out_consumerPrices.push_back(in_consumerPrices[i] - t_ChangedPrice[i]);
    }
}

vector<double> PricingProtocol::computeConsumerPricingSingalMPC(vector<double> in_consumerPrice, vector<double> in_individualDemand, vector<double> in_individualSupply, double in_sDotLambdaZeroDivConsumer, vector<double> in_wDotLambdaZero)
{
    vector<double> out_consumerPrice;
    out_consumerPrice.clear();

    int packingProcess = 1;
    vector<double> t_constantTerm;
    t_constantTerm.clear();

    for (int i = 0; i < numconsumers; i++) {
        double temp_denominator = in_sDotLambdaZeroDivConsumer - in_wDotLambdaZero[i];
        double temp_ConstantValue = (2.0 * ETA) / temp_denominator;
        t_constantTerm.push_back(temp_ConstantValue);
    }

    vector<vector<doubleNumber>> conPriceShare;
    conPriceShare.clear();

    vector<vector<double>> conPriceShare_withDouble;
    conPriceShare_withDouble.clear();

    for (int i = 0; i < 10; i++) {
        vector<double> temp_conPriceShare_withDouble;
        temp_conPriceShare_withDouble.clear();

        vector<doubleNumber> temp_conPriceShare;
        doubleNumber t_share1, t_share2, t_share3;
        union ui64_f64 uA;
        uint_fast64_t uiA;
        bool signA;
        int_fast16_t expA;
        uint_fast64_t sigA;
        uA.f = *(double64_t*)&in_consumerPrice[i];
        uiA = uA.ui;
        signA = signF64UI(uiA);
        expA = expF64UI(uiA);
        sigA = fracF64UI(uiA);
        t_share1.sign = 0;
        t_share1.exp  = 0;
        t_share1.sig  = 0;       
        t_share2.sign = 0;
        t_share2.exp  = 0;
        t_share2.sig  = 0;
        t_share3.sign = signA;
        t_share3.exp  = expA;
        t_share3.sig  = sigA;
        temp_conPriceShare.push_back(t_share1);
        temp_conPriceShare.push_back(t_share2);
        temp_conPriceShare.push_back(t_share3);
        conPriceShare.push_back(temp_conPriceShare);

        temp_conPriceShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_conPriceShare[0]));
        temp_conPriceShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_conPriceShare[1]));
        temp_conPriceShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_conPriceShare[2]));

        conPriceShare_withDouble.push_back(temp_conPriceShare_withDouble);
    }

    for (int i = 10; i < numconsumers; i++) 
    {
        int index = i % 10;
        conPriceShare.push_back(conPriceShare[index]);
        conPriceShare_withDouble.push_back(conPriceShare_withDouble[index]);
    }

    vector<doubleNumber> totalPriceShares;
    totalPriceShares.clear();

    for (int i = 0; i < 3; i++) 
    {
        doubleNumber totalPriceShare;
        totalPriceShare.sign = 0;
        totalPriceShare.exp = 0;
        totalPriceShare.sig = 0;

        for (int j = 0; j < numconsumers; j++) 
        {
            FloatingPoint::f64_add(totalPriceShare.sign, totalPriceShare.exp, totalPriceShare.sig, conPriceShare[j][i].sign, conPriceShare[j][i].exp, conPriceShare[j][i].sig, totalPriceShare.sign, totalPriceShare.exp, totalPriceShare.sig, packingProcess);
        }

        totalPriceShares.push_back(totalPriceShare);
    }

    vector<double> totalPriceShares_double;
    totalPriceShares_double.clear();

    for (int i = 0; i < 3; i++) {
        double temp_before = 0;

        for (int j = 0; j < numconsumers; j++) {
            temp_before += conPriceShare_withDouble[j][i];
        }

        totalPriceShares_double.push_back(temp_before);
    }

    vector<vector<doubleNumber>> supSuppShare;
    supSuppShare.clear();

    vector<vector<double>> conSupplyShare_withDouble;
    conSupplyShare_withDouble.clear();

    for (int i = 0; i < numsuppliers; i++) {
        vector<double> temp_conSupplyShare_withDouble;
        temp_conSupplyShare_withDouble.clear();

        vector<doubleNumber> temp_supSuppShare;
        doubleNumber t_share1, t_share2, t_share3;
        union ui64_f64 uA;
        uint_fast64_t uiA;
        bool signA;
        int_fast16_t expA;
        uint_fast64_t sigA;
        uA.f = *(double64_t*)&in_individualSupply[i];
        uiA = uA.ui;
        signA = signF64UI(uiA);
        expA = expF64UI(uiA);
        sigA = fracF64UI(uiA);
        t_share1.sign = 0;
        t_share1.exp  = 0;
        t_share1.sig  = 0;       
        t_share2.sign = 0;
        t_share2.exp  = 0;
        t_share2.sig  = 0;
        t_share3.sign = signA;
        t_share3.exp  = expA;
        t_share3.sig  = sigA;
        temp_supSuppShare.push_back(t_share1);
        temp_supSuppShare.push_back(t_share2);
        temp_supSuppShare.push_back(t_share3);
        supSuppShare.push_back(temp_supSuppShare);

        temp_conSupplyShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_supSuppShare[0]));
        temp_conSupplyShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_supSuppShare[1]));
        temp_conSupplyShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_supSuppShare[2]));

        conSupplyShare_withDouble.push_back(temp_conSupplyShare_withDouble);
    }

    vector<doubleNumber> totalSupplyShares;
    totalSupplyShares.clear();

    for (int i = 0; i < 3; i++) {
        doubleNumber totalSupplyShare;
        totalSupplyShare.sign = 0;
        totalSupplyShare.exp = 0;
        totalSupplyShare.sig = 0;

        for (int j = 0; j < numsuppliers; j++) {
            FloatingPoint::f64_add(totalSupplyShare.sign, totalSupplyShare.exp, totalSupplyShare.sig, supSuppShare[j][i].sign, supSuppShare[j][i].exp, supSuppShare[j][i].sig, totalSupplyShare.sign, totalSupplyShare.exp, totalSupplyShare.sig, packingProcess);
        }

        totalSupplyShares.push_back(totalSupplyShare);
    }

    vector<double> totalSupplyShares_double;
    totalSupplyShares_double.clear();

    for (int i = 0; i < 3; i++) {
        double temp_before = 0;

        for (int j = 0; j < numsuppliers; j++) {
            temp_before += conSupplyShare_withDouble[j][i];
        }

        totalSupplyShares_double.push_back(temp_before);
    }

    vector<vector<doubleNumber>> conDemandShare;
    conDemandShare.clear();
    vector<vector<double>> conDemandShare_withDouble;
    conDemandShare_withDouble.clear();

    for (int i = 0; i < numconsumers; i++) {
        vector<double> temp_conDemandShare_withDouble;
        temp_conDemandShare_withDouble.clear();

        vector<doubleNumber> temp_conDemandShare;
        doubleNumber t_share1, t_share2, t_share3;
        union ui64_f64 uA;
        uint_fast64_t uiA;
        bool signA;
        int_fast16_t expA;
        uint_fast64_t sigA;
        uA.f = *(double64_t*)&in_individualDemand[i];
        uiA = uA.ui;
        signA = signF64UI(uiA);
        expA = expF64UI(uiA);
        sigA = fracF64UI(uiA);
        t_share1.sign = 0;
        t_share1.exp  = 0;
        t_share1.sig  = 0;       
        t_share2.sign = 0;
        t_share2.exp  = 0;
        t_share2.sig  = 0;
        t_share3.sign = signA;
        t_share3.exp  = expA;
        t_share3.sig  = sigA;
        temp_conDemandShare.push_back(t_share1);
        temp_conDemandShare.push_back(t_share2);
        temp_conDemandShare.push_back(t_share3);
        conDemandShare.push_back(temp_conDemandShare);

        temp_conDemandShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_conDemandShare[0]));
        temp_conDemandShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_conDemandShare[1]));
        temp_conDemandShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_conDemandShare[2]));

        conDemandShare_withDouble.push_back(temp_conDemandShare_withDouble);
    }


    for (int i = 0; i < numconsumers; i++) {
        doubleNumber t_divisionShare;
        vector<doubleNumber> divisionShare = MPCProtocol::MultiPartyDivisionProtocol(conPriceShare[i][0], conPriceShare[i][1], conPriceShare[i][2], totalPriceShares[0], totalPriceShares[1], totalPriceShares[2], t_divisionShare.sign, t_divisionShare.exp, t_divisionShare.sig);

        vector<doubleNumber> supplyTimesDivisionShares = MPCProtocol::DuatallahMultiplicationProtocol(totalSupplyShares[0], totalSupplyShares[1], totalSupplyShares[2], divisionShare[0], divisionShare[1], divisionShare[2]);

        conDemandShare[i][0].sign = conDemandShare[i][0].sign ^ 1;
        conDemandShare[i][1].sign = conDemandShare[i][1].sign ^ 1;
        conDemandShare[i][2].sign = conDemandShare[i][2].sign ^ 1;

        double finalComputedValue = 0;
        vector<doubleNumber> allfinalSharesOfRTP = MPCProtocol::MultiPartyAdditionProtocolForFinalComputation(supplyTimesDivisionShares[0], conDemandShare[i][0], supplyTimesDivisionShares[1], conDemandShare[i][1], supplyTimesDivisionShares[2], conDemandShare[i][2], finalComputedValue);

        double t_generationErrorForConsumer = finalComputedValue;
        double changeInPrice = t_constantTerm[i] * t_generationErrorForConsumer;
        out_consumerPrice.push_back(in_consumerPrice[i] - changeInPrice);
    }

    return out_consumerPrice;
}

vector<double> PricingProtocol::computeSupplierPricingSingalMPC(vector<double> in_supplierPrice, vector<double> in_individualSupply, vector<double> in_individualDemand, double t_wDotLambdaZeroDivSupplier, vector<double> in_sDotLambdaZero)
{
    vector<double> out_supplierPrice;
    out_supplierPrice.clear();

    int packingProcess = 1;
    vector<double> t_constantTerm;
    t_constantTerm.clear();

    for (int i = 0; i < numsuppliers; i++) {
        double temp_denominator = in_sDotLambdaZero[i] - t_wDotLambdaZeroDivSupplier;
        double temp_ConstantValue = (2.0 * ETA) / temp_denominator;
        t_constantTerm.push_back(temp_ConstantValue);
    }

    vector<vector<doubleNumber>> supPriceShare;
    supPriceShare.clear();

    vector<vector<double>> supPriceShareFP;
    supPriceShareFP.clear();

    for (int i = 0; i < numsuppliers; i++) {
        vector<double> supPriceShare_withDouble;
        supPriceShare_withDouble.clear();

        vector<doubleNumber> temp_supPriceShare;
        doubleNumber t_share1, t_share2, t_share3;
        union ui64_f64 uA;
        uint_fast64_t uiA;
        bool signA;
        int_fast16_t expA;
        uint_fast64_t sigA;
        uA.f = *(double64_t*)&in_supplierPrice[i];
        uiA = uA.ui;
        signA = signF64UI(uiA);
        expA = expF64UI(uiA);
        sigA = fracF64UI(uiA);
        t_share1.sign = 0;
        t_share1.exp  = 0;
        t_share1.sig  = 0;       
        t_share2.sign = 0;
        t_share2.exp  = 0;
        t_share2.sig  = 0;
        t_share3.sign = signA;
        t_share3.exp  = expA;
        t_share3.sig  = sigA;
        temp_supPriceShare.push_back(t_share1);
        temp_supPriceShare.push_back(t_share2);
        temp_supPriceShare.push_back(t_share3);
        supPriceShare.push_back(temp_supPriceShare);

        supPriceShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_supPriceShare[0]));
        supPriceShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_supPriceShare[1]));
        supPriceShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_supPriceShare[2]));

        supPriceShareFP.push_back(supPriceShare_withDouble);
    }

    vector<vector<doubleNumber>> conDemandShare;
    conDemandShare.clear();

    vector<vector<double>> conDemandShareFP;
    conDemandShareFP.clear();

    for (int i = 0; i < numconsumers; i++) {
        vector<double> conDemandShare_withDouble;
        conDemandShare_withDouble.clear();

        vector<doubleNumber> temp_conDemandShare;
        doubleNumber t_share1, t_share2, t_share3;
        union ui64_f64 uA;
        uint_fast64_t uiA;
        bool signA;
        int_fast16_t expA;
        uint_fast64_t sigA;
        uA.f = *(double64_t*)&in_individualDemand[i];
        uiA = uA.ui;
        signA = signF64UI(uiA);
        expA = expF64UI(uiA);
        sigA = fracF64UI(uiA);
        t_share1.sign = 0;
        t_share1.exp  = 0;
        t_share1.sig  = 0;       
        t_share2.sign = 0;
        t_share2.exp  = 0;
        t_share2.sig  = 0;
        t_share3.sign = signA;
        t_share3.exp  = expA;
        t_share3.sig  = sigA;
        temp_conDemandShare.push_back(t_share1);
        temp_conDemandShare.push_back(t_share2);
        temp_conDemandShare.push_back(t_share3);
        conDemandShare.push_back(temp_conDemandShare);

        conDemandShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_conDemandShare[0]));
        conDemandShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_conDemandShare[1]));
        conDemandShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_conDemandShare[2]));

        conDemandShareFP.push_back(conDemandShare_withDouble);
    }

    vector<vector<doubleNumber>> supSuppShare;
    supSuppShare.clear();

    vector<vector<double>> supSuppShareFP;
    supSuppShareFP.clear();

    for (int i = 0; i < numsuppliers; i++) {
        vector<double> supSuppShare_withDouble;
        supSuppShare_withDouble.clear();

        vector<doubleNumber> temp_supSuppShare;
        doubleNumber t_share1, t_share2, t_share3;
        union ui64_f64 uA;
        uint_fast64_t uiA;
        bool signA;
        int_fast16_t expA;
        uint_fast64_t sigA;
        uA.f = *(double64_t*)&in_individualSupply[i];
        uiA = uA.ui;
        signA = signF64UI(uiA);
        expA = expF64UI(uiA);
        sigA = fracF64UI(uiA);
        t_share1.sign = 0;
        t_share1.exp  = 0;
        t_share1.sig  = 0;       
        t_share2.sign = 0;
        t_share2.exp  = 0;
        t_share2.sig  = 0;
        t_share3.sign = signA;
        t_share3.exp  = expA;
        t_share3.sig  = sigA;
        temp_supSuppShare.push_back(t_share1);
        temp_supSuppShare.push_back(t_share2);
        temp_supSuppShare.push_back(t_share3);
        supSuppShare.push_back(temp_supSuppShare);

        supSuppShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_supSuppShare[0]));
        supSuppShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_supSuppShare[1]));
        supSuppShare_withDouble.push_back(FloatingPoint::rePackForTesting(temp_supSuppShare[2]));

        supSuppShareFP.push_back(supSuppShare_withDouble);
    }

    vector<doubleNumber> totalPriceShares;
    totalPriceShares.clear();

    for (int i = 0; i < 3; i++) {
        doubleNumber totalPriceShare;
        totalPriceShare.sign = 0;
        totalPriceShare.exp = 0;
        totalPriceShare.sig = 0;

        for (int j = 0; j < numsuppliers; j++) {
            FloatingPoint::f64_add(totalPriceShare.sign, totalPriceShare.exp, totalPriceShare.sig, supPriceShare[j][i].sign, supPriceShare[j][i].exp, supPriceShare[j][i].sig, totalPriceShare.sign, totalPriceShare.exp, totalPriceShare.sig, packingProcess);
        }

        totalPriceShares.push_back(totalPriceShare);
    }

    vector<double> totalPriceShares_double;
    totalPriceShares_double.clear();

    for (int i = 0; i < 3; i++) {
        double temp_before = 0;

        for (int j = 0; j < numsuppliers; j++) {
            temp_before += supPriceShareFP[j][i];
        }

        totalPriceShares_double.push_back(temp_before);
    }

    vector<doubleNumber> totalDemandShares;
    totalDemandShares.clear();

    for (int i = 0; i < 3; i++) {
        doubleNumber totalDemandShare;
        totalDemandShare.sign = 0;
        totalDemandShare.exp = 0;
        totalDemandShare.sig = 0;

        for (int j = 0; j < numconsumers; j++) {
            FloatingPoint::f64_add(totalDemandShare.sign, totalDemandShare.exp, totalDemandShare.sig, conDemandShare[j][i].sign, conDemandShare[j][i].exp, conDemandShare[j][i].sig, totalDemandShare.sign, totalDemandShare.exp, totalDemandShare.sig, packingProcess);
        }

        totalDemandShares.push_back(totalDemandShare);
    }

    vector<double> totalDemandShares_double;
    totalDemandShares_double.clear();

    for (int i = 0; i < 3; i++) {
        double temp_before = 0;

        for (int j = 0; j < numconsumers; j++) {
            temp_before += conDemandShareFP[j][i];
        }

        totalDemandShares_double.push_back(temp_before);
    }

    for (int i = 0; i < numsuppliers; i++) {
        doubleNumber t_divisionShare;
        vector<doubleNumber> divisionShare = MPCProtocol::MultiPartyDivisionProtocol(supPriceShare[i][0], supPriceShare[i][1], supPriceShare[i][2], totalPriceShares[0], totalPriceShares[1], totalPriceShares[2], t_divisionShare.sign, t_divisionShare.exp, t_divisionShare.sig);

        vector<doubleNumber> supplyTimesDivisionShares = MPCProtocol::DuatallahMultiplicationProtocol(totalDemandShares[0], totalDemandShares[1], totalDemandShares[2], divisionShare[0], divisionShare[1], divisionShare[2]);

        supplyTimesDivisionShares[0].sign ^= 1;
        supplyTimesDivisionShares[1].sign ^= 1;
        supplyTimesDivisionShares[2].sign ^= 1;

        double finalComputedValue = 0;
        vector<doubleNumber> allfinalSharesOfRTP = MPCProtocol::MultiPartyAdditionProtocolForFinalComputation(supSuppShare[i][0], supplyTimesDivisionShares[0], supSuppShare[i][1], supplyTimesDivisionShares[1], supSuppShare[i][2], supplyTimesDivisionShares[2], finalComputedValue);

        double t_generationErrorForSupplier = finalComputedValue;
        double changeInPrice = t_constantTerm[i] * t_generationErrorForSupplier;

        out_supplierPrice.push_back(in_supplierPrice[i] - changeInPrice);
    }

    return out_supplierPrice;
}

void PricingProtocol::ObtainMaximumSystemPrivateValue()
{
    std::cout << "Maximum Private System Parameter: " << MPCProtocol::getMaxInputValue() << std::endl;
    double in_number = MPCProtocol::getMaxInputValue() * 3.0; //For 3 shares. To be extra security
    union ui64_f64 uA;
    uint_fast64_t uiA;
    bool signA;
    int_fast16_t expA;
    uint_fast64_t sigA;
    uA.f = *(double64_t*)&in_number;
    uiA = uA.ui;
    signA = signF64UI(uiA);
    expA = expF64UI(uiA);
    sigA = fracF64UI(uiA);
    cout << "expA: " <<expA <<endl;
    cout << "sigA: " <<sigA <<endl;
}