/* elliptic point struct header file */

#include "rational.h"
#include "rmath.h"
#ifndef ELLIPTIC_H
#define ELLIPTIC_H


typedef struct {
    long int a;
    long int b;
} Curve;

typedef struct {
    Rational *x;
    Rational *y;
    Curve *E;
} Point;

int Cequal(Curve *A, Curve *B){
    return ((A->a == B->a) && (A->b == B->b));
}

int Pequal(Point *P, Point *Q){
    int Cequal(Curve *, Curve *);
    if (Cequal(P->E, Q->E)) return (Requal(P->x,Q->x) && Requal(P->y,Q->y));
    return 0;
}

int onCurve(Point *P){
    return (pow(P->y, 2) == (pow(P->x,3) + P->E->a * P->x + P->E->b))
}

long int PntTangent(Point *P){
    return ((3 * pow(P->x, 2) + P->E->a)/(2 * P->y));
}

Point *dble(Point *P){
    long int PntTangent(Point *);

    if (!onCurve(P)){
        return NULL;
    }
    
    if (P->y != 0){
        Point *R = (Point *) malloc(sizeof(Point));
        R->E = P->E;
        long int lambda = PntTangent(P);
        R->x = pow(lambda, 2) - (2 * P->x);
        R->y = lambda * (P->x - R->x) - P->y;
        return R; 
    } else {
        return NULL;
    }
}

Point *padd(Point *P, Point *Q){
    int Cequal(Curve *, Curve *);
    int Pequal(Point *, Point *);
    long int PntTangent(Point *);
    Point *dble(Point *);

    if (!onCurve(P) || !onCurve(Q)){
        return NULL;
    }

    if (Cequal(P->E, Q->E)){
        return NULL;
    } else if (Pequal(P, Q)) {
        return dble(P);
    }

    long int lambda;
    Point *R = (Point*) malloc(sizeof(Point));
    R->E = P->E;

    if (P->x == Q->x){
        if ((P->y == 0) && (Q->y != 0)){
            lambda = PntTangent(Q);
        } else if ((P->y != 0) && (Q->y == 0)){
            lambda = PntTangent(P);
        } else {
            return NULL;
        }
    } else {
        lambda = (Q->y - P->y)/(Q->x - P->x);
    }

    R->x = pow(lambda, 2) - P->x - Q->x; 
    R->y = lambda * (P->x - R->x) - P->y; 
    return R;
}

#endif


