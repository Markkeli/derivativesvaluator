#ifndef BLACKSCHOLESVALUATOR_H
#define BLACKSCHOLESVALUATOR_H

// Pure c++ class for usability elsewhere

class blackscholesvaluator
{
public:

    // Constructor
    blackscholesvaluator();

    // Destructor
   ~blackscholesvaluator();

    void setParameters(double st0, double exprice,
                       double sigma, double maturity,
                       double r);

    // Calculates the option prices
    void calculatePrices();

    double europeanPutPrice() const;
    double europeanCallPrice() const;
    double binaryPutPrice() const;
    double binaryCallPrice() const;

private:
    // input parameters
    double strikePriceAtTheBeginning_; // strike price of the underlying financial instrument S(0)
    double exercisePrice_; // derivative strike price or exercise price = K
    double volatility_;
    double maturityTime_; // expiration time in years
    double riskFreeRate_;

    // output values
    double callPrice_;
    double putPrice_;
    double binCallPrice_;
    double binPutPrice_;

};

#endif // BLACKSCHOLESVALUATOR_H
