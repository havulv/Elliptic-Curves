/* Rational and integer basic mathematical function implementations */


#ifndef RMATH_H
#define RMATH_H

#include "rational.h"

long int powli(long int base, int power);
Q *Rpow(Q *base, int power);
Q *overflow(Q *A);

#endif
