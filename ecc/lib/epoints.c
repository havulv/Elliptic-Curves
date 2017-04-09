
#include "epoints.h"
#include "rational.h"
#include "ecurves.h"
#include "rmath.h"
#include <stdlib.h>
#include <stdio.h>


// Check for equality between points
int Pequal(Point *P, Point *M){
    if (Cequal(P->E, M->E)) return (equals(P->x, M->x) && equals(P->y, M->y));
    return 0;
}

// Check that the x, y coordinates are on the associated curve
// Curve is assumed to be of Weierstrass normal form
// y**2 == x**3 + x * a + b
int onCurve(Point *P){
    Q *right = createQ(0, 0, 0);
    Q *left = createQ(0, 0, 0);

    printf("x = %d/%d, E->a = %d\n", P->x->m, P->y->n, P->E->a);
    // Begin checks for failures
    multiplyIntTo(P->x, P->E->a, right);
    if (right != NULL) {
        printf("right %d/%d\n", right->m, right->n);
        // The cast is necessary because RaI takes a
        // long int and this is assumed to be an int
        addIntTo(right, (long int) P->E->b, right);
        Q *buff = Rpow(P->x, 3);
        if (right != NULL && buff != NULL) {
            printf("buff %d/%d , right %d/%d\n", buff->m, buff->n, right->m, right->n);
            addTo(right, buff, right);
            left = Rpow(P->y, 2);
            if (right != NULL && left != NULL) {
                printf("left %d/%d == right %d/%d\n", left->m, left->n, right->m, right->n);
                return equals(left, right);
            }
        }
    }
    destroyQ(right);
    destroyQ(left);
    return 0;
}

void *slope(Q *result, Point *P, Point *M) {
    Q *tmp1 = subtract(P->x, M->x);
    Q *tmp2 = subtract(P->y, M->y);
    if ((tmp1 != NULL) && (tmp2 != NULL) && (tmp2->m && tmp2->n )) {
        divideTo(tmp2, tmp1, result);
    } else {
        destroyQ(tmp1);
        destroyQ(tmp2);
        return NULL;
    }
}

// Returns the tangent slope at the point P
// (x**2 * 3 + a) / (2 * y)
void *PntTangent(Q *result, Point *P){
    // Begin failure checks
    result = Rpow(P->x, 2);
    if (result != NULL) {
        multiplyIntTo(result, 3, result);
        if (result != NULL) {
            addIntTo(result, P->E->a, result);
            Q *buff;
            multiplyIntTo(buff, 2, P->y);
            if (result != NULL && buff != NULL) {
                divideTo(result, buff, result);
            }
        }
    }
    return NULL;
}

Point *dble(Point *P){
    
    if (P->y->m || P->y->n){
        Point *R = malloc(sizeof(Point));
        // Allocation check
        if (R == NULL) {
            free(R);
            return NULL; 
        }
        R->E = P->E;
        Q *lambda = createQ(0,0,0);
        PntTangent(lambda, P);
        if (lambda == NULL) {
            destroyQ(lambda);
            return NULL;
        }
        Q *tmp1 = Rpow(lambda, 2);
        Q *tmp2 = multiplyInt(P->x, 2);
        if ((tmp1 != NULL) && (tmp2 != NULL)) {
            subtractTo(tmp1, tmp2, R->x);
            if ( R->x != NULL) {
                subtractTo(P->x, R->x, tmp2);
                if (tmp2 != NULL) {
                    multiplyTo(tmp2, lambda, tmp2);
                    if (tmp2 != NULL) {
                        subtractTo(P->y, tmp2, R->y);
                    }
                }
            }
        }
        destroyQ(tmp1);
        destroyQ(tmp2);
    }
    return NULL;
}

Point *padd(Point *P, Point *M){

    if (Cequal(P->E, M->E) ^ 1){
        return NULL;
    } else if (Pequal(P, M)) {
        return dble(P);
    }

    Q *lambda = createQ(0,0,0);
    Point *R = malloc(sizeof(Point));
    // Allocation check
    if (R == NULL || lambda == NULL) {
        destroyQ(lambda);
        free(R);
        return NULL;
    }
    R->E = P->E;

    // Check if the x's are equal to each other
    if (equals(P->x, M->x)){
        if ((P->y->m || P->y->n) && ((M->y->m || M->y->n) ^ 1)){
            PntTangent(lambda, M);
        } else if (((P->y->m || P->y->n) ^ 1) && (M->y->m || M->y->n)){
            PntTangent(lambda, P);
        } else {
            destroyQ(lambda);
            free(R);
            return NULL;
        }
    } else {
        slope(lambda, M, P);
    }

    // this is where a goto would be necessary

    if (lambda == NULL) {
        destroyQ(lambda);
        free(R);
        return NULL;
    }

    Q *tmp= Rpow(lambda, 2);
    if (tmp != NULL) {
        subtractTo(tmp, P->x, tmp);
        if (tmp != NULL) {
            subtractTo(tmp, M->x, R->x);
        } else { 
            destroyQ(lambda);
            destroyQ(tmp);
            free(R);
            return NULL;
        }
    } else {
        destroyQ(lambda);
        destroyQ(tmp);
        free(R);
        return NULL; 
    }

    if (R->x != NULL) {
        subtractTo(P->x, R->x, tmp);
        if (tmp != NULL) {
            multiplyTo(tmp, lambda, tmp);
            if (tmp != NULL) {
                subtractTo(tmp, P->y, R->y);
            } else {
                destroyQ(lambda);
                destroyQ(tmp);
                free(R); 
                return NULL;
            }
        } else {
            destroyQ(lambda);
            destroyQ(tmp);
            free(R);
            return NULL;
        }
    } else {
        destroyQ(lambda);
        destroyQ(tmp);
        free(R);
        return NULL;
    }

    return R;
}

