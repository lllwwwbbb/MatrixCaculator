#include "expinputdlg.h"
#include "ui_expinputdlg.h"
#include <QLineEdit>
#include "mainwindow.h"

ExpInputDlg::ExpInputDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExpInputDlg)
{
    ui->setupUi(this);
    this->setWindowTitle("小键盘");
    this->setFixedSize(this->size());

    QPoint pos = parent->pos();
    pos.setX(parent->mapToGlobal(pos).x());
    pos.setY(parent->mapToGlobal(pos).y());// + parent->height());
    this->move(pos);
}

ExpInputDlg::~ExpInputDlg()
{
    delete ui;
}

void ExpInputDlg::AddCharToParent(QChar ch)
{
    QLineEdit *pLineEdit = (QLineEdit *)this->parent();
    QString text = pLineEdit->text();
    int pos = pLineEdit->cursorPosition();
    text.insert(pos, ch);
    pLineEdit->setText(text);
    pLineEdit->setCursorPosition(pos + 1);
//    MainWindow *pM = (MainWindow *)pLineEdit->parent();
//    pM->activateWindow();
//    pLineEdit->setFocus();
}

void ExpInputDlg::on_pushButton_back_clicked()
{
    QLineEdit *pLineEdit = (QLineEdit *)this->parent();
    QString text = pLineEdit->text();
    int pos = pLineEdit->cursorPosition();
    if (text.size() > 0 && pos > 0)
    {
        text.remove(pos-1, 1);
        pLineEdit->setText(text);
        pLineEdit->setCursorPosition(pos-1);
//        MainWindow *pM = (MainWindow *)pLineEdit->parent();
//        pM->activateWindow();
//        pLineEdit->setFocus();
    }
}

void ExpInputDlg::on_pushButton_clear_clicked()
{
    QLineEdit *pLineEdit = (QLineEdit *)this->parent();
    if ( !pLineEdit->text().isEmpty())
    {
        pLineEdit->clear();
//        MainWindow *pM = (MainWindow *)pLineEdit->parent();
//        pM->activateWindow();
//        pLineEdit->setFocus();
    }
}

void ExpInputDlg::on_pushButton_0_clicked()
{
    this->AddCharToParent('0');
}

void ExpInputDlg::on_pushButton_1_clicked()
{
    this->AddCharToParent('1');
}

void ExpInputDlg::on_pushButton_2_clicked()
{
    this->AddCharToParent('2');
}

void ExpInputDlg::on_pushButton_3_clicked()
{
    this->AddCharToParent('3');
}

void ExpInputDlg::on_pushButton_4_clicked()
{
    this->AddCharToParent('4');
}

void ExpInputDlg::on_pushButton_5_clicked()
{
    this->AddCharToParent('5');
}

void ExpInputDlg::on_pushButton_6_clicked()
{
    this->AddCharToParent('6');
}

void ExpInputDlg::on_pushButton_7_clicked()
{
    this->AddCharToParent('7');
}

void ExpInputDlg::on_pushButton_8_clicked()
{
    this->AddCharToParent('8');
}

void ExpInputDlg::on_pushButton_9_clicked()
{
    this->AddCharToParent('9');
}

void ExpInputDlg::on_pushButton_point_clicked()
{
    this->AddCharToParent('.');
}

void ExpInputDlg::on_pushButton_plus_clicked()
{
    this->AddCharToParent('+');
}

void ExpInputDlg::on_pushButton_substract_clicked()
{
    this->AddCharToParent('-');
}

void ExpInputDlg::on_pushButton_multiply_clicked()
{
    this->AddCharToParent('*');
}

void ExpInputDlg::on_pushButton_divide_clicked()
{
    this->AddCharToParent('/');
}

void ExpInputDlg::on_pushButton_reverse_clicked()
{
    this->AddCharToParent('|');
}

void ExpInputDlg::on_pushButton_leftB_clicked()
{
    this->AddCharToParent('(');
}

void ExpInputDlg::on_pushButton_rightB_clicked()
{
    this->AddCharToParent(')');
}
