#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <vector>

namespace Utils
{
    uint8_t countLeadingZeros64(uint64_t a);
    extern const uint8_t countLeadingZeros8[256];

    void divideSupplyValuesIntoShares(double in_values, std::vector<double> &out_shareOfEachvalue, int in_choice);
}

#endif



