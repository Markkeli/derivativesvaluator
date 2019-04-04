#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMainWindow>

#include "montecarlovaluator.h"
#include "blackscholesvaluator.h"

namespace Ui {
class UserInterface;
}

class UserInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = nullptr);
    ~UserInterface();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::UserInterface *ui;
    montecarlovaluator *eval;
    blackscholesvaluator *evalbs;
};

#endif // USERINTERFACE_H
