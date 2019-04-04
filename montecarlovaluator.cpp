
#include <cmath>


#include <ndcalculator.h>
#include <montecarlovaluator.h>


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
avgStrikePrice_(0),
putPriceCompound_(0),
callPriceCompound_(0),
binCallPriceCompound_(0),
binPutPriceCompound_(0),
avgStrikePriceCompound_(0)
{
}

montecarlovaluator::~montecarlovaluator()
{

}

void montecarlovaluator::setParameters(double st0, double exprice, double mu, double sigma, double dt, double maturity, double r)
{
    strikePriceAtTheBeginning_ = st0; // strike price of the underlying financial instrument S(0)
    exercisePrice_ = exprice; // derivative strike / exercise price - K
    driftRate_ = mu;
    volatility_ = sigma;
    dt_ = dt; // time step
    maturityTime_ = maturity; // expiration time in years
    riskFreeRate_ = r;
}

void montecarlovaluator::reset()
{
    putPriceCompound_ = 0;
    callPriceCompound_ = 0;
    binCallPriceCompound_ = 0;
    binPutPriceCompound_ = 0;
    avgStrikePriceCompound_ = 0;
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

// Calculates one singular trajectory for the strike price
std::deque<double> montecarlovaluator::calculatePrices(const unsigned & runningNumber)
{
    unsigned numberOfTimeSteps = unsigned(maturityTime_ / dt_); // n
    double derivativeValue = exercisePrice_ * std::exp(-riskFreeRate_ * numberOfTimeSteps * dt_); // Kc

    std::deque<double> strikePriceTrajectory;
    strikePriceTrajectory.clear();
    double strikePrice = 0;

    strikePriceTrajectory = calculateSingleTrajectory();
    strikePrice = strikePriceTrajectory.back();

    // Exercise price compounding
    avgStrikePriceCompound_ += strikePrice;

    // Put and call price compounding
    putPriceCompound_ += std::max( (derivativeValue - strikePrice), 0.0 );
    callPriceCompound_ += std::max( (strikePrice - derivativeValue), 0.0 );

    // Binary put and call price compounding;
    if ( strikePrice < derivativeValue ) binPutPriceCompound_ += 1.0 * std::exp(-riskFreeRate_ * numberOfTimeSteps * dt_);
    if ( strikePrice >= derivativeValue ) binCallPriceCompound_ += 1.0 * std::exp(-riskFreeRate_ * numberOfTimeSteps * dt_);


    // Averaging after the runs

    avgStrikePrice_ = avgStrikePriceCompound_ / double(runningNumber+1);
    putPrice_ = putPriceCompound_ / double(runningNumber+1);
    callPrice_ = callPriceCompound_ / double(runningNumber+1);
    binPutPrice_ = binPutPriceCompound_ / double(runningNumber+1);
    binCallPrice_ = binCallPriceCompound_ / double(runningNumber+1);

    return strikePriceTrajectory;
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
