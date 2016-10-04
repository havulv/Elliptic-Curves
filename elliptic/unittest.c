
#include <stdio.h>
#include "elliptic.h"

int main(){
    printf("Running unittests on elliptic.h\n");

    printf("Creating curve y**2 = x**3 + 12 * x + 100\n");
    Curve *M = (Curve *) malloc(sizeof(Curve));
    M->a = 12;
    M->b = 100;

    printf("Creating point (0,10)\n");
    Point *P = (Point *) malloc(sizeof(Point));
    P->E = M;
    P->x = 0;
    P->y = 10;

    if (onCurve(P)){
        printf("The point (0,10) is on the curve\n");
    } else {
        printf("The point (0,10) is not on the curve\n");
        printf("Eq :: %d**2 = %d**3 + %d * %d + %d\n",
                P->y, P->x, P->x, P->E->a, P->E->b);
        printf("Eq :: %d = %d + %d + %d\n",
                pow(P->y, 2), pow(P->x, 3), P->E->a * P->x, P->E->b);
        return 0;
    }

    Point *Q;
    Q = dble(P);
    printf("The doubled point is (%d,%d).\n", Q->x, Q->y);

    Point *R;
    R = padd(P, Q);

    if (R == NULL){
        printf("The addition of (%d, %d) and (%d, %d) was invalid. Check their parameters.",
                P->x, P->y, Q->x, Q->y);
    } else {
        printf("Adding (%d, %d) to (%d,%d) gives: (%d, %d)",
                P->x, P->y, Q->x, Q->y, R->x, R->y);
    }

    return 0;
}

