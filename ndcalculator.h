#ifndef NORMALDISTRIBUTIONCONFIDENCECALCULATOR_H
#define NORMALDISTRIBUTIONCONFIDENCECALCULATOR_H

// This class is used to calculate the inverse normal distribution
//
class NDCalculator
{
public:
    NDCalculator();
    ~NDCalculator();

    static double inverseNormalDistribution(double probability, double min, double max);
    static double NormalDistribution(double x, double mean, double sigma);
    static double NormInv(double probability);

    static double NormInv(double probability, double mean, double sigma);

};

#endif // NORMALDISTRIBUTIONCONFIDENCECALCULATOR_H
