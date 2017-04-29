/* Elliptic Points header file */

#ifndef EPOINTS_H
#define EPOINTS_H

#include "ecurves.h"
#include "rational.h"
#include <stdlib.h>

// Struct for a point on a rational elliptic curve
// x, y are rational points, E is the curve assciated with the point
// It is possible to have a point (x, y) that does not exist on the curve E
typedef struct Point {
    Q *x;
    Q *y;
    Curve *E;
} Point;

// Check for equality between points
int Pequal(Point *P, Point *M);

// Create a point
Point *createPoint(void);
Point *initPoint(Q *x, Q *y, Curve *E);

// Check that the x, y coordinates are on the associated curve
// Curve is assumed to be of Weierstrass normal form
// y**2 == x**3 + x * a + b
int onCurve(Point *P);
static void *slope(Q *result, Point *P, Point *M);

// Returns the tangent slope at the point P
// (x**2 * 3 + a) / (2 * y)
static void *PntTangent(Q *result, Point *P);
Point *dble(Point *P);
Point *padd(Point *P, Point *M);

#endif
