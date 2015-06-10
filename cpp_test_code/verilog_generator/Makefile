CXX = clang++-3.6
CXXFLAGS = -Werror -std=c++11
OBJ_FILES = verilog_generator.o CDFG_Element.o CDFG_Node.o CDFG_Operator.o CStateMachineGen.o COutput.o
OUTPUT = generator

generator: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(OUTPUT)

CDFG_Node.o: CDFG_Node.cpp
	$(CXX) -c CDFG_Node.cpp $(CXXFLAGS)

CDFG_Element.o: CDFG_Element.cpp CDFG_Node.o
	$(CXX) -c CDFG_Element.cpp $(CXXFLAGS)

CDFG_Operator.o: CDFG_Operator.cpp
	$(CXX) -c CDFG_Operator.cpp $(CXXFLAGS)

CStateMachineGen.o: CStateMachineGen.cpp
	$(CXX) -c CStateMachineGen.cpp $(CXXFLAGS)

COutput.o: COutput.cpp
	$(CXX) -c COutput.cpp $(CXXFLAGS)

verilog_generator.o: verilog_generator.cpp
	$(CXX) -c verilog_generator.cpp $(CXXFLAGS)

.PHONY:	clean
clean:
	rm -f *.o $(OUTPUT)
