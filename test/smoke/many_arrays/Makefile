include ../../Makefile.defs

TESTNAME     = many_arrays
TESTSRC_MAIN = many_arrays.cpp
TESTSRC_AUX  =
TESTSRC_ALL  = $(TESTSRC_MAIN) $(TESTSRC_AUX)

CLANG        = clang++
OMP_BIN      = $(AOMP)/bin/$(CLANG)
CC           = $(OMP_BIN) $(VERBOSE)
#-ccc-print-phases
#"-\#\#\#"

include ../Makefile.rules

run: $(TESTNAME)
	LIBOMPTARGET_KERNEL_TRACE=2 $(RUNENV) $(RUNPROF) ./$(TESTNAME) 2>&1 | tee $@.log
