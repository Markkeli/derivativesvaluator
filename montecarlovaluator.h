#ifndef MONTECARLOVALUATOR_H
#define MONTECARLOVALUATOR_H

#include <deque>
#include <stdlib.h>


class montecarlovaluator
{
public:
    // Constructor class
    montecarlovaluator();

    // Destructor class
    ~montecarlovaluator();

    void setParameters(double st0, double exprice, double mu,
                       double sigma, double dt, double maturity,
                       double r, unsigned runs);

    // Calculates one singular trajectory for the strike price
    std::deque<double> calculateSingleTrajectory() const;

    // Calculates the
    void calculatePrices();

    double europeanPutPrice() const;
    double europeanCallPrice() const;
    double binaryPutPrice() const;
    double binaryCallPrice() const;
    double averageStrikePrice() const;

private:
    // helper function
    double randomZeroToOne() const;

    // input parameters
    double strikePriceAtTheBeginning_; // strike price of the underlying financial instrument S(0)
    double exercisePrice_; // derivative strike / exercise price - K
    double driftRate_;
    double volatility_;
    double dt_; // time step
    double maturityTime_; // expiration time in years
    double riskFreeRate_;
    unsigned numberOfRuns_;

    // internal variables
    std::deque<std::deque<double>> strikePricesTrajectories_; // Storage for the current strike prices S(t) of the underlying financial instrument

    // output values
    double callPrice_;
    double putPrice_;
    double binCallPrice_;
    double binPutPrice_;
    double avgStrikePrice_;

};

#endif // MONTECARLOVALUATOR_H
