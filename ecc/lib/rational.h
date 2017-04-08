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
typedef struct Q {
    unsigned int sgn : 1;
    unsigned long int m;
    unsigned long int n;
} Q;

// Recursive Greatest Common Denominator
unsigned long int gcdr(unsigned long int a, unsigned long int b);

// In place reduction of Rational to gcd. I.E. Q(sgn, m, n) -> Q(sgn, m/gcd(m,n), n/gcd(m,n)) 
void reduce(Q *A);

// For float and double conversion, I need to reverse the significand's bits
unsigned int reverse24(unsigned int x);

// Create the Rational number
Q* createQ(int sign, unsigned long int a, unsigned long int b);

// Create a deep copy  of $from into $to
void copy(Q *from, Q *to);

// Check if the Rationals are equal 
int equals(Q *A, Q *B);

// free the rational and its data
void destroyQ(Q *rational);

// double to rational and return a rational
Q* dToQ(double num);

// double to rational and store in a rational
void dToQTo(double num, Q *to);

// float to rational and return a rational
Q* fToQ(float num);

// float to rational and store in a rational
void fToQTo(float num, Q *to);

// int to rational and return a rational
Q* intToQ(int num);

// int to rational and store in rational
void intToQTo(int num, Q *to);

// unsigned int to rational and return a rational
Q* uintToQ(unsigned int num);

// unsigned int to rational and store in rational
void uintToQTo(unsigned int num, Q *to);

// long int to rational and return a rational
Q* lintToQ(long int num);

// long int to rational and store in rational
void lintToQTo(long int num, Q *to);

// unsigned long int to rational and return a rational
Q* ulintToQ(unsigned long int num);

// unsigned long int to rational and store in rational
void ulintToQTo(unsigned long int num, Q *to);

// rational to float and return float pointer
float* QTof(Q *rational);

// rational to float and store in float
void QTofTo(Q *rational, float *to);

// rational to double and return double
double* QTod(Q *rational);

// rational to double and store in double
void QTodTo(Q *rational, double *to);

// Add two rationals together
Q* add(Q *A, Q *B);

// Add one rational $from into another one $to
void addTo(Q *p, Q *q, Q *to);

// Add an integer to a rational m/n + s = (m + ns)/n
Q* addInt(Q *A, long int num);

// Add an integer to a rational and store in same rational
void addIntTo(Q *from, long int num, Q *to);

// Subract two rationals $A from $B A-B
Q* subtract(Q *A, Q *B);

// Subract two rationals $from - $to -> $to
void subtractTo(Q *p, Q *q, Q *to);

// Subtract an integer from a rational
Q* subtractInt(Q *A, long int num);

// Subtract an integer from a rational and store in same rational
void subtractIntTo(Q *from, long int num, Q *to);

// Multiply Two rationals together
Q* multiply(Q *A, Q *B);

// Multiply Two rationals in place and store in to $from * $to -> $to
void multiplyTo(Q *from1, Q *from2, Q *to);

// Sticking with same nomenclature for "rationality" har har har
// Scale a rational by an int and return a rational
Q* multiplyInt(Q *A, long int num);

// Scale a rational by an int and store in same rational
void multiplyIntTo(Q *from, long int num, Q *to);

// Divide a rational by another rational m/n / b/a = ma/nb
Q* divide(Q *A, Q *B);

// Divide a rational by another rational and store in to $from / $to -> $to
void divideTo(Q *p, Q *q, Q *to);

// Divide a rational by an integer m/n / s = m/ns
Q* divideByInt(Q *A, long int num);

// Divide a rational by an integer and store in rational $to / num -> $to
void divideByIntTo(Q *from, long int num, Q *to);

void divideIntByTo(long int num, Q *from, Q *to);

#endif 
