#ifndef ADDMATRIXDLG_H
#define ADDMATRIXDLG_H

#include <QDialog>
#include "def.h"
#include "mainwindow.h"

namespace Ui {
class AddMatrixDlg;
}

class AddMatrixDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddMatrixDlg(QWidget *parent, DLG_MODLE dlgModel = ADD_MATRIX);
    ~AddMatrixDlg();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_cancle_clicked();

    void on_lineEdit_row_editingFinished();

    void on_lineEdit_col_editingFinished();

    void on_tableWidget_matrix_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    Ui::AddMatrixDlg *ui;
    DLG_MODLE model;
};

#endif // ADDMATRIXDLG_H
