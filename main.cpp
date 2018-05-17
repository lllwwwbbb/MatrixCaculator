#include "mainwindow.h"
#include <QApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Calculator cal;
    UI ui;
    Controller ctrller(&cal, &ui);

    ctrller.Run();

    return a.exec();
}
