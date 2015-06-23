CXXFLAGS         = -std=c++11 -fPIC
CXX            = clang++-3.6
INPUT_FORTRAN  = ../fortran/lcm/lcm.f
INPUT_LLVM     = ../fortran/lcm/lcm.s
OBJ_FILES = \
	./dfg/CDFG_Node.o \
	./dfg/CDFG_Element.o \
	./dfg/CDFG_Operator.o \
	./dfg/CStateMachineGen.o \
	./dfg/COutput.o \
	./dfg/CModuleGenerator.o \
	./dfg/CModule.o \
	./core/fortrock.o
	# ./xml/COperatorManager.o \
	# ./xml/CInstancingOperator.o \


.PHONY: fortrock
fortrock: $(OBJ_FILES)
	$(CXX) -g -shared -o fortrock.so $(OBJ_FILES) $(CXXFLAGS)

./core/fortrock.o: ./core/fortrock.cpp
	$(CXX) -g ./core/fortrock.cpp `llvm-config --cflags --ldflags --libs`\
				 -c -o ./core/fortrock.o $(CXXFLAGS)

.PHONY: use
use: ./fortrock.so
	opt -load=./fortrock.so -instnamer -fortrock $(INPUT_LLVM) -S

.PHONY: llvm
llvm:
	gfortran-4.7 -O3 $(INPUT_FORTRAN) -fplugin=/usr/lib/gcc/x86_64-linux-gnu/4.7/plugin/dragonegg.so -fplugin-arg-dragonegg-emit-ir -S -o $(INPUT_LLVM)

.PHONY: count
count:;
	wc -lc ./core/*.cpp ./core/*.hpp
	wc -lc ./dfg/*.cpp ./dfg/*.hpp
	wc -lc ./xml/*.cpp ./xml/*.hpp
	wc -lc ./core/*.cpp ./core/*.hpp ./dfg/*.cpp ./dfg/*.hpp ./xml/*.cpp ./xml/*.hpp

.PHONY: clean
clean:
	rm -f ./core/*.o ./dfg/*.o ./xml/*.o *.so
