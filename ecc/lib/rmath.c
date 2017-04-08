
#include "rational.c"
#include "rmath.h"


long int powli(long int base, int power){
    if (power < 0){
        return powli( 1 / (double) base, -power);
    } else if (power == 0) {
        return (long int) 1;
    } else if (power == 1) {
        return base;
    } else if ((power % 2) == 0){
        return powli(base * base, power/2);
    } else {
        return powli(base * base, (power-1)/2);
    }
}

// Maybe reductions should be made, but this could be really expensive
Q *Rpow(Q *base, int power){
    if (power < 0){
        unsigned long int mid = base->m;
        base->m = base->n;
        base->n = mid;
        return Rpow(base, -power);
    } else if (power == 0){
        base->sgn = 0;
        base->m = 1;
        base->n = 1;
        return base;
    } else if (power == 1){
        return base;
    } else if ((power % 2) == 0){
        return Rpow( multiply(base, base), power/2);
    } else {
        return Rpow( multiply(base, multiply(base, base)), (power-1)/2);
    }
}

Q *overflow(Q *A){
    return NULL;
}

