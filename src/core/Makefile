# core/Makefile

OBJS := \
	fortrock.o \
	CDebug.o \

EXTRA_OBJS := \
	../dfg/CDFG_Node.o \
	../dfg/CDFG_Parameter.o \
	../dfg/CDFG_Label.o \
	../dfg/CDFG_Wire.o \
	../dfg/CDFG_Reg.o \
	../dfg/CDFG_Mem.o \
	../dfg/CDFG_Array.o \
	../dfg/CDFG_Ram.o \
	../dfg/CDFG_Addr.o \
	../dfg/CDFG_Element.o \
	../dfg/CDFG_FcmpElem.o \
	../dfg/CDFG_StoreElem.o \
	../dfg/CDFG_Operator.o \
	../dfg/CDFG_Scheduler.o \
	../dfg/CStateMachineGen.o \
	../dfg/COutput.o \
	../dfg/CModuleGenerator.o \
	../dfg/CModule.o \
	../xml/CInstancingOperator.o \
	../xml/COperatorGenerator.o \

TARGET := ../fortrock.so
TARGETS := $(TARGET)

include ../Makefile.in

$(TARGET): $(OBJS) $(EXTRA_OBJS)
	$(CXX) -g -shared -o $@ $(OBJS) $(EXTRA_OBJS) $(CXXFLAGS) $(LIBS) $(LDFLAGS)

CDebug.o: CDebug.cpp CDebug.hpp
fortrock.o: fortrock.cpp fortrock.hpp
