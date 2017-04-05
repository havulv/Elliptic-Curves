
#include "rational.h"
#include "ecurves.h"
#include "rmath.h"
#include "epoints.h"
#include <stdio.h>


// Check for equality between points
int Pequal(Point *P, Point *M){
    if (Cequal(P->E, M->E)) return (Requal(P->x, M->x) && Requal(P->y, M->y));
    return 0;
}

// Check that the x, y coordinates are on the associated curve
// Curve is assumed to be of Weierstrass normal form
// y**2 == x**3 + x * a + b
int onCurve(Point *P){
    Q *right = NULL;
    Q *left = NULL;

    printf("x = %d/%d, E->a = %d\n", P->x->m, P->y->n, P->E->a);
    // Begin checks for failures
    RmI(right, P->x, P->E->a);
    if (right != NULL) {
        printf("right %d/%d\n", right->m, right->n);
        // The cast is necessary because RaI takes a
        // long int and this is assumed to be an int
        RaI(right, right, (long int) P->E->b);
        Q *buff = Rpow(P->x, 3);
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

void *slope(Q *result, Point *P, Point *M) {
    Q *tmp1;
    Rs(tmp1, P->x, M->x);
    Q *tmp2;
    Rs(tmp2, P->y, M->y);
    if ((tmp1 != NULL) && (tmp2 != NULL) && (Rzero(tmp2) ^ 1)) {
        Rd(result, tmp2, tmp1);
    } else return NULL;
}

// Returns the tangent slope at the point P
// (x**2 * 3 + a) / (2 * y)
void *PntTangent(Q *result, Point *P){
    // Begin failure checks
    result = Rpow(P->x, 2);
    if (result != NULL) {
        RmI(result, result, 3);
        if (result != NULL) {
            RaI(result, result, P->E->a);
            Q *buff;
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
        Q *lambda = NULL;
        PntTangent(lambda, P);
        if (lambda == NULL) return NULL;
        Q *tmp1 = Rpow(lambda, 2);
        Q *tmp2 = NULL;
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

Point *padd(Point *P, Point *M){

    if (Cequal(P->E, M->E) ^ 1){
        return NULL;
    } else if (Pequal(P, M)) {
        return dble(P);
    }

    Q *lambda = NULL;
    Point *R = malloc(sizeof(Point));
    // Allocation check
    if (R == NULL) return NULL;
    R->E = P->E;

    // Check if the x's are equal to each other
    if (Requal(P->x, M->x)){
        if (Rzero(P->y) && (Rzero(M->y) ^ 1)){
            PntTangent(lambda, M);
        } else if ((Rzero(P->y) ^ 1) && Rzero(M->y)){
            PntTangent(lambda, P);
        } else {
            return NULL;
        }
    } else {
        slope(lambda, M, P);
    }

    if (lambda == NULL) return NULL;

    Q *tmp;
    tmp = Rpow(lambda, 2);
    if (tmp != NULL) {
        Rs(tmp, tmp, P->x);
        if (tmp != NULL) {
            Rs(R->x, tmp, M->x);
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

