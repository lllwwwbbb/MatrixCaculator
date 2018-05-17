#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "matrix.h"
#include "operate.h"
#include "def.h"
#include "addmatrixdlg.h"
#include "quickaddmatrixdlg.h"
#include <QMouseEvent>
#include <QEvent>
#include "expinputdlg.h"
#include <QPointer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void SendMsg(ACTION_MSG msg);

private:
    bool eventFilter(QObject *obj, QEvent *event);
    void AddMatrix();
    void AddFromFile();
    void PrintToFile();
    void DeleteMatrxi();
    void EditMatrix();
    void AddFromResult();
    void AddFromExisted();
    void AddAllZeroMatrix();
    void AddAllOneMatrix();
    void AddUnitMatrix();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Show();
    string GetExpression();
    MatrixVar GetMatrixVar();
    void PutMatrixVar(MatrixVar var);
    void ShowResult(ExpressionElement result);
    void ShowError(string errorMsg, int errorPos = 0, int errorLen = 0);
    void ShowVariable(vector<string> varName);
    void ShowMatrix(string varName);
    void RemoveMatrix(string varName);

private slots:
    void on_pushButton_calculate_clicked();

    void on_listWidget_matrix_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_matrix_pressed(const QModelIndex &index);

    void on_lineEdit_expression_returnPressed();

private:
    Ui::MainWindow *ui;
    QPointer<ExpInputDlg> pInputDlg;

    friend class AddMatrixDlg;
    friend class QuickAddMatrixDlg;
protected:
    MatrixVar var;
};

#endif // MAINWINDOW_H
