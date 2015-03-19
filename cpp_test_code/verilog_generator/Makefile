CXX = clang++
CXXFLAGS = -Werror -std=c++11 -stdlib=libc++
OBJ_FILES = verilog_generator.o CDFG_Element.o CDFG_Node.o CDFG_Connect.o

all: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES)

CDFG_Element.o: CDFG_Element.cpp
	$(CXX) -c CDFG_Element.cpp $(CXXFLAGS)

CDFG_Node.o: CDFG_Node.cpp
	$(CXX) -c CDFG_Node.cpp $(CXXFLAGS)

CDFG_Connect.o: CDFG_Connect.cpp
	$(CXX) -c CDFG_Connect.cpp $(CXXFLAGS)

verilog_generator.o: verilog_generator.cpp
	$(CXX) verilog_generator.cpp $(CXXFLAGS)

.PHONY:	clean
clean:
	rm *.o ./a.out 2> /dev/null
