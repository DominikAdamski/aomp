include ../../Makefile.defs

TESTNAME     = clang-325070
TESTSRC_MAIN = clang-325070.cpp
TESTSRC_AUX  =
TESTSRC_ALL  = $(TESTSRC_MAIN) $(TESTSRC_AUX)

CLANG        = clang++
CFLAGS       = -g -O0
OMP_BIN      = $(AOMP)/bin/$(CLANG)
CC           = $(OMP_BIN) $(VERBOSE)

include ../Makefile.rules
run:
	./$(TESTNAME)
	rocgdb -x doit.gdb --args ./$(TESTNAME) 0
