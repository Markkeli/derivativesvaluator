#include "blackscholesvaluator.h"

#include <cmath>

blackscholesvaluator::blackscholesvaluator():
strikePriceAtTheBeginning_(0),
exercisePrice_(0),
volatility_(0),
maturityTime_(0),
riskFreeRate_(0),
callPrice_(0),
putPrice_(0),
binCallPrice_(0),
binPutPrice_(0)
{

}

blackscholesvaluator::~blackscholesvaluator()
{

}

void blackscholesvaluator::setParameters(double st0, double exprice, double sigma, double maturity, double r)
{
    strikePriceAtTheBeginning_ = st0;
    exercisePrice_ = exprice;
    volatility_ = sigma;
    maturityTime_ = maturity;
    riskFreeRate_ = r;
}

void blackscholesvaluator::calculatePrices()
{
    double d1, d2;
    double Nd1, Nd2, Nnd1, Nnd2;

    // Lambda returning the normal cumulative distribution functio
    auto normsdist = [](double d){ return (0.5*(std::erf(d/std::sqrt(2))+1)); };

    // Black-scholes formula factors
    d1 = (log(strikePriceAtTheBeginning_/exercisePrice_) +
          (riskFreeRate_ + 0.5*std::pow(volatility_, 2))*maturityTime_)
            /
         (volatility_*std::sqrt(maturityTime_));

    d2 = (log(strikePriceAtTheBeginning_/exercisePrice_) +
          (riskFreeRate_ - 0.5*std::pow(volatility_, 2))*maturityTime_)
            /
         (volatility_*std::sqrt(maturityTime_));

    // N(d1) etc...
    Nd1 = normsdist(d1);
    Nd2 = normsdist(d2);
    Nnd1 = normsdist(-d1);
    Nnd2 = normsdist(-d2);

    putPrice_ = exercisePrice_*Nnd2*std::exp(-riskFreeRate_*maturityTime_) - strikePriceAtTheBeginning_*Nnd1;
    callPrice_ = strikePriceAtTheBeginning_*Nd1 - exercisePrice_*Nd2*std::exp(-riskFreeRate_*maturityTime_);
    binPutPrice_ = std::exp(-riskFreeRate_*maturityTime_)*(-Nnd2);
    binCallPrice_ = std::exp(-riskFreeRate_*maturityTime_)*Nd2;

}

double blackscholesvaluator::europeanPutPrice() const
{
    if (putPrice_ < 0) return 0;
    else return putPrice_;
}

double blackscholesvaluator::europeanCallPrice() const
{
    if (callPrice_ < 0) return 0;
    else return callPrice_;
}

double blackscholesvaluator::binaryPutPrice() const
{
    if (binPutPrice_ < 0) return 0;
    else return binPutPrice_;
}

double blackscholesvaluator::binaryCallPrice() const
{
    if (binCallPrice_ < 0) return 0;
    else return binCallPrice_;
}
