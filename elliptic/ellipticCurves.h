/* elliptic point struct header file */

#ifndef ELLIPTICCURVES_H
#define ELLIPTICCURVES_H


typedef struct {
    long int a;
    long int b;
} Curve;


int Cequal(Curve *A, Curve *B){
    return ((A->a == B->a) && (A->b == B->b));
}

#endif


