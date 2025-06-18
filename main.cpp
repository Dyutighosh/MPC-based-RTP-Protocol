// This project presents a proof-of-concept implementation of Real-Time Pricing (RTP) in smart grids using Multi-Party Computation (MPC). The current version of implementation is an in-place simulation, focused primarily on evaluating the accuracy of the MPC-based RTP protocol. It does not yet incorporate the full communication pipeline among multiple parties. This allows us to isolate and evaluate the correctness and feasibility of secure computations in the context of real-time pricing. We are actively working on extending this implementation to a three-party, client-server-based architecture, where each party participates semi-honestly (i.e., they follow the protocol but may try to learn extra information). The later version will include networked communication, synchronization, and data exchange mechanisms between the parties in a distributed setup. Timing measurements in this prototype were obtained by estimating the total data exchanged, dividing it by the local network throughput, and adding the total computational time for each party. While these estimates are currently based on simulated communication, they provide a meaningful baseline for evaluating performance in practical deployments. The forthcoming implementation will include explicit synchronization and communication delays, further improving realistic conditions.
// This implementation is generalized for smart grid pricing. However, it can also serve as a secure computation framework for Cyber-Physical Systems (CPS) where system dynamics, controller parameters, and signal bounds are well-defined. Just as in Kalman-filter-based control and state estimation in CPS, it is assumed that the system dynamics, consumer and supplier behaviors, and signal ranges are known a priori (In CPS, leakage in Real World already present as the bounds of all the variables are already known). Specifically, this includes: i) Known bounds on consumer pricing signals and demands, ii) Known bounds on supplier pricing and scheduled supply, iii) Predictable interaction models among system components. Given such assumptions, this MPC-based approach can be applied to any CPS scenario where preserving the privacy of individual users/agents (e.g., consumers or control agents) is essential, while still enabling collaborative and highly accurate computation over private inputs. 

// Incase of any clarity or suggestions please reach out to soumyadyuti.ghosh@gmail.com

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <string>
#include <math.h>
#include "Constants.hpp"
#include "FloatingPoint.hpp"
#include "MPCProtocol.hpp"
#include "PricingProtocol.hpp"
#include "Utils.hpp"

using namespace std;

extern const int numconsumers;
extern const int numsuppliers;
extern const double ETA;
extern const double LAMBDA;
extern const double bk; 


int main()
{
    cout << setprecision(400);

    vector<double> t_sDotLambdaZero;
    double t_sDotLambdaZeroConsumer;
    vector<double> t_totalSupplyFordouble;
    vector<double> t_totalSupplyForMPCdouble;
    vector<double> t_doubleSupplierPrice;
    vector<double> t_supplierPriceforMPC;

    vector<double> t_wDotLambdaZero;
    vector<double> t_individualDemandFordouble;
    vector<double> t_individualDemandForMPC;
    vector<double> t_consumerPricesUSingdoubleingPoint;
    vector<double> t_doubleConsumerPrices;

    for (int i = 0; i < numsuppliers; i++) {
        t_supplierPriceforMPC.push_back(LAMBDA);
        t_doubleSupplierPrice.push_back(LAMBDA);
    }

    double t_totalWlambdaZeroForSupplier = 0;

    for (int i = 0; i < numconsumers; i++) {
        t_consumerPricesUSingdoubleingPoint.push_back(LAMBDA);
        t_doubleConsumerPrices.push_back(LAMBDA);

        double wDotLambdaZero = d[i % 10] * eph[i % 10] * pow(LAMBDA, eph[i % 10] - 1);
        t_wDotLambdaZero.push_back(wDotLambdaZero);
        t_totalWlambdaZeroForSupplier += wDotLambdaZero;
    }

    double p = (0.57 * 152.0 * numconsumers) / 2800000;
    double q = (0.57 * 4503.0 * numconsumers) / 2800000;
    vector<double> t_individualSupplierP;
    vector<double> t_individualSupplierQ;
    Utils::divideSupplyValuesIntoShares(p, t_individualSupplierP, 1);
    Utils::divideSupplyValuesIntoShares(q, t_individualSupplierQ, 2);

    t_sDotLambdaZeroConsumer = p / static_cast<double>(numconsumers);
    t_totalWlambdaZeroForSupplier /= static_cast<double>(numsuppliers);

    vector<double> t_wDotLambdaZeroSharesForSupplier;
    vector<vector<double>> t_allConsumerPricesOriginal;
    vector<vector<double>> t_allConsumerPricesdouble;
    vector<vector<double>> t_supplierPriceOriginal;
    vector<vector<double>> t_supplierPriceUsingMPC;
    vector<vector<double>> t_SchedulingErrorOfIndividualConsumer;
    vector<double> t_overallGenerationSchedulingError;

    int counter = 0;

    while (true) {

        cout << "Consumers price (without MPC)   : " << endl;

        vector<double> t_schedulingErrorOfEachIteration;
        t_individualDemandFordouble.clear();

        for (int i = 0; i < numconsumers; i++) {
            if (i < 10) {
                cout << t_doubleConsumerPrices[i] << ",";
            }
            double d_k = bk + (d[i%10]*pow(t_doubleConsumerPrices[i],eph[i%10]));
            t_individualDemandFordouble.push_back(d_k);
        }
        cout << endl;

        t_allConsumerPricesOriginal.push_back(t_doubleConsumerPrices);

        cout << "Consumers price (using MPC)     :    " << endl;

        t_individualDemandForMPC.clear();

        for (int i = 0; i < numconsumers; i++) {
            if (i < 10) {
                cout << t_consumerPricesUSingdoubleingPoint[i] << ",";
            }
            double d_k = bk + (d[i % 10] * pow(t_consumerPricesUSingdoubleingPoint[i], eph[i % 10]));
            t_individualDemandForMPC.push_back(d_k);
        }
        cout << endl;

        t_allConsumerPricesdouble.push_back(t_consumerPricesUSingdoubleingPoint);

        t_totalSupplyFordouble.clear();

        for (int i = 0; i < numsuppliers; i++) {
            double s_i = t_individualSupplierP[i] * t_doubleSupplierPrice[i] + t_individualSupplierQ[i];
            t_totalSupplyFordouble.push_back(s_i);
        }

        t_supplierPriceOriginal.push_back(t_doubleSupplierPrice);

        t_totalSupplyForMPCdouble.clear();

        for (int i = 0; i < numsuppliers; i++) {
            double s_i = t_individualSupplierP[i] * t_supplierPriceforMPC[i] + t_individualSupplierQ[i];
            t_totalSupplyForMPCdouble.push_back(s_i);
        }

        t_supplierPriceUsingMPC.push_back(t_supplierPriceforMPC);

        double t_totalDemandFordouble = 0;
        for (auto val : t_individualDemandFordouble) {
            t_totalDemandFordouble += val;
        }

        double t_totalSupplyFordoubleWithoutMPC = 0;
        for (auto val : t_totalSupplyFordouble) {
            t_totalSupplyFordoubleWithoutMPC += val;
        }

        double generationSchedulingError = abs(t_totalDemandFordouble - t_totalSupplyFordoubleWithoutMPC);

        double t_totalDemandForMPC = 0;
        for (auto val : t_individualDemandForMPC) {
            t_totalDemandForMPC += val;
        }

        double t_totalSupplyFordoubleWithMPC = 0;
        for (auto val : t_totalSupplyForMPCdouble) {
            t_totalSupplyFordoubleWithMPC += val;
        }

        double generationSchedulingError1 = abs(t_totalDemandForMPC - t_totalSupplyFordoubleWithMPC);

        t_overallGenerationSchedulingError.push_back(generationSchedulingError1);

        counter++;

        cout << "Generation Scheduling Error For without MPC                        : " << generationSchedulingError << endl;
        cout << "Generation Scheduling Error Using MPC                              : " << generationSchedulingError1 << endl;

        cout << "Error For without MPC (Generation Error / total Demand)            : " << (generationSchedulingError / t_totalDemandFordouble) * 100 << endl;
        cout << "Error For with MPC (Generation Error / total Demand)               : " << (generationSchedulingError1 / t_totalDemandForMPC) * 100 << endl;

        cout << endl << endl << endl;

        if (counter > 100) 
        {
            PricingProtocol::ObtainMaximumSystemPrivateValue();
            break;
        }

        PricingProtocol::computeConsumerPricingSingaldouble(t_doubleConsumerPrices, t_individualDemandFordouble, t_wDotLambdaZero, t_totalSupplyFordoubleWithoutMPC, t_sDotLambdaZeroConsumer, t_doubleConsumerPrices, t_schedulingErrorOfEachIteration);

        PricingProtocol::computeSupplierPricingSingaldouble(t_doubleSupplierPrice, t_totalSupplyFordouble, t_individualSupplierP, t_totalDemandFordouble, t_totalWlambdaZeroForSupplier, t_doubleSupplierPrice);

        t_SchedulingErrorOfIndividualConsumer.push_back(t_schedulingErrorOfEachIteration);

        vector<double> out_consumerPricesUSingdoubleingPoint = PricingProtocol::computeConsumerPricingSingalMPC(t_consumerPricesUSingdoubleingPoint, t_individualDemandForMPC, t_totalSupplyForMPCdouble, t_sDotLambdaZeroConsumer, t_wDotLambdaZero);

        t_consumerPricesUSingdoubleingPoint = out_consumerPricesUSingdoubleingPoint;

        vector<double> out_supplierPriceforMPC = PricingProtocol::computeSupplierPricingSingalMPC(t_supplierPriceforMPC, t_totalSupplyForMPCdouble, t_individualDemandForMPC, t_totalWlambdaZeroForSupplier, t_individualSupplierP);
        
        t_supplierPriceforMPC = out_supplierPriceforMPC;
    }

    cout << "--------------------------------------------------------------------Overall Generation Scheduling Error---------------------------------------------------------------------" << endl << endl;
    for (auto err : t_overallGenerationSchedulingError) {
        cout << err << ",";
    }
    cout << endl << endl;

    cout << "-----------------------------------------------------------------------------ERROR PERCENTAGE-----------------------------------------------------------------------------" << endl;

    vector<vector<double>> t_allConsumerPricesErrorAverage;
    vector<string> t_allConsumerPricesErrorAverageString;
    vector<double> t_vectorOfConsumersMaxErrors;

    int iterationNumber = 100;

    for (int i = 0; i < numconsumers; i++) {
        if (i < 10) {
            cout << endl << endl << endl;
            cout << "Consumer ID:   " << i << endl;
        }

        string t_value = "";
        vector<double> t_allConsumerPricesErrorAveragePer;
        t_allConsumerPricesErrorAveragePer.clear();

        for (int j = 0; j < iterationNumber; j++) {
            double temp_____ = (abs(t_allConsumerPricesdouble[j][i] - t_allConsumerPricesOriginal[j][i]) / t_allConsumerPricesOriginal[j][i]) * 100;

            if (i < 10) {
                if (j == iterationNumber - 1) {
                    cout << temp_____ << endl;
                } else {
                    cout << temp_____ << ",";
                }
            }
            t_value += to_string(temp_____) + ",";
            t_allConsumerPricesErrorAveragePer.push_back(temp_____);
        }

        t_vectorOfConsumersMaxErrors.push_back(*max_element(t_allConsumerPricesErrorAveragePer.begin(), t_allConsumerPricesErrorAveragePer.end()));

        if (i < 10) {
            cout << "max error: " << double(*max_element(t_allConsumerPricesErrorAveragePer.begin(), t_allConsumerPricesErrorAveragePer.end())) << endl;
        }

        t_allConsumerPricesErrorAverage.push_back(t_allConsumerPricesErrorAveragePer);
        t_value.erase(t_value.size() - 1);
        t_allConsumerPricesErrorAverageString.push_back(t_value);
    }

    cout << "MAX ERRORS OF THE CONSUMERS-----------------------------------------------------------------------------" << endl << endl;
    for (auto err : t_vectorOfConsumersMaxErrors) {
        cout << err << ",";
    }
    cout << endl << endl;

    cout << "Average Errors of the Consumers-----------------------------------------------------------------------------" << endl << endl;

    vector<double> t_allConsumerPricesErrorAveragePerFinal;
    string t_valueConAverage = "";

    for (int j = 0; j < iterationNumber; j++) {
        double temp_aaa = 0;

        for (int i = 0; i < numconsumers; i++) {
            temp_aaa += t_allConsumerPricesErrorAverage[i][j];
        }
        temp_aaa /= static_cast<double>(numconsumers);

        cout << temp_aaa << ",";
        t_valueConAverage += to_string(temp_aaa) + ",";
        t_allConsumerPricesErrorAveragePerFinal.push_back(temp_aaa);
    }

    cout << endl << endl;
    cout << "Average Max Error: " << double(*max_element(t_allConsumerPricesErrorAveragePerFinal.begin(), t_allConsumerPricesErrorAveragePerFinal.end())) << endl << endl;
    t_valueConAverage.erase(t_valueConAverage.size() - 1);

    return 0;
}
