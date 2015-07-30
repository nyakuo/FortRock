# dfg/Makefile

OBJS := \
	CDFG_Node.o \
	CDFG_Parameter.o \
	CDFG_Label.o \
	CDFG_Element.o \
	CDFG_Operator.o \
	CStateMachineGen.o \
	COutput.o \
	CModuleGenerator.o \
	CModule.o \
	verilog_generator.o \

TARGET := generator
TARGETS := $(TARGET)

include ../Makefile.in

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

verilog_generator.o: verilog_generator.cpp

CDFG_Node.o: CDFG_Node.cpp CDFG_Node.hpp

CDFG_Parameter.o: CDFG_Parameter.hpp CDFG_Parameter.cpp

CDFG_Label.o: CDFG_Label.hpp CDFG_Label.cpp

CDFG_Element.o: CDFG_Element.cpp CDFG_Element.hpp

CDFG_Operator.o: CDFG_Operator.cpp CDFG_Operator.hpp

CStateMachineGen.o: CStateMachineGen.cpp CStateMachineGen.hpp

COutput.o: COutput.cpp COutput.hpp

CModuleGenerator.o: CModuleGenerator.cpp CModuleGenerator.hpp

CModule.o: CModule.cpp CModule.hpp
