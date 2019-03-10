#include "userinterface.h"
#include <QApplication>

#include <montecarlovaluator.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserInterface w;
    w.show();
    montecarlovaluator valuator;
    double dt = 1.0/3650.0; // one day timestep in years
    double maturity = 4; // in years
    double S_0 = 100.0;
    double K = 90.0;
    double volatility = 0.1;
    double driftRate = -0.5 * volatility * volatility;
    double r = 0.1;

    valuator.setParameters(S_0, K, driftRate, volatility, dt,  maturity, r, 1000);

    valuator.calculatePrices();
    double binput = valuator.binaryPutPrice();
    double bincall = valuator.binaryCallPrice();
    double call = valuator.europeanCallPrice();
    double put = valuator.europeanPutPrice();
    double avgstrike = valuator.averageStrikePrice();

    qDebug() << "put:" << put << "\n";
    qDebug() << "call:" << call << "\n";
    qDebug() << "binary put:" << binput << "\n";
    qDebug() << "binary call:" << bincall << "\n";
    qDebug() << "average strike price:" << avgstrike << "\n";

    // TODO  - QCUSTOMPLOT VISUALIZATION

    return a.exec();
}
