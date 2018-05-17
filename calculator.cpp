#include "calculator.h"

#include <cstdlib>
#include <stack>
#include <algorithm>
#include <sstream>
using namespace std;

Calculator::Calculator()
{
    Operator preLib[] =
    {
        {'*', 3}, {'/', 3}, {'+', 4}, {'-', 4}, {'|', 2}, {'(', 1}, {')', 9}
    };
    int size = 7;
    for (int i = 0; i < size; i++)
    {
        optrLib.push_back(preLib[i]);
    }
}
ExpressionElement Calculator::Calculate(string expression)
{
    vector<ExpressionElement> stdExpression;
    StdTranslation(expression, stdExpression);
    return StdCalculate(stdExpression);
}
void Calculator::PreTranslation(string &expression)
{
    //the last type, OPTR or REAL
    ELETYPE type = OPTR;
    for (int i = 0; i < expression.size(); i++)
    {
        if (find(optrLib.begin(), optrLib.end(), expression[i])
                != optrLib.end())
        {
            if (type == OPTR &&
                    (expression[i] == '-' || expression[i] == '+'))
            {
                string sub;
                if (expression[i] == '+')
                {
                    sub = "(0+1)*";
                }
                else
                {
                    sub = "(0-1)*";
                }
                expression = expression.substr(0, i) + sub
                        + expression.substr(i+1, expression.size()-(i+1));
                i += sub.size() - 1;
            }
            else
            {
                if (expression[i] != ')')
                {
                    type = OPTR;
                }
                else
                {
                    type = REAL;
                }
            }
        }
        else
        {
            type = REAL;
        }
    }
}
int Calculator::StdTranslation(string expression, vector<ExpressionElement>& stdExpression)
{
    stdExpression.clear();
    //check the expression here
    //not recognise '-' '+' as one operand operator
    //translate -x to (0-1)*x { +x to (0+1)*x }
    PreTranslation(expression);
    expression = "(" + expression + ")";
    for (int i = 0; i < expression.size(); i++)
    {
        ExpressionElement ele;
        if (find(optrLib.begin(), optrLib.end(), expression[i])
                != optrLib.end())
        {
            Operator optr = *find(optrLib.begin(), optrLib.end(), expression[i]);
            ele.type = OPTR;
            ele.optr = optr;
        }
        else
        {
            int nLen = 0;
            while (i + nLen < expression.size() &&
                   find(optrLib.begin(), optrLib.end(), expression[i + nLen])
                   == optrLib.end())
            {
                nLen++;
            }
            string num = expression.substr(i, nLen), temp;
            double d = 0;
            stringstream buf;
            buf << num;
            buf >> d;
            buf << d;
            temp = buf.str();

            if (num == "0")
            {
                ele.type = REAL;
                ele.real = 0;
            }
            else if (0 != d && num == temp)
            {
                ele.type = REAL;
                ele.real = d;
            }
            else if (find(matrixLib.begin(), matrixLib.end(), num) != matrixLib.end())
            {
                ele.type = MATRIX;
                ele.matrix = find(matrixLib.begin(), matrixLib.end(), num)->value;
            }
            else
            {
                stdExpression.clear();
                errorLen = nLen;
                errorMsg = "无法识别的标识";
                return i - 1;   //find error
            }
            i = i + nLen - 1;
        }
        stdExpression.push_back(ele);
    }
    return -1;
}
ExpressionElement Calculator::StdCalculate(vector<ExpressionElement> stdExpression)
{
    stack<Operator> stOptr;
    stack<ExpressionElement> stOpnd;

    int pos = 0;
    stOptr.push(stdExpression[pos++].optr);
    ExpressionElement ele = stdExpression[pos++];
    while ( !stOptr.empty())
    {
        if (ele.type != OPTR)
        {
            stOpnd.push(ele);
            ele = stdExpression[pos++];
        }
        else
        {
            if (stOptr.top() == ele.optr)
            {
                stOptr.pop();
                if (stOptr.empty())
                {
                    break;
                }
                ele = stdExpression[pos++];
            }
            else if (stOptr.top() >= ele.optr)
            {
                ExpressionElement left, right, result;
                right = stOpnd.top();
                stOpnd.pop();
                left = stOpnd.top();
                stOpnd.pop();
                Operator optr = stOptr.top();
                stOptr.pop();
                result = Operate(left, optr, right);
                stOpnd.push(result);
            }
            else
            {
                stOptr.push(ele.optr);
                ele = stdExpression[pos++];
            }
        }
    }
    return stOpnd.top();
}
int Calculator::Check(string expression)
{
    //return the pos of error or return -1 when no error
    //error message save in member variable
    int pos;
    if (expression.empty())
    {
        errorLen = 0;
        errorMsg = "未输入表达式";
        return 0;
    }
    pos = CheckChar(expression);
    if (pos != -1)
    {
        return pos;
    }
    pos = CheckBracket(expression);
    if (pos != -1)
    {
        return pos;
    }
    vector<ExpressionElement> stdExpression;
    pos =  StdTranslation(expression, stdExpression);
    if (pos != -1)
    {
        return pos;
    }
    pos = CheckCalculate(stdExpression); //return the position of operator
    if (pos != -1)
    {
        return FindErrorPos(expression, pos);
    }
    return -1;
}
int Calculator::CheckBracket(string expression)
{
    int left = 0;
    for (int i = 0; i < expression.size(); i++)
    {
        if (expression[i] == '(')
        {
            left++;
        }
        else if (expression[i] == ')')
        {
            left--;
        }
        if (left < 0)
        {
            errorLen = 1;
            errorMsg = "缺少左括号";
            return i;
        }
    }
    if (left > 0)
    {
        errorLen = 1;
        errorMsg = "缺少右括号";
        return expression.size() - 1;
    }
    return -1;
}
int Calculator::CheckChar(string expression)
{
    ELETYPE type = OPTR;
    for (int i = 0; i < expression.size(); i++)
    {
        if (find(optrLib.begin(), optrLib.end(), expression[i])
                != optrLib.end())
        {
            if (type == OPTR && expression[i] != '-'
                    && expression[i] != '+'
                    && expression[i] != '(')
            {
                errorLen = 1;
                errorMsg = "缺少操作数";
                return i;
            }
            else if (type == REAL && expression[i] == '(')
            {
                errorLen = 1;
                errorMsg = "缺少运算符";
                return i;
            }
            else if (expression[i] == ')')
            {
                type = REAL;
            }
            else
            {
                type = OPTR;
            }
        }
        else
        {
            type = REAL;
        }
    }
    if (type == OPTR)
    {
        errorLen = 0;
        errorMsg = "结尾缺少运算数";
        return expression.size();
    }
    return -1;
}
int Calculator::CheckCalculate(vector<ExpressionElement> stdExpression)
{
    //return the position of operator ignore the first '('
    int optrPos = -1;
    stack<Operator> stOptr;
    stack<ExpressionElement> stOpnd;

    int pos = 0;
    stOptr.push(stdExpression[pos++].optr);
    ExpressionElement ele = stdExpression[pos++];
    while ( !stOptr.empty())
    {
        if (ele.type != OPTR)
        {
            stOpnd.push(ele);
            ele = stdExpression[pos++];
        }
        else
        {
            if (stOptr.top() == ele.optr)
            {
                stOptr.pop();
                // /////////
                optrPos++;
                if (stOptr.empty())
                {
                    break;
                }
                ele = stdExpression[pos++];
            }
            else if (stOptr.top() >= ele.optr)
            {
                ExpressionElement left, right, result;
                right = stOpnd.top();
                stOpnd.pop();
                left = stOpnd.top();
                stOpnd.pop();
                Operator optr = stOptr.top();
                stOptr.pop();
                switch (optr.charName) {
                case '+': case '-':
                    if (left.type != right.type)
                    {
                        errorLen = 1;
                        errorMsg = "矩阵无法和实数相加减";
                        return optrPos;
                    }
                    else if (left.type == MATRIX && right.type == MATRIX)
                    {
                        if (left.matrix.size() != right.matrix.size()
                                || left.matrix[0].size() != right.matrix[0].size())
                        {
                            errorLen = 1;
                            errorMsg = "不匹配矩阵无法加减";
                            return optrPos;
                        }
                    }
                    break;
                case '*':
                    if (left.type == MATRIX && right.type == MATRIX)
                    {
                        if (left.matrix[0].size() != right.matrix.size())
                        {
                            errorLen = 1;
                            errorMsg = "不匹配矩阵无法相乘";
                            return optrPos;
                        }
                    }
                    break;
                case '|':
                    if (left.type != MATRIX || right.type != REAL || right.real != 1)
                    {
                        errorLen = 1;
                        errorMsg = "实数没有求逆运算";
                        return optrPos;
                    }
                    else if (left.matrix.size() != left.matrix[0].size()
                             || 0.0 == Determinant(left.matrix))
                    {
                        errorLen = 1;
                        errorMsg = "矩阵不可逆";
                        return optrPos;
                    }
                    break;
                case '/':
                    if (right.type == REAL && right.real == 0)
                    {
                        errorLen = 1;
                        errorMsg = "0不能为被除数";
                        return optrPos;
                    }
                    else if (left.type == REAL && right.type == MATRIX)
                    {
                        errorLen = 1;
                        errorMsg = "实数无法除以矩阵";
                        return optrPos;
                    }
                    else if (left.type == MATRIX && right.type == MATRIX)
                    {
                        if (left.matrix[0].size() != right.matrix.size()
                                || right.matrix.size() != right.matrix[0].size())
                        {
                            errorLen = 1;
                            errorMsg = "不匹配矩阵无法相除";
                            return optrPos;
                        }
                        if (0.0 == Determinant(right.matrix))
                        {
                            errorLen = 1;
                            errorMsg = "被除矩阵不可逆";
                            return optrPos;
                        }
                    }
                    break;
                default:
                    break;
                }
                result = Operate(left, optr, right);
                stOpnd.push(result);
            }
            else
            {
                stOptr.push(ele.optr);
                // /////////////
                optrPos++;
                ele = stdExpression[pos++];
            }
        }
    }
    return -1;
}
void Calculator::GetError(string &errorMsg, int &errorLen)
{
    errorLen = this->errorLen;
    errorMsg = this->errorMsg;
}
bool Calculator::AddMatrix(MatrixVar var)
{
    if (find(matrixLib.begin(), matrixLib.end(), var.varName)
            != matrixLib.end())
    {
        errorMsg = var.varName + ":同名矩阵变量已存在";
        return false;
    }
    matrixLib.push_back(var);
    return true;
}

bool Calculator::DeleteMatrix(string varName)
{
    if (find(matrixLib.begin(), matrixLib.end(), varName)
            == matrixLib.end())
    {
        errorMsg = "未找到矩阵变量:" + varName;
        return false;
    }
    matrixLib.erase(find(matrixLib.begin(), matrixLib.end(), varName));
    return true;
}

ExpressionElement Calculator::Operate(ExpressionElement left, Operator optr, ExpressionElement right)
{
    ExpressionElement result;
    if (left.type == REAL && right.type == REAL)
    {
        result.type = REAL;
        switch (optr.charName) {
        case '+':
            result.real = left.real + right.real;
            break;
        case '-':
            result.real = left.real - right.real;
            break;
        case '*':
            result.real = left.real * right.real;
            break;
        case '/':
            result.real = left.real / right.real;
            break;
        default:
            break;
        }
    }
    else if (left.type == REAL && right.type == MATRIX)
    {
        result.type = MATRIX;
        for (int r = 0; r < right.matrix.size(); r++)
        {
            Array arr;
            for (int c = 0; c < right.matrix[r].size(); c++)
            {
                arr.push_back(left.real * right.matrix[r][c]);
            }
            result.matrix.push_back(arr);
        }
    }
    else if (left.type == MATRIX && right.type == REAL)
    {
        result.type = MATRIX;
        if (optr.charName == '|')
        {
            result.matrix = ReverseMatrix(left.matrix);
        }
        else
        {
            for (int r = 0; r < left.matrix.size(); r++)
            {
                Array arr;
                for (int c = 0; c < left.matrix[r].size(); c++)
                {
                    if (optr.charName == '*')
                    {
                        arr.push_back(left.matrix[r][c] * right.real);
                    }
                    else
                    {
                        arr.push_back(left.matrix[r][c] / right.real);
                    }
                }
                result.matrix.push_back(arr);
            }
        }
    }
    else if (left.type == MATRIX && right.type == MATRIX)
    {
        result.type = MATRIX;
        switch (optr.charName) {
        case '+': case '-':
            for (int r = 0; r < left.matrix.size(); r++)
            {
                Array arr;
                for (int c = 0; c < left.matrix[r].size(); c++)
                {
                    if (optr.charName == '+')
                    {
                        arr.push_back(left.matrix[r][c] + right.matrix[r][c]);
                    }
                    else
                    {
                        arr.push_back(left.matrix[r][c] - right.matrix[r][c]);
                    }
                }
                result.matrix.push_back(arr);
            }
            break;
        case '*':
            for (int r = 0; r < left.matrix.size(); r++)
            {
                Array arr;
                for (int c = 0; c < right.matrix[0].size(); c++)
                {
                    double d = 0.0;
                    for (int i = 0; i < right.matrix.size(); i++)
                    {
                        d += left.matrix[r][i] * right.matrix[i][c];
                    }
                    arr.push_back(d);
                }
                result.matrix.push_back(arr);
            }
            break;
        case '/':
        {
            Matrix reversal = ReverseMatrix(right.matrix);
            for (int r = 0; r < left.matrix.size(); r++)
            {
                Array arr;
                for (int c = 0; c < right.matrix[0].size(); c++)
                {
                    double d = 0.0;
                    for (int i = 0; i < reversal.size(); i++)
                    {
                        d += left.matrix[r][i] * reversal[i][c];
                    }
                    arr.push_back(d);
                }
                result.matrix.push_back(arr);
            }
        }
            break;
        default:
            break;
        }
    }
    return result;
}
double Calculator::Determinant(Matrix matrix)
{
    if (matrix.size() != matrix[0].size())
    {
        return 0;
    }
    if (matrix.size() == 1)
    {
        return matrix[0][0];
    }
    if (matrix.size() == 2)
    {
        return (matrix[0][0] * matrix[1][1]
                - matrix[0][1] * matrix[1][0]);
    }
    double result = 0;
    for (int c = 0; c < matrix.size(); c++)
    {
           Matrix low = matrix;
           low.erase(low.begin());
           for (int r = 0; r < low.size(); r++)
           {
               low[r].erase(low[r].begin() + c);
           }
           if (c % 2 == 0)
           {
               result += matrix[0][c] * Determinant(low);
           }
           else
           {
               result -= matrix[0][c] * Determinant(low);
           }
    }
    return result;
}
Matrix Calculator::ReverseMatrix(Matrix matrix)
{
    Matrix reversal;
    for (int r = 0; r < matrix.size(); r++)
    {
        Array arr;
        for (int c = 0; c < matrix[r].size(); c++)
        {
            Matrix low = matrix;
            low.erase(low.begin() + r);
            for (int r2 = 0; r2 < low.size(); r2++)
            {
                low[r2].erase(low[r2].begin() + c);
            }
            if ((r + c) % 2 == 0)
            {
                arr.push_back(Determinant(low));
            }
            else
            {
                arr.push_back(- Determinant(low));
            }
        }
        reversal.push_back(arr);
    }
    //inversion
    for (int r = 0; r < reversal.size(); r++)
    {
        for (int c = r + 1; c < reversal[r].size(); c++)
        {
            double temp = reversal[r][c];
            reversal[r][c] = reversal[c][r];
            reversal[c][r] = temp;
        }
    }
    //devision
    double determinant = Determinant(matrix);
    for (int r = 0; r < reversal.size(); r++)
    {
        for (int c = 0; c < reversal[r].size(); c++)
        {
            reversal[r][c] /= determinant;
        }
    }
    return reversal;
}
int Calculator::FindErrorPos(string expression, int elePos)
{
    //-x(+x) has been translate to (0-1)*{(0+1)*}x
    string sub1 = "(0-1)*", sub2 = "(0+1)*";
    int pos = -1;
    for (int i = 0; i < expression.size(); i++)
    {
        if (expression.substr(i, sub1.size()) == sub1
                || expression.substr(i, sub2.size()) == sub2)
        {
            pos--;
        }
        if (find(optrLib.begin(), optrLib.end(), expression[i])
                != optrLib.end())
        {
            pos++;
        }
        if (pos == elePos)
        {
            return i;
        }
    }
}
