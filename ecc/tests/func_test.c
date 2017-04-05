
#include <stdio.h>
#include "..\lib\rmath.h"
#include "..\lib\rational.h"
#include "..\lib\epoints.h"

#include <time.h>

int main(){
    Q *Rpow(Q*, int);
    printf("Running unittests on elliptic.h\n");

    printf("Creating curve y**2 = x**3 + 12 * x + 100\n");
    Curve *M = malloc(sizeof(Curve));
    M->a = 12;
    M->b = 100;

    printf("Creating point (0,10)\n");
    Point *P = malloc(sizeof(Point));
    P->x = malloc(sizeof(Q));
    P->y = malloc(sizeof(Q));
    P->E = M;
    
    Q *a = malloc(sizeof(Q));
    Q *b = malloc(sizeof(Q));
    a->sgn = 0;
    b->sgn = 0;
    a->m = 0;
    a->n = 0;
    b->m = 10;
    b->n = 1;

    P->x = a;
    P->y = b;

    printf("Assigned Points\n");
    if (onCurve(P)){
        printf("The point (0,10) is on the curve\n");
    } else {
        printf("The point (0,10) is not on the curve\n");
        printf("Eq :: %d/%d**2 = %d/%d**3 + %d/%d * %d + %d\n",
                P->y->m, P->y->n, P->x->m, P->x->n, P->x->m, P->x->n, P->E->a, P->E->b);
        return 0;
    }

    Point *X = malloc(sizeof(Point));
    X = dble(P);
    printf("The doubled point is (%s%d/%d, %s%d/%d).\n", X->x->sgn ? "-" : "+",
            X->x->m, X->x->n, X->y->sgn ? "-" : "+", X->y->m, X->x->n);

    Point *R = malloc(sizeof(Point));
    if (R != NULL) {
        R = padd(P, X);

        if (R == NULL){
            printf("The addition of (%d/%d, %d/%d) and (%d/%d, %d/%d) was invalid. Check their parameters.\n",
                    P->x->m, P->x->n, P->y->m, P->y->n, X->x->m, X->x->n, X->y->m, X->y->n);
        } else {
            printf("The addition of (%d/%d, %d/%d) and (%d/%d, %d/%d) was valid.\n",
                    P->x->m, P->x->n, P->y->m, P->y->n, X->x->m, X->x->n,
                    X->y->m, X->y->n, R->x->m, R->x->m, R->y->m, R->y->n);
        }
    } else {
        puts("Allocation error on R");
    }
 
    free(P);
    free(R);
    free(M);
   
    puts("Starting iterative tests");
    X->x->m = 0;
    X->x->n = 0;
    X->y->m = 10;
    X->y->n = 1;

    clock_t start = clock(), diff;
    int j = 0;
    for (int i = 0; i < 1000000; i++){
        printf("X + X = (%d/%d, %d/%d)\n", X->x->m, X->x->n, X->y->m, X->y->n);
        X = dble(X); 
        if (X == NULL){ break; 
        } else if (onCurve(X)){
            printf("ON CURVE: (%d/%d, %d/%d) @ i: %d\n", X->x->m, X->x->n, X->y->m, X->y->n, i);
            if (j > 10){
                break;
            } else {
               j = j + 1; 
            }
        } else {
            printf("NOT ON CURVE: (%d/%d, %d/%d) @ i: %d\n", X->x->m, X->x->n, X->y->m, X->y->n, i);
            break;
        }
    }
    diff = clock() - start;

    int msec = ( (float) diff * 1000) / CLOCKS_PER_SEC;
    printf("\npadd took an average of %d seconds %d millisecond\n /\\/\\ clocks: %d\n",
            msec/1000, msec%1000, diff);

    return 0;
}

