/* Rational number header file
 *  I should implement a conversion function set
 *      i.e. int to Q, double to Q, long int to Q, etc.
 * 
 */


#include <stdlib.h>
#include <stdio.h>

#ifndef RATIONAL_H
#define RATIONAL_H

#define IS_ZERO(m,n) (!m || !n)

// Currently 0/0, 0/x, or x/0 is assumed to be zero and treated as such i.e. }-> 0/0 sgn=0
// 0 = + ; 1 = -
// No check for division by zero
typedef struct Q_ {
    unsigned int sgn : 1;
    unsigned long int m;
    unsigned long int n;
} Q;

// Recursive Greatest Common Denominator
unsigned long int gcdr(unsigned long int a, unsigned long int b);

// In place reduction of Rational to gcd. I.E. Q(sgn, m, n) -> Q(sgn, m/gcd(m,n), n/gcd(m,n)) 
void reduce(Q *A);

// Create the Rational number
Q* createQ(int sign, unsigned long int a, unsigned long int b);

// Create a deep copy  of $from into $to
void copy(Q *from, Q *to);

// Check if the Rationals are equal 
int equals(Q *A, Q *B);

// free the rational and its data
void destroyQ(Q *rational);

// Add two rationals together
Q* add(Q *A, Q *B);

// Add one rational $from into another one $to
void addTo(Q *from, Q *to);

// Add an integer to a rational m/n + s = (m + ns)/n
Q* addInt(Q *A, long int num);

// Add an integer to a rational and store in same rational
void addIntTo(Q *to, long int num);

// Subract two rationals $A from $B A-B
Q* subtract(Q *A, Q *B);

// Subract two rationals $from - $to -> $to
void subtractTo(Q *from, Q *to);

// Subtract an integer from a rational
Q* subtractInt(Q *A, long int num);

// Subtract an integer from a rational and store in same rational
void subtractIntTo(Q *to, long int num);

// Multiply Two rationals together
Q* multiply(Q *A, Q *B);

// Multiply Two rationals in place and store in to $from * $to -> $to
void multiplyTo(Q *from, Q*to);

// Sticking with same nomenclature for "rationality" har har har
// Scale a rational by an int and return a rational
Q* multiplyInt(Q *A, long int num);

// Scale a rational by an int and store in same rational
void multiplyIntTo(Q *to, long int num);

// Divide a rational by another rational m/n / b/a = ma/nb
Q* divide(Q *A, Q *B);

// Divide a rational by another rational and store in to $from / $to -> $to
void divideTo(Q *from, Q *to);

// Divide a rational by an integer m/n / s = m/ns
Q* divideInt(Q *A, long int num);

// Divide a rational by an integer and store in rational $to / num -> $to
void divideIntTo(Q *to, long int num);



/*
 *
 *  Begin Functions
 *
 *
 */


unsigned long int gcdr(unsigned long int a, unsigned long int b){
    return a == 0 ? b : gcdr(b%a, a);
}

// Reduce in place. If gcd is 1 nothing happens. Else it reduces by gcdr
void reduce(Q *A){
    // Zero Check
    if (!IS_ZERO(A->m, A->n)){
        //Because it is gcd, the max val is def bounded by max(m,n) 
        unsigned long int gcd = gcdr(A->m, A->n);
        if (gcd != 1){
            A->sgn = A->sgn;
            A->m = A->m / gcd;
            A->n = A->n / gcd;
        }
    } else {
        A->sgn = 0;
        A->m = 0;
        A->n = 0;
    }
}


Q* createQ(int sign, unsigned long int a, unsigned long int b){
    Q* rat = (Q*) malloc(sizeof(Q));
    if (IS_ZERO(a,b)){
        rat->sgn = 0;
        rat->m = 0;
        rat->n = 0;
    } else { 
        rat->sgn = sign < 0 ? 1 : 0;
        rat->m = a;
        rat->n = b;
    }
    reduce(rat);
    return rat;
}

void copy(Q *from, Q *to){
    to->sgn = from->sgn;
    to->n = from->n;
    to->m = from->m;
}


// Returns 0 if not equal, 1 if equal 
int equals(Q *A, Q *B){
    return ( A->m == B->m ) && ( A->n == B->n) && (B->sgn == A->sgn);
}

void destroyQ(Q *rational){
    free(rational);
}

//Add Rationals
Q* add(Q *A, Q *B){

    Q *result = createQ(0, 0, 0);

    if (IS_ZERO(A->m, A->n)){
        result->sgn = B->sgn;
        result->m = B->m;
        result->n = B->n;
    } else if (IS_ZERO(B->m, B->n)){
        result->sgn = A->sgn;
        result->m = A->m;
        result->n = A->n;
    } else {

        // Overflow integer check
        unsigned long int x = A->m * B->n;
        if (x < 0 || x / B->n != A->m) { destroyQ(result); return NULL; }
        unsigned long int y = A->n * B->m;
        if (y < 0 || y / A->n != B->m) { destroyQ(result); return NULL; }
        unsigned long int denom = A->n * B->n;
        if (denom < 0 || denom / A->n != B->n) { destroyQ(result); return NULL; }
        result->n = denom;

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

    // reduce has a "set to proper zero" function in it
    reduce(result);
    return result;
}

void addTo(Q *from, Q *to){

    if (IS_ZERO(from->m, from->n)){;
    } else if (IS_ZERO(to->m, to->n)){
        to->sgn = from->sgn;
        to->m = from->m;
        to->n = from->n;
    } else {

        // Overflow integer check
        unsigned long int x = from->m * to->n;
        if (x < 0 || x / to->n != from->m) { to = NULL; return;}
        unsigned long int y = from->n * to->m;
        if (y < 0 || y / from->n != to->m) { to = NULL; return;}
        unsigned long int denom = from->n * to->n;
        if (denom < 0 || denom / from->n != to->n) { to = NULL; return;}

        // Set denominator
        to->n = denom;

        // Actual addition
        if (from->sgn == to->sgn){
            to->m = (x + y);
            to->sgn = from->sgn;
        } else {
            // If from is negative and to is positive
            if (from->sgn && 1){
                // If from is greater than or equal to then to is -
                if (x >= y){
                    to->m = (x - y);
                    to->sgn = 1;
                // Else if from is less than to then to is +
                } else {
                    to->m = (y - x);
                    to->sgn = 0;
                }

            // If from is positive and to is negative
            } else {
                // If from is greater than or equal to then to is +
                if (x >= y){
                    to->m = (x - y);
                    to->sgn = 0;
                // Else if to is less than from then to is -
                } else {
                    to->m = (y - x);
                    to->sgn = 1;
                }
            }
        }
    }
    // reduce has a "set to proper zero" function in it
    reduce(to);
}

// This function is a bit too long-form but it is clearer than shortening
Q* addInt(Q *A, long int num){
    Q *result = createQ(0, 0, 0);
    // 0 then add nothing
    if (!num){
        copy(A, result);
    } else if ( num < 0){
        result->sgn = 1;
        result->m = (unsigned long int) num * -1;
        result->n = 1;
        addTo(A, result);
    } else {
        result->sgn = 0;
        result->m = (unsigned long int) num;
        result->n = 1;
        addTo(A, result);
    }
    return result;
}

void addIntTo(Q *to, long int num){
    Q *result = createQ(0, 0, 0);
    // 0 then add nothing
    if (!num){;
    } else if (num < 0) {
        result->sgn = 1;
        result->m = (unsigned long int) (num * -1);
        result->n = 1;
    } else {
        result->sgn = 0;
        result->m = (unsigned long int) num;
        result->n = 1;
    }        
    addTo(result, to);
    destroyQ(result);
}

Q* subtract(Q *A, Q *B){
    Q *tmp = createQ(0, 0, 0);
    copy(B, tmp);
    tmp->sgn = tmp->sgn ^ 1;
    addTo(A, tmp);
    return tmp;
}

void subtractTo(Q *from, Q *to){
    to->sgn = to->sgn ^ 1;
    addTo(from, to);
}

Q* subtractInt(Q *A, long int num){
    num = -1 * num;
    return addInt(A, num);
}

void subtractIntTo(Q *to, long int num){
    num = -1 * num;
    addIntTo(to, num);
}

Q* multiply(Q *A, Q *B){
    Q *result = createQ(0, 0, 0);

    // 0 check
    if (IS_ZERO(A->m, A->n) || IS_ZERO(B->m, B->n));
    else {
        // integer overflow
        unsigned long int x = A->m * B->m;
        if (x < 0 || x / A->m != B->m) { destroyQ(result); return NULL; }
        unsigned long int y = A->n * B->n;
        if (y < 0 || y / A->n != B->n) { destroyQ(result); return NULL; }

        result->m = x;
        result->n = y;
        result->sgn = (A->sgn ^ B->sgn);
    }

    // reduce has a zero check so yea
    reduce(result);
    return result;
}

void multiplyTo(Q *from, Q *to){
    
    // 0 check
    if (IS_ZERO(to->m, to->n) || IS_ZERO(from->m, from->n));
    else {
        // integer overflow
        unsigned long int x = from->m * to->m;
        if (x < 0 || x / from->m != to->m) { to = NULL; return; }
        unsigned long int y = from->n * to->n;
        if (y < 0 || y / from->n != to->n) { to = NULL; return; }

        to->m = x;
        to->n = y;
        to->sgn = (to->sgn ^ from->sgn);
    }
    
    // reduce has a zero check -- so there is no need to set to zero prev
    reduce(to);
}

Q* multiplyInt(Q *A, long int num){
    Q *result = createQ(0, 0, 0);
    if (!num);
    else if (num < 0) {
        result->sgn = 1;
        result->m = (unsigned long int) (num * -1);
        result->n = 1;
    } else {
        result->sgn = 0;
        result->m = (unsigned long int) num;
        result->n = 1;
    }
    multiplyTo(A, result);
    return result;
}

void multiplyIntTo(Q *to, long int num){
    Q *result = createQ(0, 0, 0);

    // 0 then set 0 
    if (!num) { ;
    } else if (num < 0) {
        result->sgn = 1;
        result->m = (unsigned long int) (num * -1);
        result->n = 1;
    } else {
        result->sgn = 0;
        result->m = (unsigned long int) num;
        result->n = 1;
    }   
    multiplyTo(result, to);
    destroyQ(result);
}

Q* divide(Q *A, Q *B){
    Q *result = createQ(0, 0, 0);

    // x/0 is a divide by zero error -- 0/x is 0 bc 0/x = 0 * 1/x = 0
    if (IS_ZERO(B->m, B->n)) { result = NULL;
    } else if (IS_ZERO(A->m, A->n)) { copy(A, result);
    } else {
        // Explicit instead of implicit
        result->sgn = B->sgn;
        result->m = B->n;
        result->n = B->m;
        multiplyTo(A, result);
    }
    return result;
}

void divideTo(Q *from, Q *to){

    if (IS_ZERO(to->m, to->n)) { to = NULL;
    } else if (IS_ZERO(from->n, from->n)) { copy(from, to);
    } else {
        to->m = to->n;
        to->n = to->m;
        multiplyTo(from, to);
    }
}

Q* divideInt(Q *A, long int num){
    Q *result = createQ(0,0,0);
    
    if (!num) { result = NULL;
    } else if (IS_ZERO(A->m, A->n)){;
    } else {
        if (num < 0) {
            result->sgn = 1;
            result->m = (unsigned long int) (num * -1);
        } else {
            result->m = (unsigned long int) num;
        }
        result->n = 1;
        } 
    divideTo(A, result);
    return result;
}

void divideIntTo(Q *to, long int num){
    if (!num) { to = NULL;
    } else {
        Q *result = createQ(0, 0, 0);
        if (IS_ZERO(to->m, to->n)){ ;
        } else if (num < 0) {
            result->sgn = 1;
            result->m = (unsigned long int) (num * -1);
            result->n = 1;
        } else {
            result->sgn = 0;
            result->m = (unsigned long int) num;
            result->n = 1;
        }
        divideTo(result, to);
        destroyQ(result);
    }
}

#endif 
