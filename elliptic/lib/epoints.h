/* Elliptic Points header file */

#include "rational.h"
#include "ecurves.h"
#include "rmath.h"
#include <stdio.h>

#ifndef EPOINTS_H
#define EPOINTS_H

// Struct for a point on a rational elliptic curve
// x, y are rational points, E is the curve assciated with the point
// It is possible to have a point (x, y) that does not exist on the curve E
typedef struct {
    Rational *x;
    Rational *y;
    Curve *E;
} Point;

// Check for equality between points
int Pequal(Point *P, Point *Q){
    if (Cequal(P->E, Q->E)) return (Requal(P->x,Q->x) && Requal(P->y,Q->y));
    return 0;
}

// Check that the x, y coordinates are on the associated curve
// Curve is assumed to be of Weierstrass normal form
// y**2 == x**3 + x * a + b
int onCurve(Point *P){
    Rational *right;
    Rational *left;

    printf("x = %d/%d, E->a = %d\n", P->x->m, P->y->n, P->E->a);
    // Begin checks for failures
    RmI(right, P->x, P->E->a);
    if (right != NULL) {
        printf("right %d/%d\n", right->m, right->n);
        // The cast is necessary because RaI takes a
        // long int and this is assumed to be an int
        RaI(right, right, (long int) P->E->b);
        Rational *buff = Rpow(P->x, 3);
        if (right != NULL && buff != NULL) {
            printf("buff %d/%d , right %d/%d\n", buff->m, buff->n, right->m, right->n);
            Ra(right, buff, right);
            left = Rpow(P->y, 2);
            if (right != NULL && left != NULL) {
                printf("left %d/%d == right %d/%d\n", left->m, left->n, right->m, right->n);
                return Requal(left, right);
            }
        }
    }
    return 0;
}

void *slope(Rational *result, Point *P, Point *Q) {
    Rational *tmp1;
    Rs(tmp1, P->x, Q->x);
    Rational *tmp2;
    Rs(tmp2, P->y, Q->y);
    if ((tmp1 != NULL) && (tmp2 != NULL) && (Rzero(tmp2) ^ 1)) {
        Rd(result, tmp2, tmp1);
    } else return NULL;
}

// Returns the tangent slope at the point P
// (x**2 * 3 + a) / (2 * y)
void *PntTangent(Rational *result, Point *P){
    // Begin failure checks
    result = Rpow(P->x, 2);
    if (result != NULL) {
        RmI(result, result, 3);
        if (result != NULL) {
            RaI(result, result, P->E->a);
            Rational *buff;
            RmI(buff, P->y, 2);
            if (result != NULL && buff != NULL) {
                Rd(result, result, buff);
            }
        }
    }
    return NULL;
}

Point *dble(Point *P){
    
    if (Rzero(P->y) ^ 1){
        Point *R = malloc(sizeof(Point));
        // Allocation check
        if (R == NULL) return NULL; 
        R->E = P->E;
        Rational *lambda;
        PntTangent(lambda, P);
        if (lambda == NULL) return NULL;
        Rational *tmp1 = Rpow(lambda, 2);
        Rational *tmp2;
        RmI(tmp2, P->x, 2);
        if ((tmp1 != NULL) && (tmp2 != NULL)) {
            Rs(R->x, tmp1, tmp2);
            if ( R->x != NULL) {
                Rs(tmp1, P->x, R->x);
                if (tmp1 != NULL) {
                    Rm(tmp1, lambda, tmp1);
                    if (tmp1 != NULL) {
                        Rs(R->y, tmp1, P->y);
                    }
                }
            }
        }
    }
    return NULL;
}

Point *padd(Point *P, Point *Q){

    if (Cequal(P->E, Q->E) ^ 1){
        return NULL;
    } else if (Pequal(P, Q)) {
        return dble(P);
    }

    Rational *lambda = NULL;
    Point *R = malloc(sizeof(Point));
    // Allocation check
    if (R == NULL) return NULL;
    R->E = P->E;

    // Check if the x's are equal to each other
    if (Requal(P->x, Q->x)){
        if (Rzero(P->y) && (Rzero(Q->y) ^ 1)){
            PntTangent(lambda, Q);
        } else if ((Rzero(P->y) ^ 1) && Rzero(Q->y)){
            PntTangent(lambda, P);
        } else {
            return NULL;
        }
    } else {
        slope(lambda, Q, P);
    }

    if (lambda == NULL) return NULL;

    Rational *tmp;
    tmp = Rpow(lambda, 2);
    if (tmp != NULL) {
        Rs(tmp, tmp, P->x);
        if (tmp != NULL) {
            Rs(R->x, tmp, Q->x);
        } else { return NULL; }
    } else { return NULL; }

    if (R->x != NULL) {
        Rs(tmp, R->x, P->x);
        if (tmp != NULL) {
            Rm(tmp, lambda, tmp);
            if (tmp != NULL) {
                Ra(R->y, P->y, tmp);
            } else return NULL;
        } else return NULL;
    } else return NULL;

    return R;
}


#endif
