UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CPPFLAGS= -Wall -g -I/usr/include/eigen3 -I../utils/include -I../tool/ -I../utils/tinyxml2

LDFLAGS=-static
LDLIBS+=-L../utils/lib -lcpptest
endif

ifeq ($(UNAME), Darwin)
CPPFLAGS= -Wall -g -I/opt/local/include/eigen3 -I../utils/include -I../tool -I../utils/tinyxml2
LDLIBS+=-L../utils/lib -lcpptest
endif
CPPFLAGS+= -O8
VPATH=../tool:../utils/tinyxml2
OBJS= matrix.o closed_form.o companion.o auxiliary_func.o qbd_cr.o qbd_latouche.o qbd_compute_pi0.o matrix_io.o pmf.o cdf.o optimisation.o probabilistic_optimisation.o task_factory.o distr_factory.o qos_factory.o standard_task.o standard_qosfun.o standard_distr.o tinyxml2.o parser.o

LD=g++
CC=g++
all: solver test-generic-task-descriptor test-periodic-task-descriptor test-prob-periodic-task-descriptor test-task-factory test-optimisation test-standard-distr test-distr-factory test-qos-factory test-parser test-xml-util test-integrated-optimisation test-integrated-optimisation-beta
#all: test-prob-periodic-task-descriptor

solver: solver.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) solver.o $(LDLIBS) -o solver 

test-generic-task-descriptor: test-generic-task-descriptor.o $(OBJS) 
	$(LD) $(LDFLAGS) $(OBJS) test-generic-task-descriptor.o $(LDLIBS) -o test-generic-task-descriptor

test-periodic-task-descriptor: test-periodic-task-descriptor.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) test-periodic-task-descriptor.o $(LDLIBS) -o test-periodic-task-descriptor

test-prob-periodic-task-descriptor: test-prob-periodic-task-descriptor.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) test-prob-periodic-task-descriptor.o $(LDLIBS) -o test-prob-periodic-task-descriptor	

test-task-factory: test-task-factory.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) test-task-factory.o $(LDLIBS) -o test-task-factory	

test-optimisation: test-optimisation.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) test-optimisation.o $(LDLIBS) -o test-optimisation

test-standard-distr: test-standard-distr.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) test-standard-distr.o $(LDLIBS) -o test-standard-distr

test-distr-factory: test-distr-factory.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) test-distr-factory.o $(LDLIBS) -o test-distr-factory

test-qos-factory: test-qos-factory.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) test-qos-factory.o $(LDLIBS) -o test-qos-factory

test-parser: test-parser.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) test-parser.o $(LDLIBS) -o test-parser

test-xml-util: test-xml-util.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) test-xml-util.o $(LDLIBS) -o test-xml-util

test-integrated-optimisation: test-integrated-optimisation.o $(OBJS)
	 $(LD) $(LDFLAGS) $(OBJS) test-integrated-optimisation.o $(LDLIBS) -o test-integrated-optimisation

test-integrated-optimisation-beta: test-integrated-optimisation-beta.o $(OBJS)
	 $(LD) $(LDFLAGS) $(OBJS) test-integrated-optimisation-beta.o $(LDLIBS) -o test-integrated-optimisation-beta

clean :
	rm -f *.o *.d *~ test-generic-task-descriptor test-periodic-task-descriptor test-prob-periodic-task-descriptor test-task-factory test-optimisation test-standard-distr test-distr-factory test-qos-factory test-parser test-xml-util test-integrated-optmisation test-integrated-optimisation-beta

%.d : %.cpp
	$(CC) -MM -MF $@ $<

-include $(OBJS:.o=.d)