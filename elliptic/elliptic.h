/* elliptic point struct header file */

#ifndef ELLIPTIC_H
#define ELLIPTIC_H

typedef struct {
    long int a;
    long int b;
} Curve;

typedef struct {
    long int x;
    long int y;
    Curve *E;
} Point;

long int pow(long int base, int power){
    if (power < 0){
        return pow( 1 / base, -power);
    } else if (power == 0) {
        return (long int) 1;
    } else if (power == 1) {
        return base;
    } else if ((power % 2) == 0){
        return pow(base * base, power/2);
    } else {
        return pow(base * base, (power-1)/2);
    }
}


int Cequal(Curve *A, Curve *B){
    if ((A->a == B->a) && (A->b == B->b)){
        return 1;
    } else {
        return 0;
    }
}

int Pequal(Point *P, Point *Q){
    int Cequal(Curve *, Curve *);

    if (Cequal(P->E, Q->E)){
        if ((P->x == Q->x) && (P->y == Q->y)){
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int onCurve(Point *P){
    if (pow(P->y, 2) == pow(P->x,3) + P->E->a * P->x + P->E->b){
        return 1;
    } else {
        return 0;
    }
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


