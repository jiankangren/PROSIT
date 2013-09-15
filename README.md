PROSIT
===============================================================

A software tool for PRObabilistic deSign of Real--Time Systems

COMPILATION INSTRUCTIONS
====================================================================
COMPILING THE TOOL

Prerequisites
In order to compile PROSIT you need
   - a gcc/g++ compiler (version 4.2 or later)
   - a make utility
   - the eigen3 library installed

Compilation
   - change directory to tool subdirectory 
   - edit Makefile to specify the position of the eigen3 library
   - type make


COMPILING THE TESTS
To enable the tests. You first need to compile the CPPTEST library.
This is done as follows:
    cd ./utils/cpptest-1.1.2/ 
    ./configure --prefix=/.../utils/
    make
    make install

The next step is to clean up the tool directory. Just cd to tool and
type "make clean".
Then cd to unit-test directory and type "make". The tests can be executed
by the shell script run-tests.sh. 


COMMAND LINE USE
===============================================================

Syntax

solver [Options] file1 [file2]

file1: PMF with computation times
file2: PMF with interarrival times (quantised to the server period); this file
  is not requested for periodic tasks

OPTIONS:
GENERAL OPTIONS
-t num:			server period 
-q num:	    		reservation budget
-T num:	    		task period
-s num:	    		step to resample the computation time
-v (--verbose):		diagnostic messages

ALGORITHM
-c (--cyclic):		cyclic reduction
-o (--companion):	companion form
-l (--latouche):	latouch algorithm
   SUBOPTIONS
   -i num (--max_iterations num):	maximum number of iterations
   -e num (--epsilon num):  		resolution
   -m compact states below deadline
 
-a (--analytic):     	analytic lower bound (very sensitive to resolution)	   -d			old model flag						

USE WITH XML FILE
===================================================================
The xml file use is throgh the xml-solve command.
You may browse the demo directory for xml file examples.


