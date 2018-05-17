#ifndef DEF_H
#define DEF_H

#define REGEXP_RC               "[1-9][0-9]{0,9}"
#define REGEXP_VARNAME          "[a-z,A-Z]{1,64}[0-9]{0,64}"
#define REGEXP_REAL             "-?[0-9]{1,10}[.]{0,1}[0-9]{0,3}"
#define DBL_PEC                 6
#define DBL_MIN                 1e-7

#define ACTION_MSG                     unsigned char
#define ADD_MATRIX              0
#define GET_MATRIX              1
#define RMV_MATRIX              2
#define FND_MATRIX              3
#define CALCULATE               4

#define DLG_MODLE               unsigned char
#define ADD_MATRIX              0
#define EDIT_MATRIX             1
#define NEW_MATRIX              2
#define ADD_RESULT              3
#define ADD_ZERO                4
#define ADD_UNIT                5
#define ADD_ONE                 6

#define DATA_HEAD               "[matrix]"
#define MATRIX_RESULT           "(矩阵)"

#endif // DEF_H
