#ifndef CONTROLLER_H
#define CONTROLLER_H
//Controller

#include "def.h"
#include "calculator.h"
#include <QObject>

#include "mainwindow.h"
typedef MainWindow UI;

class Controller : public QObject
{
    Q_OBJECT

    Calculator *cal;
    UI *ui;

private:

public:
    Controller(Calculator *pCal, UI *pUi);
    void Run();
    void RcvMsg(ACTION_MSG msg);
};


#endif // CONTROLLER_H
