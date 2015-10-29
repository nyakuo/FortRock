# dfg/Makefile

OBJS := \
	CDFG_Node.o \
	CDFG_Parameter.o \
	CDFG_Label.o \
	CDFG_Wire.o \
	CDFG_Reg.o \
	CDFG_Mem.o \
	CDFG_Ram.o \
	CDFG_StoreElem.o \
	CDFG_Array.o \
	CDFG_Addr.o \
	CDFG_Element.o \
	CDFG_FcmpElem.o \
	CDFG_Operator.o \
	CDFG_Scheduler.o \
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
CDFG_Wire.o: CDFG_Wire.hpp CDFG_Wire.cpp
CDFG_Reg.o: CDFG_Reg.hpp CDFG_Reg.cpp
CDFG_Addr.o: CDFG_Addr.hpp CDFG_Addr.cpp
CDFG_Mem.o: CDFG_Mem.hpp CDFG_Mem.cpp
CDFG_Ram.o: CDFG_Ram.hpp CDFG_Ram.cpp
CDFG_Array.o: CDFG_Array.hpp CDFG_Array.cpp
CDFG_Element.o: CDFG_Element.cpp CDFG_Element.hpp
CDFG_FcmpElem.o: CDFG_FcmpElem.cpp CDFG_FcmpElem.hpp
CDFG_StoreElem.o: CDFG_StoreElem.cpp CDFG_StoreElem.hpp
CDFG_Operator.o: CDFG_Operator.cpp CDFG_Operator.hpp
CDFG_Scheduler.o: CDFG_Scheduler.cpp CDFG_Scheduler.hpp
CStateMachineGen.o: CStateMachineGen.cpp CStateMachineGen.hpp
COutput.o: COutput.cpp COutput.hpp
CModule.o: CModule.cpp CModule.hpp
CModuleGenerator.o: CModuleGenerator.cpp CModuleGenerator.hpp
