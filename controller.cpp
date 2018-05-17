
#include "controller.h"

Controller::Controller(Calculator *pCal, UI *pUi)
    :cal(pCal), ui(pUi)
{
    QObject::connect(ui, &UI::SendMsg,
                     this, &Controller::RcvMsg);
}

void Controller::Run()
{
    ui->Show();
}

void Controller::RcvMsg(ACTION_MSG msg)
{
    switch (msg) {
    case ADD_MATRIX:
    {
        MatrixVar var = ui->GetMatrixVar();
        if ( !cal->AddMatrix(var))
        {
            string errorMsg;
            int t;
            cal->GetError(errorMsg, t);
            ui->ShowError(errorMsg);
        }
        else
        {
            ui->ShowMatrix(var.varName);
        }
    }
        break;
    case RMV_MATRIX:
    {
        string varName = ui->GetMatrixVar().varName;
        if ( !cal->DeleteMatrix(varName))
        {
            string errorMsg;
            int t;
            cal->GetError(errorMsg, t);
            ui->ShowError(errorMsg);
        }
        else
        {
            ui->RemoveMatrix(varName);
        }
    }
        break;
    case GET_MATRIX:
    {
        string varName = ui->GetMatrixVar().varName;
        int errorPos = cal->Check(varName);
        if (-1 != errorPos)
        {
            ui->ShowError("not find the matrix" + varName);
        }
        else
        {
            ExpressionElement result = cal->Calculate(varName);
            MatrixVar var;
            var.varName = varName;
            var.value = result.matrix;
            ui->PutMatrixVar(var);
        }
    }
        break;
    case FND_MATRIX:
    {
        string varName = ui->GetMatrixVar().varName;
        int errorPos = cal->Check(varName);
        MatrixVar var;
        var.varName = varName;
        var.value.clear();
        if (-1 != errorPos)
        {
            ui->PutMatrixVar(var);
        }
        else
        {
            ExpressionElement result = cal->Calculate(varName);
            var.value = result.matrix;
            ui->PutMatrixVar(var);
        }
    }
        break;
    case CALCULATE:
    {
        string expression = ui->GetExpression();
        int errorPos = cal->Check(expression);
        if (-1 != errorPos)
        {
            string errorMsg;
            int errorLen;
            cal->GetError(errorMsg, errorLen);
            ui->ShowError(errorMsg, errorPos, errorLen);
        }
        else
        {
            ExpressionElement result = cal->Calculate(expression);
            if (result.type == MATRIX)
            {
                MatrixVar resultVar;
                resultVar.varName = MATRIX_RESULT;
                resultVar.value = result.matrix;
                ui->PutMatrixVar(resultVar);
            }
            ui->ShowResult(result);
        }
    }
        break;
    default:
        break;
    }
}
