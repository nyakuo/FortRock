CXXFLAGS         = -std=c++11
#CXX            = clang++-3.6
CXX            = g++
INPUT_FORTRAN  = ../fortran/lcm/lcm.f
INPUT_LLVM     = ../fortran/lcm/lcm.s
OUTPUT_VERILOG = output.v

.PHONY: fortrock
fortrock: ./core/fortrock.cpp
	$(CXX) -g ./core/fortrock.cpp `llvm-config-3.3 --cflags --ldflags --libs` -c -o fortrock.o $(CXXFLAGS)
	$(CXX) -g -shared -o fortrock.so fortrock.o $(CXXFLAGS)

.PHONY: use
use: ./fortrock.so
	opt-3.3 -load=./fortrock.so -instnamer -fortrock $(INPUT_LLVM) -S 1> /dev/null 2> $(OUTPUT_VERILOG)

.PHONY: llvm
llvm:
	gfortran-4.7 -O3 $(INPUT_FORTRAN) -fplugin=/usr/lib/gcc/x86_64-linux-gnu/4.7/plugin/dragonegg.so -fplugin-arg-dragonegg-emit-ir -S

.PHONY: clean
clean:
	rm -f *.o *.so
