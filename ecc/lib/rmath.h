/* Rational and integer basic mathematical function implementations */

#include "rational.h"

#ifndef RMATH_H
#define RMATH_H

long int powli(long int base, int power);
Q *Rpow(Q *base, int power);
Q *overflow(Q *A);

#endif
