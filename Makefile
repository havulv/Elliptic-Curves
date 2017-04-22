TDIR=ecc/tests
LDIR=ecc/lib

IDIR = ecc/lib
CC=gcc
CFlAGS=-I$(LDIR) -Wall -O2

_DEPS = ecurves.c epoints.c rational.c rmat.c
_HDEPS = ecurves.h epoints.h rational.h rmat.h
DEPS = $(patsubst %, $(IDIR)/%,$(_DEPS))
HDEPS = $(patsubst %, $(IDIR)/%,$(_HDEPS))

elliptic:
	gcc $(TDIR)/func_test.c $(DEPS) $(HDEPS) -o $@ $(CFLAGS)

.PHONY: all test clean

clean:
	rm *.obj *.o
