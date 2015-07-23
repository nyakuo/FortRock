# Makefile

INPUT_LLVM    := ../fortran/lcm/lcm.s
INPUT_FORTRAN := ../fortran/lcm/lcm.f

.PHONY: all dfg xml core use llvm count clean
all:
	cd dfg; make
	cd xml; make
	cd core; make

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
	wc -lc dfg/*.cpp dfg/*.hpp dfg/Makefile \
		core/*.cpp core/*.hpp core/Makefile \
		xml/*.cpp xml/*.hpp xml/Makefile 

clean:
	cd dfg; make clean
	cd xml; make clean
	cd core; make clean
