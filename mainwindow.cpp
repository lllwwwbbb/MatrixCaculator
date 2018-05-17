#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle("矩阵计算器");

    ui->lineEdit_expression->setClearButtonEnabled(true);

    ui->lineEdit_result->setEnabled(false);
    QPalette palette = ui->lineEdit_result->palette();
    palette.setColor(QPalette::Base, QColor(255,255,255));
    palette.setColor(QPalette::Text, QColor(0,0,0));
    ui->lineEdit_result->setPalette(palette);
    ui->lineEdit_result->setAlignment(Qt::AlignHCenter);

    ui->tableWidget_matrix->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listWidget_matrix->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //
    ui->lineEdit_expression->installEventFilter(this);
    ui->listWidget_matrix->viewport()->installEventFilter(this);
    ui->listWidget_matrix->installEventFilter(this);
    ui->label_variable->installEventFilter(this);
    ui->tableWidget_matrix->viewport()->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Show()
{
    this->show();
}

string MainWindow::GetExpression()
{
    QString Qexpression = ui->lineEdit_expression->text();
    string expression = Qexpression.toStdString();
    for (int i = 0; i < expression.size(); i++)
    {
        //clean the whitespace
        if (expression[i] == ' '
                || expression[i] == '\t'
                || expression[i] == '\n')
        {
            expression.erase(i--,1);
        }
    }
    return expression;
}

MatrixVar MainWindow::GetMatrixVar()
{
    MatrixVar t = this->var;
    var.varName = "\0";
    var.value.erase(var.value.begin(), var.value.end());

    return t;
}

void MainWindow::ShowError(string errorMsg, int errorPos, int errorLen)
{
    //
    ui->tableWidget_matrix->setRowCount(0);
    ui->tableWidget_matrix->setColumnCount(0);
    ui->lineEdit_result->setText("");//错误
    //
    int posL = -1, posR, stdPos = 0;
    QString expression = ui->lineEdit_expression->text();
    for (int i = 0; i < expression.size(); i++)
    {
        if (posL == -1 && stdPos == errorPos)
        {
            posL = i;
            posR = i;
        }
        else if (stdPos == errorPos + errorLen)
        {
            posR = i;
            break;
        }
        if (expression[i] != ' ' && expression[i] != '\t'
                && expression[i] !='\n')
        {
            stdPos++;
        }
    }
    if (errorLen != 0 && posR == posL)
    {
        posR = expression.size();
    }
    ui->lineEdit_expression->setFocus();
    if (posL == posR)
    {
        ui->lineEdit_expression->setCursorPosition(posL);
    }
    else
    {
        ui->lineEdit_expression->setSelection(posL, posR-posL);
    }    
    //
    QString QerrorMsg;
    QerrorMsg = QString::fromStdString(errorMsg);
    QMessageBox::warning(this, "错误", QerrorMsg);
}

void MainWindow::PutMatrixVar(MatrixVar var)
{
    // /calculate error
    for (int r = 0; r < var.value.size(); r++)
    {
        for (int c = 0; c < var.value[r].size(); c++)
        {
            if ( -DBL_MIN < var.value[r][c] &&
                 var.value[r][c] < DBL_MIN)
            {
                var.value[r][c] = 0;
            }
        }
    }
    this->var = var;
}

void MainWindow::ShowMatrix(string varName)
{
    QString QvarName = QString::fromStdString(varName);
    ui->listWidget_matrix->addItem(QvarName);
    ui->listWidget_matrix->sortItems();
}

void MainWindow::RemoveMatrix(string varName)
{
    QString QVarName = QString::fromStdString(varName);
    for (int i = 0; i < ui->listWidget_matrix->count(); i++)
    {
        if (ui->listWidget_matrix->item(i)->text() == QVarName)
        {
            delete ui->listWidget_matrix->takeItem(i);
            break;
        }
    }
}

void MainWindow::ShowResult(ExpressionElement result)
{
    QString Qresult;
    if (result.type == REAL)
    {
        ui->tableWidget_matrix->setRowCount(0);
        ui->tableWidget_matrix->setColumnCount(0);

        Qresult.setNum(result.real);
        ui->lineEdit_result->setText(Qresult);
    }
    else
    {
        Qresult = MATRIX_RESULT;
        ui->lineEdit_result->setText(Qresult);

        ui->tableWidget_matrix->setRowCount(result.matrix.size());
        ui->tableWidget_matrix->setColumnCount(result.matrix[0].size());
        for (int r = 0; r < result.matrix.size(); r++)
        {
            for (int c = 0; c < result.matrix[0].size(); c++)
            {
                QString real = QString::number(result.matrix[r][c], 'g', DBL_PEC);
                if (-DBL_MIN < real.toDouble() && real.toDouble() < DBL_MIN )
                {
                    real = "0";
                }
                ui->tableWidget_matrix->setItem(r,c, new QTableWidgetItem(real));
            }
        }
    }
    //adjust
    ui->tableWidget_matrix->resizeColumnsToContents();
}

void MainWindow::on_pushButton_calculate_clicked()
{
    SendMsg(CALCULATE);
}

void MainWindow::AddMatrix()
{
    //dialoge
    AddMatrixDlg dlg(this);
    if (AddMatrixDlg::Accepted == dlg.exec())
    {
        SendMsg(ADD_MATRIX);
    }
}

void MainWindow::AddUnitMatrix()
{
    QuickAddMatrixDlg dlg(this, ADD_UNIT);
    if (AddMatrixDlg::Accepted == dlg.exec())
    {
        SendMsg(ADD_MATRIX);
    }
}

void MainWindow::AddAllZeroMatrix()
{
    QuickAddMatrixDlg dlg(this, ADD_ZERO);
    if (AddMatrixDlg::Accepted == dlg.exec())
    {
        SendMsg(ADD_MATRIX);
    }
}

void MainWindow::AddAllOneMatrix()
{
    QuickAddMatrixDlg dlg(this, ADD_ONE);
    if (AddMatrixDlg::Accepted == dlg.exec())
    {
        SendMsg(ADD_MATRIX);
    }
}

void MainWindow::DeleteMatrxi()
{
    if (QMessageBox::No == QMessageBox::question(this, "确认", "确定删除此变量吗?"))
    {
        return;
    }
    this->var.varName = ui->listWidget_matrix->item(    //->selectedItems().constFirst()->text();
                ui->listWidget_matrix->currentRow())->text().toStdString();
    SendMsg(RMV_MATRIX);
}

void MainWindow::EditMatrix()
{
    this->var.varName = ui->listWidget_matrix->item(
                ui->listWidget_matrix->currentRow())->text().toStdString();
    SendMsg(GET_MATRIX);
    //
    MatrixVar preVar = this->var;
    AddMatrixDlg dlg(this, EDIT_MATRIX);
    if (QDialog::Accepted == dlg.exec())
    {
        MatrixVar newVar = this->var;
        this->var = preVar;
        SendMsg(RMV_MATRIX);
        this->var = newVar;
        SendMsg(FND_MATRIX);
        if (this->var.value.empty())
        {
            this->var = newVar;
            SendMsg(ADD_MATRIX);
        }
        else
        {
            this->var = preVar;
            SendMsg(ADD_MATRIX);
            this->var = newVar;
            SendMsg(ADD_MATRIX);
        }
    }
}

void MainWindow::AddFromExisted()
{
    this->var.varName = ui->listWidget_matrix->item(
                ui->listWidget_matrix->currentRow())->text().toStdString();
    SendMsg(GET_MATRIX);
    //
    AddMatrixDlg dlg(this, NEW_MATRIX);
    if (QDialog::Accepted == dlg.exec())
    {
        SendMsg(ADD_MATRIX);
    }
}

void MainWindow::AddFromResult()
{
    if (this->var.varName != MATRIX_RESULT)
    {
        QMessageBox::warning(this, "错误", "没有矩阵");
        return;
    }
//    this->var.varName.clear();

    AddMatrixDlg dlg(this, ADD_RESULT);
    if (AddMatrixDlg::Accepted == dlg.exec())
    {
        SendMsg(ADD_MATRIX);
    }
}

void MainWindow::AddFromFile()
{
    QString fName = QFileDialog::getOpenFileName(this, "导入", "..", "*.txt");
    if ( !fName.isNull() && !fName.isEmpty())
    {
        QFile file(fName);
        file.open(QIODevice::ReadOnly);
        if ( !file.isOpen())
        {
            QMessageBox::warning(this, "错误", "打开文件失败");
        }
        else
        {
            QTextStream stream(&file);
            while ( !stream.atEnd())
            {
                QString buf = stream.readLine();
                if (buf != DATA_HEAD)
                {
                    continue;
                }
                MatrixVar var;
                buf.clear();
                buf =  stream.readLine();
                var.varName = buf.toStdString();
                if (var.varName.empty())
                {
                    continue;
                }
                int rows = 0, cols = 0;
                stream >> rows >> cols;
                if (rows == 0 || cols == 0)
                {
                    continue;
                }
                for (int r = 0; r < rows; r++)
                {
                    Array arr;
                    for (int c = 0; c < cols; c++)
                    {
                        double real = 0;//default : zero
                        stream >> real;
                        arr.push_back(real);
                    }
                    var.value.push_back(arr);
                }
                this->var = var;
                SendMsg(ADD_MATRIX);
            }
            QMessageBox::information(this, "导入", "导入完成");
        }
        file.close();
    }
}

void MainWindow::PrintToFile()
{
    QString fName = QFileDialog::getSaveFileName(this, "导出", "../MATRIX.txt");
    if ( !fName.isNull() && !fName.isEmpty())
    {
        QFile file(fName);
        file.open(QIODevice::WriteOnly);
        if ( !file.isOpen())
        {
            QMessageBox::warning(this, "错误", "打开文件失败");
        }
        else
        {
            QTextStream stream(&file);
            stream.setFieldAlignment(QTextStream::AlignLeft);
            for (int i = 0; i < ui->listWidget_matrix->count(); i++)
            {
                stream.setFieldWidth(0);
                this->var.varName = ui->listWidget_matrix->item(i)->text().toStdString();
                SendMsg(GET_MATRIX);
                stream << DATA_HEAD << '\n'
                       << QString::fromStdString(this->var.varName) << '\n'
                       << this->var.value.size() << '\t'
                       << this->var.value[0].size() << '\n';
                int width = 0;
                for (int r = 0; r < this->var.value.size(); r++)
                {
                    for (int c = 0; c < this->var.value[0].size(); c++)
                    {
                        QString real = QString::number(this->var.value[r][c], 'g', DBL_PEC);
                        if (-DBL_MIN < real.toDouble() && real.toDouble() < DBL_MIN )
                        {
                            real = "0";
                        }
                        width = real.size() > width ? real.size() : width;
                    }
                }
                for (int r = 0; r < this->var.value.size(); r++)
                {
                    stream.setFieldWidth(width + 2);
                    for (int c = 0; c < this->var.value[0].size(); c++)
                    {
                        QString real = QString::number(this->var.value[r][c], 'g', DBL_PEC);
                        if (-DBL_MIN < real.toDouble() && real.toDouble() < DBL_MIN )
                        {
                            real = "0";
                        }
                        stream << real;
                    }
                    stream.setFieldWidth(0);
                    stream << '\n';
                }
                stream << '\n';
            }
            QMessageBox::information(this, "导出", "导出完成");
        }
        file.close();
    }
}

void MainWindow::on_listWidget_matrix_itemDoubleClicked(QListWidgetItem *item)
{
    QString varName = item->text();
    QString expression = ui->lineEdit_expression->text();
    int pos = ui->lineEdit_expression->cursorPosition();
    expression.insert(pos, varName);
    ui->lineEdit_expression->setText(expression);
    ui->lineEdit_expression->setCursorPosition(pos + varName.size());
    ui->lineEdit_expression->setFocus();
}

void MainWindow::on_listWidget_matrix_pressed(const QModelIndex &index)
{
    if (qApp->mouseButtons() == Qt::RightButton)
    {
        QMenu menu;
        menu.addAction("编辑", this, &MainWindow::EditMatrix);
        menu.addAction("生成新矩阵", this, &MainWindow::AddFromExisted);
        menu.addAction("删除", this, &MainWindow::DeleteMatrxi);
        menu.exec(QCursor::pos());
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if ((obj == ui->listWidget_matrix->viewport() || obj == ui->label_variable)
            && event->type() == QEvent::MouseButtonPress
            && qApp->mouseButtons() == Qt::RightButton)
    {
        if (obj == ui->label_variable
                || ui->listWidget_matrix->itemAt(
                    ui->listWidget_matrix->mapFromGlobal(QCursor::pos())) == NULL)
        {
            QMenu menu, quickMenu;
            menu.addAction("键入矩阵", this, &MainWindow::AddMatrix);
            menu.addAction("从文件导入", this, &MainWindow::AddFromFile);
            quickMenu.setTitle("快速添加");
            quickMenu.addAction("单位矩阵", this, &MainWindow::AddUnitMatrix);
            quickMenu.addAction("全0矩阵", this, &MainWindow::AddAllZeroMatrix);
            quickMenu.addAction("全1矩阵", this, &MainWindow::AddAllOneMatrix);
            menu.addMenu(&quickMenu);
            menu.addAction("导出到文件", this, &MainWindow::PrintToFile);
            menu.exec(QCursor::pos());
            return true;
        }
    }
    if (obj == ui->tableWidget_matrix->viewport()
            && event->type() == QEvent::MouseButtonPress
            && qApp->mouseButtons() == Qt::RightButton)
    {
        QMenu menu;
        menu.addAction("添加到矩阵变量", this, &MainWindow::AddFromResult);
        menu.exec(QCursor::pos());
        return true;
    }
    if (obj == ui->lineEdit_expression
            && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *kEvent = (QKeyEvent *)event;
        if (kEvent->key() == Qt::Key_Tab)
        {
            if (ui->listWidget_matrix->count() > 0)
            {
                ui->listWidget_matrix->setFocus();
                ui->listWidget_matrix->item(0)->setSelected(true);
                return true;
            }
        }
    }
    if (obj == ui->listWidget_matrix
            && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *kEvent = (QKeyEvent *)event;
        if (kEvent->key() == Qt::Key_Return)
        {
            if ( !ui->listWidget_matrix->selectedItems().empty())
            {
                QListWidgetItem *item = ui->listWidget_matrix->selectedItems()[0];
                this->on_listWidget_matrix_itemDoubleClicked(item);
            }
        }
    }
    if (obj == ui->lineEdit_expression
            && event->type() == QEvent::MouseButtonPress
            && qApp->mouseButtons() == Qt::LeftButton)
    {
        if (ui->checkBox_inputdlg->checkState() == Qt::Checked)
        {
            if (this->pInputDlg.isNull())
            {
                this->pInputDlg = new ExpInputDlg(ui->lineEdit_expression);
                this->pInputDlg.data()->show();
                this->pInputDlg.data()->setAttribute(Qt::WA_DeleteOnClose);
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::on_lineEdit_expression_returnPressed()
{
    this->on_pushButton_calculate_clicked();
}
