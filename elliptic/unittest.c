
#include <stdio.h>
#include "ellipticCurves.h"
#include "rmath.h"
#include "rational.h"
#include "ellipticPoints.h"

int main(){
    Rational *Rpow(Rational*, int);
    printf("Running unittests on elliptic.h\n");

    printf("Creating curve y**2 = x**3 + 12 * x + 100\n");
    Curve *M = malloc(sizeof(Curve));
    M->a = 12;
    M->b = 100;

    printf("Creating point (0,10)\n");
    Point *P = malloc(sizeof(Point));
    P->x = malloc(sizeof(Rational));
    P->y = malloc(sizeof(Rational));
    P->E = M;
    
    Rational *a = malloc(sizeof(Rational));
    Rational *b = malloc(sizeof(Rational));
    a->sgn = 0;
    b->sgn = 0;
    a->m = 0;
    a->n = 0;
    b->m = 10;
    b->n = 1;

    P->x = a;
    P->y = b;

    printf("Assigned Points\n");
    printf("Rpow works\n");
    if (onCurve(P)){
        printf("The point (0,10) is on the curve\n");
    } else {
        printf("The point (0,10) is not on the curve\n");
        printf("Eq :: %d/%d**2 = %d/%d**3 + %d/%d * %d + %d\n",
                P->y->m, P->y->n, P->x->m, P->x->n, P->x->m, P->x->n, P->E->a, P->E->b);
        printf("Eq :: %d/%d = %d/%d + %d/%d + %d\n",
                Rpow(P->y, 2)->m, Rpow(P->y, 2)->n, Rpow(P->x, 3)->m, Rpow(P->x, 3)->n,
                RmI(P->x, P->E->a)->m, RmI(P->x, P->E->a)->n, P->E->b);
        return 0;
    }

    Point *Q;
    Q = dble(P);
    printf("The doubled point is (%s%d/%d, %s%d/%d).\n", Q->x->sgn ? "-" : "+",
            Q->x->m, Q->x->n, Q->y->sgn ? "-" : "+", Q->y->m, Q->x->n);

    Point *R = malloc(sizeof(Point));
    R = padd(P, Q);

    if (R == NULL){
        printf("The addition of (%d/%d, %d/%d) and (%d/%d, %d/%d) was invalid. Check their parameters.",
                P->x->m, P->x->n, P->y->m, P->y->n, Q->x->m, Q->x->n, Q->y->m, Q->y->n);
    } else {
        printf("The addition of (%d/%d, %d/%d) and (%d/%d, %d/%d) was valid.",
                P->x->m, P->x->n, P->y->m, P->y->n, Q->x->m, Q->x->n,
                Q->y->m, Q->y->n, R->x->m, R->x->m, R->y->m, R->y->n);
    }

    return 0;
}

