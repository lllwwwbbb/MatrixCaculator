#include "addmatrixdlg.h"
#include "ui_addmatrixdlg.h"
#include <QItemDelegate>
#include <QMessageBox>
#include "realeditdelegate.h"

AddMatrixDlg::AddMatrixDlg(QWidget *parent, unsigned char dlgModel) :
    QDialog(parent), ui(new Ui::AddMatrixDlg),
    model(dlgModel)

{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());
    this->setWindowTitle("添加");

    QRegExp RegExp(REGEXP_RC);
    QRegExpValidator *pRegValidator = new QRegExpValidator(RegExp, this);
    ui->lineEdit_row->setValidator(pRegValidator);
    ui->lineEdit_col->setValidator(pRegValidator);
    //
    QRegExp RegExp2(REGEXP_VARNAME);
    QRegExpValidator *pRegValidator2 = new QRegExpValidator(RegExp2, this);
    ui->lineEdit_name->setValidator(pRegValidator2);
    //
    RealEditDelegate *pDelegator = new RealEditDelegate(ui->tableWidget_matrix);
    ui->tableWidget_matrix->setItemDelegate(pDelegator);
    //
    if (this->model == EDIT_MATRIX
            || this->model == ADD_RESULT
            || this->model == NEW_MATRIX)
    {
        MainWindow *pM = (MainWindow *)this->parent();
        MatrixVar var = pM->var;
        ui->lineEdit_name->setText(QString::fromStdString(var.varName));
        ui->lineEdit_row->setText(QString::number(var.value.size()));
        ui->lineEdit_col->setText(QString::number(var.value[0].size()));
        ui->tableWidget_matrix->setRowCount(var.value.size());
        ui->tableWidget_matrix->setColumnCount(var.value[0].size());
        for (int r = 0; r < var.value.size(); r++)
        {
            for (int c = 0; c < var.value[0].size(); c++)
            {
                ui->tableWidget_matrix->setItem(r, c,
                        new QTableWidgetItem(QString::number(var.value[r][c])));
            }
        }
        ui->tableWidget_matrix->resizeColumnsToContents();
        if (this->model == EDIT_MATRIX)
        {
            ui->pushButton_add->setText("确定");
            this->setWindowTitle("编辑");
        }
        if (this->model == NEW_MATRIX
                || this->model == ADD_RESULT)
        {
            ui->lineEdit_name->setSelection(0, ui->lineEdit_name->text().size());
        }
    }
}

AddMatrixDlg::~AddMatrixDlg()
{
    delete ui;
}

void AddMatrixDlg::on_pushButton_add_clicked()
{
    if (ui->lineEdit_name->text().isEmpty())
    {
        QMessageBox::warning(this, "缺少名字", "请输入矩阵变量名");
        return;
    }

    MatrixVar var;
    var.varName = ui->lineEdit_name->text().toStdString();
    for (int r = 0; r < ui->tableWidget_matrix->rowCount(); r++)
    {
        Array arr;
        for (int c = 0; c < ui->tableWidget_matrix->columnCount(); c++)
        {
            if (ui->tableWidget_matrix->item(r, c) == NULL
                || ui->tableWidget_matrix->item(r, c)->text().isNull()
                || ui->tableWidget_matrix->item(r, c)->text().isEmpty())
            {
                QMessageBox::warning(this, "未完成输入", "请输入所有的矩阵元素");
                return;
            }
            else
            {
                QString real = ui->tableWidget_matrix->item(r, c)->text();
                arr.push_back(real.toDouble());
            }
        }
        var.value.push_back(arr);
    }

    MainWindow *pM = (MainWindow *)this->parentWidget();
    pM->var = var;
    this->accept();
}

void AddMatrixDlg::on_pushButton_cancle_clicked()
{
    this->reject();
}

void AddMatrixDlg::on_lineEdit_row_editingFinished()
{
    QString nRow = ui->lineEdit_row->text();
    ui->tableWidget_matrix->setRowCount(nRow.toInt());
}

void AddMatrixDlg::on_lineEdit_col_editingFinished()
{
    QString nCol = ui->lineEdit_col->text();
    ui->tableWidget_matrix->setColumnCount(nCol.toInt());
}

void AddMatrixDlg::on_tableWidget_matrix_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    ui->tableWidget_matrix->resizeColumnsToContents();
}
