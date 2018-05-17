#ifndef EXPINPUTDLG_H
#define EXPINPUTDLG_H

#include <QDialog>

namespace Ui {
class ExpInputDlg;
}

class ExpInputDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ExpInputDlg(QWidget *parent);
    ~ExpInputDlg();

private:
    void AddCharToParent(QChar ch);

private slots:
    void on_pushButton_0_clicked();
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_point_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_plus_clicked();
    void on_pushButton_substract_clicked();
    void on_pushButton_multiply_clicked();
    void on_pushButton_divide_clicked();
    void on_pushButton_reverse_clicked();
    void on_pushButton_leftB_clicked();
    void on_pushButton_rightB_clicked();

private:
    Ui::ExpInputDlg *ui;
};

#endif // EXPINPUTDLG_H
