#-------------------------------------------------
#
# Project created by QtCreator 2016-05-23T18:14:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MatrixCalculator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    calculator.cpp \
    controller.cpp \
    addmatrixdlg.cpp \
    realeditdelegate.cpp \
    quickaddmatrixdlg.cpp \
    expinputdlg.cpp

HEADERS  += mainwindow.h \
    calculator.h \
    def.h \
    matrix.h \
    operate.h \
    controller.h \
    addmatrixdlg.h \
    realeditdelegate.h \
    quickaddmatrixdlg.h \
    expinputdlg.h

FORMS    += mainwindow.ui \
    addmatrixdlg.ui \
    quickaddmatrixdlg.ui \
    expinputdlg.ui
