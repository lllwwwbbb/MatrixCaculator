#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>
using namespace std;

//Matrix
typedef vector<double> Array;
typedef vector<Array> Matrix;

//MatrixVariable
struct MatrixVar
{
    string varName;
    Matrix value;

    bool operator ==(string right)
    {
        return this->varName == right;
    }

    bool operator ==(MatrixVar& right)
    {
        return this->varName == right.varName;
    }
    bool operator <(MatrixVar& right)
    {
        return this->varName < right.varName;
    }
};

#endif // MATRIX_H
