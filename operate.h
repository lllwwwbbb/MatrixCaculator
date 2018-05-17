#ifndef OPERATE_H
#define OPERATE_H

#include "def.h"
#include "matrix.h"

//operator
struct Operator
{
    char charName;
    int priority;

    Operator(char name = '0', int pri = 0)
        :charName(name), priority(pri)
    {
    }
    bool operator ==(char ch)
    {
        return this->charName == ch;
    }
    bool operator >=(Operator& right)
    {
        return ( !(*this == right) &&
                 !(this->charName == '(') &&
                 this->priority <= right.priority);
    }
    bool operator ==(Operator& right)
    {
        return (this->charName == '(' &&
                right.charName == ')');
    }
};

//expression element
#define ELETYPE     unsigned char
#define REAL        0
#define OPTR        1
#define MATRIX      2
struct ExpressionElement
{
    ELETYPE type;
    double real;
    Matrix matrix;
    Operator optr;
};

#endif // OPERATE_H
