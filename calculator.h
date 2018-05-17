#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "operate.h"
#include "matrix.h"

class Calculator
{
private:
    vector<Operator> optrLib;
    vector<MatrixVar> matrixLib;
    int errorLen;
    string errorMsg;
private:
    int CheckChar(string expression);
    int CheckBracket(string expression);
    int StdTranslation(string expresion, vector<ExpressionElement>& stdExpression);
    int CheckCalculate(vector<ExpressionElement> stdExpression);
    int FindErrorPos(string expression, int elePos);
    void PreTranslation(string& expression);
    ExpressionElement StdCalculate(vector<ExpressionElement> stdExpession);
    ExpressionElement Operate(ExpressionElement left, Operator optr, ExpressionElement right);
    double Determinant(Matrix matrix);
    Matrix ReverseMatrix(Matrix matrix);

public:
    Calculator();
    int Check(string expression);
    void GetError(string& errorMsg, int& errorLen);
    ExpressionElement Calculate(string expression); //expression without whitespace
    bool AddMatrix(MatrixVar var);
    bool DeleteMatrix(string varName);
};

#endif // CALCULATOR_H
