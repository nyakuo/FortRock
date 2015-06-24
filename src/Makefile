# Makefile

INPUT_LLVM    := ../fortran/lcm/lcm.s
INPUT_FORTRAN := ../fortran/lcm/lcm.f

.PHONY: all
all:
	cd dfg; make
	cd xml; make
	cd core; make

.PHONY: use
use: ./fortrock.so
	opt -load ./fortrock.so -fortrock $(INPUT_LLVM) -S > /dev/null

.PHONY: llvm
llvm:
	gfortran-4.7 -O3 $(INPUT_FORTRAN) \
		-fplugin=/usr/lib/gcc/x86_64-linux-gnu/4.7/plugin/dragonegg.so \
		-fplugin-arg-dragonegg-emit-ir -S -o $(INPUT_LLVM)

.PHONY: count
	cd core; make count
	cd dfg; make count
	cd xml; make count

.PHONY: clean
clean:
	cd dfg; make clean
	cd xml; make clean
	cd core; make clean
