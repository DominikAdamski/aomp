NOOPT        = 1
EXTRA_OMP_FLAGS = -O0
include ../../Makefile.defs

TESTNAME     = MasterBarrier
TESTSRC_MAIN = MasterBarrier.cpp
TESTSRC_AUX  =
TESTSRC_ALL  = $(TESTSRC_MAIN) $(TESTSRC_AUX)

CLANG        = clang++
OMP_BIN      = $(AOMP)/bin/$(CLANG)
CC           = $(OMP_BIN) $(VERBOSE)
#-ccc-print-phases
#"-\#\#\#"

include ../Makefile.rules
