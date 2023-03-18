#ifndef _DOLPHIN_MATH_H
#define _DOLPHIN_MATH_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

#ifndef __MWERKS__
// Get clangd to shut up about __fabs being undefined.
#define __fabs(x)    (x)
#define __frsqrte(x) (x)
#endif

#define FABS(x) (float)__fabs(x)
// #define __frsqrtes opword

#define SQUARE(v) ((v) * (v))

#define TAU     6.2831855f
#define PI      3.1415927f
#define HALF_PI 1.5707964f

#define LONG_TAU 6.2831854820251465

int abs(int x); // might belong to another file, future "issue"

double asin(double);

double cos(double);
float cosf(float);
double sin(double);
double tan(double);
float tanf(float);
double atan(double);
double atan2(double, double);

extern int __float_epsilon[];
#define FLT_EPSILON (*(float *)__float_epsilon)

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif