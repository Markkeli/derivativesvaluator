#include <deque>

#include "userinterface.h"
#include "ui_userinterface.h"

UserInterface::UserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInterface),
    eval(new montecarlovaluator),
    evalbs(new blackscholesvaluator)

{
    ui->setupUi(this);
    this->setFixedSize(1300,800);
    ui->label_5->setText("                      ");
    ui->label_6->setText("                      ");
    ui->label_7->setText("                      ");
    ui->label_8->setText("                      ");
    ui->label_9->setText("                      ");
    ui->plotWidget->xAxis->setLabel("Time (years)");
    ui->plotWidget->yAxis->setLabel("Price (unit of currency)");
}

UserInterface::~UserInterface()
{
    delete ui;
    delete eval;
    delete evalbs;
}

void UserInterface::on_pushButton_clicked()
{

    double dt = ui->timeStep->text().toDouble()/365.0; // conversion from days to years
    double maturity = ui->maturity->text().toDouble(); // in years
    double S_0 = ui->S_0->text().toDouble();
    double K = ui->K->text().toDouble();
    double volatility = ui->volatility->text().toDouble()*0.01; // from percentage to p.u
    double driftRate = -0.5 * volatility * volatility;
    double r = ui->riskFree->text().toDouble()*0.01; // from percentage to p.u
    unsigned maxRuns = unsigned(ui->NOofRuns->text().toInt());
    std::deque<double> trajectory;
    trajectory.clear();
    QVector<double> qtrajectory;
    QVector<double> x;

    eval->setParameters(S_0, K, driftRate, volatility, dt,  maturity, r);

    ui->plotWidget->clearGraphs();

    for (unsigned i = 0; i < maxRuns; ++i) {
        trajectory = eval->calculatePrices(i);
        qtrajectory.clear();
        x.clear();

        for (unsigned i = 0; i < trajectory.size(); ++i) {
            qtrajectory.push_back(trajectory.at(i));
            x.push_back(dt * i);
        }
        ui->plotWidget->addGraph();

        ui->plotWidget->graph()->setData(x, qtrajectory);
        ui->plotWidget->graph()->setPen(QPen(Qt::blue));
        ui->plotWidget->rescaleAxes();
        ui->plotWidget->replot();

        double binput = eval->binaryPutPrice();
        double bincall = eval->binaryCallPrice();
        double call = eval->europeanCallPrice();
        double put = eval->europeanPutPrice();
        double avgstrike = eval->averageStrikePrice();

        ui->label_5->setText(QString::number(put));
        ui->label_6->setText(QString::number(call));
        ui->label_7->setText(QString::number(binput));
        ui->label_8->setText(QString::number(bincall));
        ui->label_9->setText(QString::number(avgstrike));
    }

    ui->label_9->setStyleSheet("font-weight: bold");

    eval->reset();

}

void UserInterface::on_pushButton_2_clicked()
{

    double maturity = ui->maturity->text().toDouble(); // in years
    double S_0 = ui->S_0->text().toDouble();
    double K = ui->K->text().toDouble();
    double volatility = ui->volatility->text().toDouble()*0.01; // from persentage to p.u
    double r = ui->riskFree->text().toDouble()*0.01;
    evalbs->setParameters(S_0, K, volatility, maturity, r);
    evalbs->calculatePrices();

    double binput = evalbs->binaryPutPrice();
    double bincall = evalbs->binaryCallPrice();
    double call = evalbs->europeanCallPrice();
    double put = evalbs->europeanPutPrice();

    ui->label_32->setText(QString::number(put));
    ui->label_34->setText(QString::number(call));
    ui->label_36->setText(QString::number(binput));
    ui->label_38->setText(QString::number(bincall));
}
