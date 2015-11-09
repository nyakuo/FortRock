# Makefile

INPUT_LLVM    := ../fortran/lcm/lcm.s
INPUT_FORTRAN := ../fortran/lcm/lcm.f

include Makefile.in

SRCS := $(shell find ./ -type f -name '*.cpp')
OBJS := $(SRCS:.cpp=.o)

.PHONY: all dfg xml core use llvm count clean
all:
	cd dfg; make
	cd xml; make
	cd core; make
	$(CXX) -g -shared -o fortrock.so $(OBJS) $(LIBS) $(LDFLAGS)

dfg:
	cd dfg; make

xml:
	cd xml; make

core:
	cd core; make

use: ./fortrock.so
	opt -load ./fortrock.so -instnamer -fortrock $(INPUT_LLVM) -S
	cat output.v

llvm:
	gfortran-4.7 -O3 $(INPUT_FORTRAN) \
		-fplugin=/usr/lib/gcc/x86_64-linux-gnu/4.7/plugin/dragonegg.so \
		-fplugin-arg-dragonegg-emit-ir -S -o $(INPUT_LLVM)

count:
	cd core; make count
	cd dfg; make count
	cd xml; make count

clean:
	cd dfg; make clean
	cd xml; make clean
	cd core; make clean
