#ifndef MPCPROTOCOL_HPP
#define MPCPROTOCOL_HPP

#include <vector>
#include "FloatingPoint.hpp"

class MPCProtocol {
public:
    static void ScalingOperands(int_fast16_t in_exp1, int_fast16_t in_exp2, int_fast16_t in_exp3, bool &sign_ScalingFactor, int_fast16_t &exp_ScalingFactor, uint_fast64_t &sig_ScalingFactor);

    static double getMaxInputValue();

    static double64_t MultiPartyAdditionProtocol(doubleNumber x1, doubleNumber x2, doubleNumber x3,
                                             doubleNumber y1, doubleNumber y2, doubleNumber y3,
                                             doubleNumber &out_totalSum);

    static std::vector<doubleNumber> MultiPartyAdditionProtocolForFinalComputation(doubleNumber x1, doubleNumber y1,
                                                                                  doubleNumber x2, doubleNumber y2,
                                                                                  doubleNumber x3, doubleNumber y3,
                                                                                  double &out_totalSum);

    static double64_t CrossMultiplicationUsingDuatallahProtocol(bool signA, int_fast16_t expA, uint_fast64_t sigA,
                                                           bool signB, int_fast16_t expB, uint_fast64_t sigB,
                                                           bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ,
                                                           doubleNumber &Share1, doubleNumber &Share2, doubleNumber &Share3);

    static std::vector<doubleNumber> DuatallahMultiplicationProtocol(doubleNumber x1, doubleNumber x2, doubleNumber x3,
                                                                     doubleNumber y1, doubleNumber y2, doubleNumber y3);

    static std::vector<doubleNumber> MultiPartyDivisionProtocol(doubleNumber in_x1, doubleNumber in_x2, doubleNumber in_x3,
                                                                doubleNumber y1, doubleNumber y2, doubleNumber y3,
                                                                bool &signZ, int_fast16_t &expZ, uint_fast64_t &sigZ);

private:
    static double maxInputValue;
};

#endif
