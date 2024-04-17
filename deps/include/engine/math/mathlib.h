#ifndef __MATHLIB_H__
#define __MATHLIB_H__ 1

#include <iostream>
#include <cstring>
#include <cmath>

#ifndef M_1_PI
#define M_1_PI 0.318309886183790671538
#endif
#ifndef M_1_PIf
#define M_1_PIf 0.318309886183790671538f
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PIf
#define M_PIf 3.14159265358979323846f
#endif
#ifndef REC_180
#define REC_180 0.00555555555555555555f
#endif
#ifndef RAND_FLOAT_N
#define RAND_FLOAT_N (((float)(rand() & 0x7fffffff)) / (float)RAND_MAX)
#endif
#ifndef RAND_FLOAT
#define RAND_FLOAT(min, max) (min + (max - min) * (((float)(rand() & 0x7fffffff)) / (float)RAND_MAX))
#endif

#include "vector_2.h"
#include "vector_3.h"
#include "vector_4.h"

#include "matrix_2.h"
#include "matrix_3.h"
#include "matrix_4.h"

#include "math_utils.h"

#endif /* __MATHLIB_H__ */
