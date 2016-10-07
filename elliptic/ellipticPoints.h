/* Elliptic Points header file */

#include "rational.h"
#include "ellipticCurves.h"
#include "rmath.h"

#ifndef ELLIPTICPOINTS_H
#define ELLIPTICPOINTS_H

typedef struct {
    Rational *x;
    Rational *y;
    Curve *E;
} Point;


int Pequal(Point *P, Point *Q){
    if (Cequal(P->E, Q->E)) return (Requal(P->x,Q->x) && Requal(P->y,Q->y));
    return 0;
}

int onCurve(Point *P){
    return (Requal(Rpow(P->y, 2), 
                (Ra(Rpow(P->x,3), 
                    RaI(RmI(P->x, P->E->a), 
                        (long int) P->E->b)
                    )
                 )
            ));

}

Rational *PntTangent(Point *P){
    return ((Rd(RaI(RmI(Rpow(P->x,2), 3),
                        P->E->a), 
                    (RmI(P->y, 2))
                    )));
}

Point *dble(Point *P){

    if (!onCurve(P)){
        return NULL;
    }
    
    if (P->y != 0){
        Point *R = malloc(sizeof(Point));
        R->E = P->E;
        Rational *lambda = PntTangent(P);
        R->x = Rm(Rpow(lambda, 2),
                    RmI(P->x, 2));
        R->y = Rs(Rm(lambda, Rs(P->x, R->x)), P->y);
        return R; 
    } else {
        return NULL;
    }
}

Point *padd(Point *P, Point *Q){

    if (!onCurve(P) || !onCurve(Q)){
        return NULL;
    }

    if (Cequal(P->E, Q->E)){
        return NULL;
    } else if (Pequal(P, Q)) {
        return dble(P);
    }

    Rational *lambda;
    Point *R = malloc(sizeof(Point));
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
        lambda = Rd(Rs(Q->y, P->y), Rs(Q->x, P->x));
    }

    R->x = Rs(Rs(Rpow(lambda, 2), P->x), Q->x); 
    R->y = Rs(Rm(lambda, Rs(P->x, R->x)), P->y); 
    return R;
}


#endif
