TDIR=ecc/tests
LDIR=ecc/lib

IDIR = ecc/lib
CC=gcc
CFlAGS=-I$(LDIR)

_DEPS = ecurves.h epoints.h rational.h rmath.h
DEPS = $(patsubst %, $(IDIR)/%,$(_DEPS))

elliptic: $(OBJ)
	gcc -o $@ $(DEPS) $(CFLAGS)

