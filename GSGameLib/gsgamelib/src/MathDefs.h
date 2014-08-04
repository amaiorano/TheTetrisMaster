#ifndef _MATH_DEFS_H_
#define _MATH_DEFS_H_

// Defines and macros for many useful mathematical symbols and equations

#define PI 3.1415926535897932384626433832795
#define DEG2RAD(x) (float)(x*PI/180.0f)
#define RAD2DEG(x) (float)(x*180.0f/PI)
#define ADJUST_ANGLE(theta) (theta<0? theta+=360.0f : (theta>360.0f? theta -=360.0f : (0)))

// This epsilon value has been carefully chosen for 'float' comparisons
// (see FLOAT_EQUALS below).
// 'float's are accurate to 6 decimals, so we allow two floats to be
// considered equal if their difference is less than 0.00001f.
#define EPSILON	0.00001f 
#define FLOAT_EQUALS(fVariable, fValue) ((fVariable <= (fValue+EPSILON)) && (fVariable >= (fValue-EPSILON)))

// Be careful with this macro, it will evaluate the input expression twice
// before multiplying their results.
// TODO: Code global function Squared() to avoid this problem.
#define SQUARED(a) (a*a)

#endif // _MATH_DEFS_H_
