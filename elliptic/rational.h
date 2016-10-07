/*Rational number header file */

#ifndef RATIONAL_H
#define RATIONAL_H

typedef struct {
    unsigned int sgn : 1;
    unsigned long int m;
    unsigned long int n;
} Rational;

// Doesn't check for buffer overflow which is a huge mistake
int Requal(Rational *A, Rational *B){
    return (((B->n * A->m) == (A->n * B->m)) && (B->sgn == A->sgn));
}

//Subtract Rationals
Rational *Rs(Rational *A, Rational *B){
    return Ra(a, b);
}

/* There is something to be said for storing values in 
 * "transitionary" memory and then doing gcd checks on them
 * but, in my opinion, this takes up too much memory. */
//Add Rationals
Rational *Ra(Rational *A, Rational *B){
    Rational *result = (Rational*) malloc(sizeof(Rational));

    //Should do overflow check here

    result->n = A->n * B->n;
    if (A->sgn == B->sgn){
        result->m = ((A->m * B->n) + (A->n * B->m));
        result->sgn = A->sgn;
    } else {
        if (A->sgn && 1){
            if ((A->m * B->n) >= (B->m * A->n)){
                result->m = ((A->m * B->n) - (B->m * A->n));
                result->sgn = 0;
            } else {
                result->m = ((B->m * A->n) - (A->m * B->n));
                result->sgn = 1;
            }
        } else {
            if ((A->m * B->n) >= (B->m * A->n)){
                result->m - ((A->m * B->n) - (B->m * A->n));
                result->sgn = 1;
            } else {
                result->m = ((B->m * A->n) - (A->m * B->n));
                result->sgn = 0;
            }
        }
    }

    //gcd checks should be performed here

    return result;
}

//Multiply Rationals
Rational *Rm(Rational *A, Rational *B){
    Rational *result = (Rational*) malloc(sizeof(Rational));

    //Should do buffer overflow check here
    //Also should do gcd checks here

    result->m = (A->m * B->m);
    result->n = (A->n * B->n);
    result->sgn = (A->sgn ^ B->sgn);
    return result;
}

//Divide Rationals
Rational *Rd(Rational *A, Rational*B){
    Rational *result = (Rational*) malloc(sizeof(Rational));

    //Should do buffer overflow check here
    //Also should do gcd checks here
    
    result->m = (A->m * B->n);
    result->n = (A->n * B->m);
    result->sgn = (A->sgn ^ B->sgn);
    return result;
}

#endif 
