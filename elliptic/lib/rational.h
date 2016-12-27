/*Rational number header file */

#include <stdlib.h>
#include <stdio.h>

#ifndef RATIONAL_H
#define RATIONAL_H

// Currently 0/0, 0/x, or x/0 is assumed to be zero and treated as such
// 0 = + ; 1 = -
// No check for division by zero
typedef struct {
    unsigned int sgn : 1;
    unsigned long int m;
    unsigned long int n;
} Rational;

unsigned long int gcdr(unsigned long int a, unsigned long int b){
    return a == 0 ? b : gcdr(b%a, a);
}

// Reduce in place. If gcd is 1 nothing happens. Else it reduces by gcdr
void reduce(Rational *A){
    //Because it is gcd it is gaurenteed to be an integer
    unsigned long int gcd = gcdr(A->m, A->n);
    if (gcd != 1){
        A->sgn = A->sgn;
        A->m = A->m / gcd;
        A->n = A->n / gcd;
    }
}

// Check if rational is zero or not
int Rzero(Rational *A){
    return ((A->m == 0) || (A->n == 0));
}

// assumes in irreducible form for wider range of available integers
int Requal(Rational *A, Rational *B){
    return ( A->m == B->m ) && ( A->n == B->n) && (B->sgn == A->sgn);
}

/* There is something to be said for storing values in 
 * "transitionary" memory and then doing gcd checks on them
 * but, in my opinion, this takes up too much memory. */
//Add Rationals
void *Ra(Rational *result, Rational *A, Rational *B){

    if (Rzero(A)){
        result = B; 
    } else if (Rzero(B)){
        result = A;
    } else {
        result->n = A->n * B->n;

        // Overflow integer check
        unsigned long int x = A->m * B->n;
        if (x < 0 || x / B->n != A->m) { result = NULL; return NULL; }
        unsigned long int y = A->n * B->m;
        if (y < 0 || y / A->n != B->m) { result = NULL; return NULL; }


        // Actual addition
        if (A->sgn == B->sgn){
            result->m = (x + y);
            result->sgn = A->sgn;
        } else {
            // If A is negative and B is positive
            if (A->sgn && 1){
                // If A is greater than or equal B then result is -
                if (x >= y){
                    result->m = (x - y);
                    result->sgn = 1;
                // Else if A is less than B then result is +
                } else {
                    result->m = (y - x);
                    result->sgn = 0;
                }

            // If A is positive and B is negative
            } else {
                // If A is greater than or equal B then result is +
                if (x >= y){
                    result->m = (x - y);
                    result->sgn = 0;
                // Else if B is less than B then result is -
                } else {
                    result->m = (y - x);
                    result->sgn = 1;
                }
            }
        }
    }

    // Check for 0 and if not then reduce
    if ((result->m == 0) || (result->n == 0)){
        result->m = 0;
        result->n = 0;
    } else {
        reduce(result);
    }
}

//Subtract Rationals
void *Rs(Rational *result, Rational *A, Rational *B){
    result = B;
    result->sgn = result->sgn ^ 1;
    Ra(result, A, result);
}

//Subtact integer from Rational
void *RsI(Rational *Scale, Rational *A, long int S){
    // Quick check for subtracting nothing
    if (S == 0) { Scale = A; }
    Scale->sgn = S < 0 ? 1 : 0;

    // Handle type conversion 
    if (S < 0) Scale->m  = (unsigned long int) (S * -1);
    else Scale->m = (unsigned long int) S;
    Scale->n = (unsigned long int) 1;
    Rs(Scale, A, Scale);
}

// Add integer (Could probably just call to subtract
void *RaI(Rational *Scale, Rational *A, long int S){
    // quick check for adding nothing
    if (S == 0) { Scale = A;}

    Scale->sgn = S < 0 ? 1 : 0;

    // Handle conversion from signed to unsigned
    if (S < 0) Scale->m  = (unsigned long int) (S * -1);
    else Scale->m = (unsigned long int) S;
    Scale->n = (unsigned long int) 1;
    Ra(Scale, A, Scale);
}

//Multiply Rationals
// Should I allocate and then return? It is out of local scope.
void *Rm(Rational *result, Rational *A, Rational *B){

    puts("multiplying");
    // 0 checks for calc skips
    if (Rzero(A) || Rzero(B)) {
        result->m = 0;
        result->n = 0;
        result->sgn = 0;
    } else {

        unsigned long int x = A->m * B->m;
        if (x < 0 || x / A->m != B->m) { result = NULL; return NULL; }
        unsigned long int y = A->n * B->n;
        if (y < 0 || y / A->n != B->n) { result = NULL; return NULL; }

        result->m = x;
        result->n = y;
        result->sgn = (A->sgn ^ B->sgn);

        // Gaurenteed not to be 0
        reduce(result);
    }
}

//Multiply an integer and a rational
void *RmI(Rational *Scale, Rational *A, long int S){
    puts("initializing");
    Scale->sgn = S < 0 ? 1 : 0;
    // Handle conversion from signed to unsigned
    puts("casting");
    if (S < 0) Scale->m = (unsigned long int) ( S * -1);
    else Scale->m = (unsigned long int) S;
    Scale->n = (unsigned long int) 1;
    Rm(Scale, A, Scale);
}

//Divide Rationals
void *Rd(Rational *result, Rational *A, Rational*B){
    // Division by 0 returns NULL
    if (Rzero(B)) { result = NULL; return NULL; }
    // 0 / x returns 0
    if (Rzero(A)) { result = A; return NULL; }

    // Integer overflow check
    unsigned long int x = A->m * B->n;
    if (x < 0 || x / A->m != B->n) { result = NULL; return NULL; }
    unsigned long int y = A->n * B->m;
    if (y < 0 || y / A->n != B->m) { result = NULL; return NULL; }
    
    result->m = x;
    result->n = y;
    result->sgn = (A->sgn ^ B->sgn);

    // Reduce to irreducible form
    reduce(result);
}

// Divide by an Integer
void *RdI(Rational *Scale, Rational *A, long int S) {
    Scale->sgn = S < 0 ? 1 : 0;
    // Handle conversion from signed to unsigned
    if (S < 0) Scale->n = (unsigned long int) (S * -1);
    else Scale->n = (unsigned long int) S;
    Scale->m = (unsigned long int) 1;
    Rd(Scale, A, Scale);
}

#endif 
