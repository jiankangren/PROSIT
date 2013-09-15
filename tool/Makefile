UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CPPFLAGS= -Wall -g -I/usr/include/eigen3 -I../utils/tinyxml2
CPPFLAGS+= -O8
endif

ifeq ($(UNAME), Darwin)
CPPFLAGS= -Wall -g -I/opt/local/include/eigen3 -I../utils/tinyxml2
CPPFLAGS+= -O8
endif


VPATH=../utils/tinyxml2

OBJS=matrix.o closed_form.o companion.o auxiliary_func.o qbd_cr.o qbd_latouche.o qbd_compute_pi0.o matrix_io.o pmf.o cdf.o optimisation.o probabilistic_optimisation.o task_factory.o distr_factory.o qos_factory.o standard_task.o standard_qosfun.o standard_distr.o tinyxml2.o parser.o

LD=g++
CC=g++

all: solver xml-solve

solver : $(OBJS) solver.o
	$(LD) $(LDFLAGS) $(OBJS) solver.o $(LDLIBS) -o solver
xml-solve: $(OBJS) xml-solve.o
	$(LD) $(LDFLAGS) $(OBJS) xml-solve.o $(LDLIBS) -o xml-solve
clean :
	rm -f *.o *.d *~ solver

%.d : %.cpp
	$(CC) -MM -MF $@ $<

-include $(OBJS:.o=.d)