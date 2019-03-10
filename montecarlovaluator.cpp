#include "montecarlovaluator.h"

#include <ndcalculator.h>
#include <cmath>
#include <QDebug>

montecarlovaluator::montecarlovaluator():
strikePriceAtTheBeginning_(0),
exercisePrice_(0),
driftRate_(0),
volatility_(0),
dt_(0),
maturityTime_(0),
riskFreeRate_(0),
numberOfRuns_(0),
callPrice_(0),
putPrice_(0),
binCallPrice_(0),
binPutPrice_(0),
avgStrikePrice_(0)
{
    strikePricesTrajectories_.clear();
}

montecarlovaluator::~montecarlovaluator()
{

}

void montecarlovaluator::setParameters(double st0, double exprice, double mu, double sigma, double dt, double maturity, double r, unsigned runs)
{
    strikePriceAtTheBeginning_ = st0; // strike price of the underlying financial instrument S(0)
    exercisePrice_ = exprice; // derivative strike / exercise price - K
    driftRate_ = mu;
    volatility_ = sigma;
    dt_ = dt; // time step
    maturityTime_ = maturity; // expiration time in years
    riskFreeRate_ = r;
    numberOfRuns_ = runs;
}

std::deque<double> montecarlovaluator::calculateSingleTrajectory() const
{
    double dw = 0;
    double strikePrice = strikePriceAtTheBeginning_;
    std::deque<double> trajectory;
    unsigned numberOfTimeSteps = unsigned(maturityTime_ / dt_);

    for ( unsigned i = 0; i < numberOfTimeSteps; ++i ) {

        // dW is found by random sampling of normal distribution
        dw = NDCalculator::NormInv(randomZeroToOne(), 0, std::sqrt(dt_));

        // Strike price calculation
        strikePrice = strikePrice * std::exp(driftRate_ * dt_ + volatility_ * dw);
        trajectory.push_back(strikePrice);
    }

    return trajectory;
}

void montecarlovaluator::calculatePrices()
{
    unsigned numberOfTimeSteps = unsigned(maturityTime_ / dt_); // n
    double derivativeValue = exercisePrice_ * std::exp(-riskFreeRate_ * numberOfTimeSteps * dt_); // Kc

    double strikePrice = 0;
    double putPriceCompound = 0;
    double callPriceCompound = 0;
    double binCallPriceCompound = 0;
    double binPutPriceCompound = 0;
    double avgStrikePriceCompound = 0;

    strikePricesTrajectories_.clear();

    for (unsigned i_run = 0; i_run < numberOfRuns_; ++i_run) {
        strikePricesTrajectories_.push_back(calculateSingleTrajectory());
        strikePrice = strikePricesTrajectories_.at(i_run).back();

        // Exercise price compounding
        avgStrikePriceCompound += strikePrice;

        // Put and call price compounding
        putPriceCompound += std::max( (strikePrice - derivativeValue), 0.0 );
        callPriceCompound += std::max( (derivativeValue - strikePrice), 0.0 );

        // Binary put and call price compounding;
        if ( strikePrice < derivativeValue ) binPutPriceCompound += 1.0 * std::exp(-riskFreeRate_ * numberOfTimeSteps * dt_);
        if ( strikePrice >= derivativeValue ) binCallPriceCompound += 1.0 * std::exp(-riskFreeRate_ * numberOfTimeSteps * dt_);
    }

    // Averaging after the runs

    avgStrikePrice_ = avgStrikePriceCompound / double(numberOfRuns_);
    putPrice_ = putPriceCompound / double(numberOfRuns_);
    callPrice_ = callPriceCompound / double(numberOfRuns_);
    binPutPrice_ = binPutPriceCompound / double(numberOfRuns_);

}

double montecarlovaluator::europeanPutPrice() const
{
    return putPrice_;
}

double montecarlovaluator::europeanCallPrice() const
{
    return callPrice_;
}

double montecarlovaluator::binaryPutPrice() const
{
    return binPutPrice_;
}

double montecarlovaluator::binaryCallPrice() const
{
    return binCallPrice_;
}

double montecarlovaluator::averageStrikePrice() const
{
    return avgStrikePrice_;
}

double montecarlovaluator::randomZeroToOne() const
{
    return rand() / (RAND_MAX + 1.0);
}
