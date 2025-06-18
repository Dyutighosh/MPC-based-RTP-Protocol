#include "Utils.hpp"
#include "Constants.hpp"

namespace Utils
{
    const uint8_t countLeadingZeros8[256] = {
        8,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };

    uint8_t countLeadingZeros64(uint64_t a)
    {
        uint_fast8_t count;
        uint32_t a32;
        count = 0;
        a32 = a>>32;
        if ( ! a32 ) {
            count = 32;
            a32 = a;
        }

        if ( a32 < 0x10000 ) {
            count += 16;
            a32 <<= 16;
        }
        if ( a32 < 0x1000000 ) {
            count += 8;
            a32 <<= 8;
        }
        count += countLeadingZeros8[a32>>24];
        return count;
    }

    void divideSupplyValuesIntoShares(double totalValue, std::vector<double> &shareFractions, int choice)
    {
        std::vector<double> sampleWeights;
        
        if(choice == 1)
        {
            sampleWeights = {14, 17, 28, 26, 24, 26, 17, 13, 10, 2};
        }
        else if(choice == 2)
        {
            sampleWeights = {3, 8, 21, 20, 24, 17, 1, 7, 23, 17};
        }

        double sumWeights = 0;
        for(int i = 0; i < numsuppliers; i++)
        {
            sumWeights += sampleWeights[i];
        }
        
        std::vector<double> normalizedWeights;
        for(int i = 0; i < numsuppliers - 1; i++)
        {
            double normalizedValue = sampleWeights[i] / sumWeights;
            normalizedWeights.push_back(normalizedValue);
        }
        
        double sumNormalized = 0;
        for(int i = 0; i < normalizedWeights.size(); i++)
        {
            sumNormalized += normalizedWeights[i];
        }
        
        double lastWeight = 1.0 - sumNormalized;
        normalizedWeights.push_back(lastWeight);

        for(int i = 0; i < numsuppliers; i++)
        {
            shareFractions.push_back(totalValue * normalizedWeights[i]);
        }
    }
}
