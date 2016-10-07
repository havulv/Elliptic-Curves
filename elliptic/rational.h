/*Rational number header file */

#ifndef RATIONAL_H
#define RATIONAL_H

typedef struct {
    unsigned int sgn : 1;
    unsigned long int m;
    unsigned long int n;
} Rational;

unsigned long int gcdr(unsigned long int a, unsigned long int b){
    return a == 0 ? b : gcdr(b%a, a);
}

// Doesn't check for buffer overflow which is a huge mistake
int Requal(Rational *a, Rational *b){
    return (((b->n * a->m) == (a->n * b->m)) && (b->sgn == a->sgn));
}

Rational *Rm(Rational *a, Rational *b){
    Rational *Ra(Rational *, Rational *);
    return Ra(a, b);
}

/* There is something to be said for storing values in 
 * "transitionary" memory and then doing gcd checks on them
 * but, in my opinion, this takes up too much memory. */
Rational *Ra(Rational *a, Rational *b){
    Rational *result = (Rational*) malloc(sizeof(Rational));

    //Should do overflow check here

    result->n = a->n * b->n;
    if (a->sgn == b->sgn){
        result->m = ((a->m * b->n) + (a->n * b->m));
        result->sgn = a->sgn;
    } else {
        if (a->sgn && 1){
            if ((a->m * b->n) >= (b->m * a->n)){
                result->m = ((a->m * b->n) - (b->m * a->n));
                result->sgn = 0;
            } else {
                result->m = ((b->m * a->n) - (a->m * b->n));
                result->sgn = 1;
            }
        } else {
            if ((a->m * b->n) >= (b->m * a->n)){
                result->m - ((a->m * b->n) - (b->m * a->n));
                result->sgn = 1;
            } else {
                result->m = ((b->m * a->n) - (a->m * b->n));
                result->sgn = 0;
            }
        }
    }

    //gcd checks should be performed here

    return result;
}

Rational *Rt(Rational *a, Rational *b){
    Rational *result = (Rational*) malloc(sizeof(Rational));

    //Should do buffer overflow check here
    //Also should do gcd checks here

    result->m = (a->m * b->m);
    result->n = (a->n * b->n);
    result->sgn = (a->sgn ^ b->sgn);
    return result;
}

#endif 
