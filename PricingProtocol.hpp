#ifndef PRICINGPROTOCOL_HPP
#define PRICINGPROTOCOL_HPP

#include <vector>
#include "FloatingPoint.hpp"
#include "MPCProtocol.hpp"

class PricingProtocol
{
public:
    static void computeConsumerPricingSingaldouble(const std::vector<double> in_consumerPrices,
                                                  const std::vector<double> in_demand,
                                                  const std::vector<double> in_wDotLambdaZero,
                                                  double in_supply,
                                                  double in_sDotLambdaZero,
                                                  std::vector<double> &out_consumerPrices,
                                                  std::vector<double> &out_schedulingError);

    static void computeSupplierPricingSingaldouble(const std::vector<double> in_consumerPrices,
                                                  const std::vector<double> in_demand,
                                                  const std::vector<double> in_wDotLambdaZero,
                                                  double in_supply,
                                                  double in_sDotLambdaZero,
                                                  std::vector<double> &out_consumerPrices);

    static std::vector<double> computeConsumerPricingSingalMPC(const std::vector<double> in_consumerPrice,
                                                                              const std::vector<double> in_individualDemand,
                                                                              const std::vector<double> in_individualSupply,
                                                                              double in_sDotLambdaZeroDivConsumer,
                                                                              const std::vector<double> in_wDotLambdaZero);

    static std::vector<double> computeSupplierPricingSingalMPC(const std::vector<double> in_supplierPrice,
                                                            const std::vector<double> in_individualSupply,
                                                            const std::vector<double> in_individualDemand,
                                                            double t_wDotLambdaZeroDivSupplier,
                                                            const std::vector<double> in_sDotLambdaZero);

    static void ObtainMaximumSystemPrivateValue();
};

#endif
