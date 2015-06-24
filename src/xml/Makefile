#CXX = clang++-3.6
CXX = g++
CXXFLAGS = -Werror -std=c++11 -fPIC
OBJ_FILES = \
	./CInstancingOperator.o \
	../dfg/CModuleGenerator.o \
	../dfg/COutput.o \
	../dfg/CDFG_Element.o \
	../dfg/CModule.o \
	../dfg/CStateMachineGen.o \
	../dfg/CDFG_Operator.o \
	../dfg/CDFG_Node.o \
	instance_test.o

.PHONY: test
test: $(OBJ_FILES)
	$(CXX) `xml2-config --cflags` $(OBJ_FILES) -o test $(CXXFLAGS) `xml2-config --libs`

instance_test.o: instance_test.cpp
	$(CXX) `xml2-config --cflags` -c instance_test.cpp $(CXXFLAGS) `xml2-config --libs`

COperatorManager.o: COperatorManager.cpp
	$(CXX) `xml2-config --cflags` -c COperatorManager.cpp $(CXXFLAGS) `xml2-config --libs`

CInstancingOperator.o: CInstancingOperator.cpp
	$(CXX) `xml2-config --cflags` -c CInstancingOperator.cpp $(CXXFLAGS) `xml2-config --libs`

.PHONY:	clean
clean:
	rm -f *.o $(OUTPUT)
