#include "quickaddmatrixdlg.h"
#include "ui_quickaddmatrixdlg.h"
#include <QMessageBox>

QuickAddMatrixDlg::QuickAddMatrixDlg(QWidget *parent, DLG_MODLE dlgModel) :
    QDialog(parent), model(dlgModel),
    ui(new Ui::QuickAddMatrixDlg)
{
    ui->setupUi(this);
    this->setWindowTitle("快速添加");
    this->setFixedSize(this->size());

    QRegExp regExp(REGEXP_RC);
    QRegExpValidator *pRegExpValidator = new QRegExpValidator(regExp, this);
    ui->lineEdit_row->setValidator(pRegExpValidator);
    ui->lineEdit_col->setValidator(pRegExpValidator);
    //
    QRegExp regExp2(REGEXP_VARNAME);
    QRegExpValidator *pRegExpValidator2 = new QRegExpValidator(regExp2, this);
    ui->lineEdit_name->setValidator(pRegExpValidator2);
    //
}

QuickAddMatrixDlg::~QuickAddMatrixDlg()
{
    delete ui;
}

void QuickAddMatrixDlg::on_lineEdit_row_textChanged(const QString &arg1)
{
    if (model == ADD_UNIT)
    {
        ui->lineEdit_col->setText(arg1);
    }
}

void QuickAddMatrixDlg::on_lineEdit_col_textChanged(const QString &arg1)
{
    if (model == ADD_UNIT)
    {
        ui->lineEdit_row->setText(arg1);
    }
}

void QuickAddMatrixDlg::on_pushButton_add_clicked()
{
    if (ui->lineEdit_name->text().isEmpty())
    {
        QMessageBox::warning(this, "未完成", "请输入变量名");
        return;
    }
    if (ui->lineEdit_row->text().isEmpty())
    {
        QMessageBox::warning(this, "未完成", "请输入行数");
        return;
    }
    if (ui->lineEdit_col->text().isEmpty())
    {
        QMessageBox::warning(this, "未完成", "请输入列数");
        return;
    }
    ///.
    ///
    MatrixVar var;
    var.varName = ui->lineEdit_name->text().toStdString();
    for (int r = 0; r < ui->lineEdit_row->text().toInt(); r++)
    {
        Array arr;
        for (int c = 0; c < ui->lineEdit_col->text().toInt(); c++)
        {
            if  (ADD_UNIT == this->model)
            {
                if (r == c)
                {
                    arr.push_back(1);
                }
                else
                {
                    arr.push_back(0);
                }
            }
            else if (ADD_ZERO == this->model)
            {
                arr.push_back(0);
            }
            else if (ADD_ONE == this->model)
            {
                arr.push_back(1);
            }
            else
            {
                arr.push_back(0);
            }
        }
        var.value.push_back(arr);
    }
    MainWindow *pM = (MainWindow *)this->parent();
    pM->var = var;

    this->accept();
}

void QuickAddMatrixDlg::on_pushButton_cancel_clicked()
{
    this->reject();
}
