CXXFLAGS         = -std=c++11 -fPIC
#CXX            = clang++-3.6
CXX            = g++
INPUT_FORTRAN  = ../fortran/lcm/lcm.f
INPUT_LLVM     = ../fortran/lcm/lcm.s
OUTPUT_VERILOG = output.v
OBJ_FILES = \
	./dfg/CDFG_Node.o \
	./dfg/CDFG_Element.o \
	./dfg/CDFG_Operator.o \
	./dfg/CStateMachineGen.o \
	./dfg/COutput.o \
	./dfg/CModuleGenerator.o \
	./dfg/CModule.o

.PHONY: fortrock
fortrock: ./core/fortrock.cpp $(OBJ_FILES)
	$(CXX) -g ./core/fortrock.cpp `llvm-config --cflags --ldflags --libs` -c -o fortrock.o $(CXXFLAGS)
	$(CXX) -g -shared -o fortrock.so fortrock.o $(OBJ_FILES) $(CXXFLAGS)

.PHONY: use
use: ./fortrock.so
	opt -load=./fortrock.so -instnamer -fortrock $(INPUT_LLVM) -S 1> /dev/null 2> $(OUTPUT_VERILOG)

.PHONY: llvm
llvm:
	gfortran-4.7 -O3 $(INPUT_FORTRAN) -fplugin=/usr/lib/gcc/x86_64-linux-gnu/4.7/plugin/dragonegg.so -fplugin-arg-dragonegg-emit-ir -S

.PHONY: clean
clean:
	rm -f *.o *.so
