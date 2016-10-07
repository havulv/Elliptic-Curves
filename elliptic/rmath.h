/* Rational and integer basic mathematical function implementations */

#include "rational.h"

#ifndef RMATH_H
#define RMATH_H

unsigned long int gcdr(unsigned long int a, unsigned long int b){
    return a == 0 ? b : gcdr(b%a, a);
}

Rational *reduce(Rational *A){
    unsigned long int gcd = gcdr(A->m, A->n);
    if (gcd == 1){
        return A;
    } else {

    }
}

long int pow(long int base, int power){
    if (power < 0){
        return pow( 1 / (double) base, -power);
    } else if (power == 0) {
        return (long int) 1;
    } else if (power == 1) {
        return base;
    } else if ((power % 2) == 0){
        return pow(base * base, power/2);
    } else {
        return pow(base * base, (power-1)/2);
    }
}

Rational *Rpow(Rational *base, int power){
    if (power < 0){
        unsigned long int mid = base->m;
        base->m = base->n;
        base->n = mid;
        return Rpow(base, -power);
    } else if (power = 0){
        base->sgn = 0;
        base->m = 1;
        base->n = 1;
        return base;
    } else if ((power % 2) == 0){
        return Rpow( Rm(base, base), power/2);
    } else {
        return Rpow( Rm(base, base), (power-1)/2);
    }
}

#endif
