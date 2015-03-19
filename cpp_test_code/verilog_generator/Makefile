CXX = clang++
CXXFLAGS = -Werror -std=c++11 -stdlib=libc++
OBJ_FILES = verilog_generator.o CDFG_Element.o CDFG_Node.o
OUTPUT = generator

generator: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(OUTPUT)

CDFG_Node.o: CDFG_Node.cpp
	$(CXX) -c CDFG_Node.cpp $(CXXFLAGS)

CDFG_Element.o: CDFG_Element.cpp CDFG_Node.o
	$(CXX) -c CDFG_Element.cpp $(CXXFLAGS)

verilog_generator.o: verilog_generator.cpp
	$(CXX) -c verilog_generator.cpp $(CXXFLAGS)

.PHONY:	clean
clean:
	rm -f *.o $(OUTPUT)
