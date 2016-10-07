/* Rational and integer basic mathematical function implementations */

#include "rational.h"

#ifndef RMATH_H
#define RMATH_H


long int pow(long int base, int power){
    if (power < 0){
        return pow( 1 / base, -power);
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



#endif
