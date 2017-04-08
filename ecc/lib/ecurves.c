

#include "ecurves.h"

int Cequal(Curve *A, Curve *B) {
    return ((A->a == B->a) && (A->b == B->b));
}
