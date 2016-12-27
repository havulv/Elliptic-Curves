/* elliptic point struct header file */

#ifndef ECURVES_H
#define ECURVES_H


typedef struct {
    long int a;
    long int b;
} Curve;


int Cequal(Curve *A, Curve *B){
    return ((A->a == B->a) && (A->b == B->b));
}

#endif


