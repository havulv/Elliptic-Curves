TDIR=ecc/tests
LDIR=ecc/lib

IDIR = ecc/lib
CC=gcc
CFlAGS=-I$(LDIR)

_DEPS = ecurves.c epoints.c rational.c rmath.c
DEPS = $(patsubst %, $(IDIR)/%,$(_DEPS))

elliptic_test.exe: $(OBJ)
	gcc -o $@ $(TDIR)/func_test.c $(DEPS) $(CFLAGS)

rational_test.exe: $(OBJ)
	gcc -o $@ $(TDIR)/rational_test.c $(DEPS) $(CFLAGS)

.PHONY: all test clean

