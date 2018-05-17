#ifndef QUICKADDMATRIXDLG_H
#define QUICKADDMATRIXDLG_H

#include <QDialog>
#include "def.h"
#include "mainwindow.h"

namespace Ui {
class QuickAddMatrixDlg;
}

class QuickAddMatrixDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QuickAddMatrixDlg(QWidget *parent, DLG_MODLE dlgModel);
    ~QuickAddMatrixDlg();

private slots:
    void on_lineEdit_row_textChanged(const QString &arg1);

    void on_lineEdit_col_textChanged(const QString &arg1);

    void on_pushButton_add_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::QuickAddMatrixDlg *ui;
    DLG_MODLE model;
};

#endif // QUICKADDMATRIXDLG_H
