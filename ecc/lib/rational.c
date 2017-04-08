
#include <stdlib.h>
#include <stdio.h>
#include "rational.h"

#define IS_ZERO(m,n) (!m || !n)

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

// Reverse a 32 bit float's significand in place
unsigned int reverse24(unsigned int x){
    // Literally, reverse in chunks as 32-bit and shift backwards
    x = ((x >> 1) & 0x55555555u) | ((x & 0x55555555u) << 1);
    x = ((x >> 2) & 0x33333333u) | ((x & 0x33333333u) << 2);
    x = ((x >> 4) & 0x0f0f0f0fu) | ((x & 0x0f0f0f0fu) << 4);
    x = ((x >> 8) & 0x00ff00ffu) | ((x & 0x00ff00ffu) << 8);
    x = ((x >> 16) & 0xffffu) | ((x & 0xffffu) << 16);
    return x >> 8; 
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

// IEEE 754
Q* dToQ(double num){
    Q *result = createQ(0,0,0);

    reduce(result);
    return result;
}

// IEEE 754
void dToQTo(double num, Q *to){

    reduce(to);
}

// IEEE 754
Q* fToQ(float num){
    Q *result = createQ(0,0,0);
    unsigned int exp = (unsigned int) num & 0x7f800000;
    unsigned int frac = (unsigned int) num & 0x007fffff;
    result->sgn = (unsigned int) num >> 31;

    // if the significand is not all zeros -> implicit bit
    if (frac) { 
        frac |= 0x00800000;
        // significand translates to 1 + 1/reverse(bits)
        frac = reverse24(frac) | 0x01000000;
    }
    
    // Exponent equals zero
    if (!exp) {

    } else if (!(exp ^ 0x7f800000)) { // Exponent = 0xff -> NaN
        result = NULL;
    } else { // All other cases
        0x00; 
    }
    result->m;
    result->n;

    reduce(result);
    return result;
}

// IEEE 754
void fToQTo(float num, Q *to){

    reduce(to);
}

Q* intToQ(int num){
    Q *result = createQ(0,0,0);
    if (num < 0){
        result->sgn = 1;
        result->m = (unsigned long int) (-1 * num);
        result->n = 1;
    } else {
        result->sgn = 0;
        result->m = (unsigned long int) num;
        result->n = !num ? 1 : 0;
    }
    return result;
}

void intToQTo(int num, Q *to){
    if (num < 0){
        to->sgn = 1;
        to->m = (unsigned long int) (-1 * num);
        to->n = 1;
    } else {
        to->sgn = 0;
        to->m = (unsigned long int) num;
        to->n = !num ? 0 : 1;
    }
}

Q* uintToQ(unsigned int num){
    Q *result = createQ(0, (unsigned long int) num, !num ? 0 : 1);
    return result;
}

void uintToQTo(unsigned int num, Q *to){
    to->sgn = 0;
    to->m = (unsigned long int) num;
    to->n = !num ? 0 : 1;
}

Q* lintToQ(long int num){
    Q *result = createQ(0,0,0);
    if (num < 0){
        result->sgn = 1;
        result->m = (unsigned long int) (-1 * num);
        result->n = 1;
    } else {
        result->sgn = 0;
        result->m = (unsigned long int) num;
        result->n = !num ? 1 : 0;
    }
    return result;
}

void lintToQTo(long int num, Q *to){
    if (num < 0){
        to->sgn = 1;
        to->m = (unsigned long int) (-1 * num);
        to->n = 1;
    } else {
        to->sgn = 0;
        to->m = (unsigned long int) num;
        to->n = !num ? 0 : 1;
    }
}

Q* ulintToQ(unsigned long int num){
    Q *result = createQ(0, num, !num ? 0 : 1);
    return result;
}

void ulintToQTo(unsigned long int num, Q *to){
    to->sgn = 0;
    to->m = num;
    to->n = !num ? 0 : 1;
}

float* QTof(Q *rational){
    float *result = malloc(sizeof(float));
    *result = (float) rational->m / (float) rational->n;
    if (rational->sgn) *result = *result * -1;
    return result;
}

void QTofTo(Q *rational, float *to){
    *to = (float) rational->m / (float) rational->n;
    if (rational->sgn) *to = *to * -1;
}

double* QTod(Q *rational){
    double *result = malloc(sizeof(double));
    *result = (double) rational->m / (double) rational->n;
    if (rational->sgn) *result = *result * -1;
    return result;
}

void QTodTo(Q *rational, double *to){
    *to = (double) rational->m / (double) rational->n;
    if (rational->sgn) *to = *to * -1;
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

void addTo(Q *p, Q *q, Q *to){

    if (IS_ZERO(p->m, p->n)){
        to->sgn = q->sgn;
        to->m = q->m;
        to->n = q->n;
    } else if (IS_ZERO(q->m, q->n)){
        to->sgn = p->sgn;
        to->m = p->m;
        to->n = p->n;
    } else {

        // Overflow integer check
        unsigned long int x = p->m * q->n;
        if (x < 0 || x / q->n != p->m) { to = NULL; return;}
        unsigned long int y = p->n * q->m;
        if (y < 0 || y / p->n != q->m) { to = NULL; return;}
        unsigned long int denom = p->n * q->n;
        if (denom < 0 || denom / p->n != q->n) { to = NULL; return;}

        // Set denominator
        to->n = denom;

        // Actual addition
        if (p->sgn == q->sgn){
            to->m = (x + y);
            to->sgn = p->sgn;
        } else {
            // If p is negative and q is positive
            if (p->sgn){
                // If p is greater than or equal to q to is -
                if (x >= y){
                    to->m = (x - y);
                    to->sgn = 1;
                // Else if p is less than to q to is +
                } else {
                    to->m = (y - x);
                    to->sgn = 0;
                }

            // If p is positive and q is negative
            } else {
                // If p is greater than or equal to q to is +
                if (x >= y){
                    to->m = (x - y);
                    to->sgn = 0;
                // Else if q is less than p then to is -
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
        addTo(A, result, result);
    } else {
        result->sgn = 0;
        result->m = (unsigned long int) num;
        result->n = 1;
        addTo(A, result, result);
    }
    return result;
}

void addIntTo(Q *from, long int num, Q *to){
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
    addTo(from, result, to);
    destroyQ(result);
}

Q* subtract(Q *A, Q *B){
    Q *tmp = createQ(0, 0, 0);
    copy(B, tmp);
    tmp->sgn = tmp->sgn ^ 1;
    addTo(A, tmp, tmp);
    return tmp;
}

void subtractTo(Q *p, Q *q, Q *to){
    q->sgn = q->sgn ^ 1;
    addTo(p, q, to);
}

Q* subtractInt(Q *A, long int num){
    num = -1 * num;
    return addInt(A, num);
}

void subtractIntTo(Q *from, long int num, Q *to){
    num = -1 * num;
    addIntTo(from, num, to);
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

void multiplyTo(Q *from1, Q *from2, Q *to){
    
    // 0 check
    if (IS_ZERO(from2->m, from2->n) || IS_ZERO(from1->m, from1->n));
    else {
        // integer overflow
        unsigned long int x = from1->m * from2->m;
        if (x < 0 || x / from1->m != from2->m) { to = NULL; return; }
        unsigned long int y = from1->n * from2->n;
        if (y < 0 || y / from1->n != from2->n) { to = NULL; return; }

        to->m = x;
        to->n = y;
        to->sgn = (from1->sgn ^ from2->sgn);
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
    multiplyTo(A, result, result);
    return result;
}

void multiplyIntTo(Q *from, long int num, Q *to){
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
    multiplyTo(result, from, to);
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
        multiplyTo(A, result, result);
    }
    return result;
}

void divideTo(Q *p, Q *q, Q *to){

    if (IS_ZERO(q->m, q->n)) { to = NULL;
    } else if (IS_ZERO(p->m, q->m)) { copy(p, to);
    } else {
        to->sgn = q->sgn;
        to->m = q->n;
        to->n = q->m;
        multiplyTo(p, to, to);
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

