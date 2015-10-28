# xml/Makefile

OBJS := \
	CInstancingOperator.o \
	instance_test.o \
	COperatorGenerator.o \

EXTRA_OBJS := \
	../dfg/CDFG_Operator.o \
	../dfg/CModuleGenerator.o \
	../dfg/COutput.o \
	../dfg/CDFG_Node.o \
	../dfg/CDFG_Parameter.o \
	../dfg/CDFG_Label.o \
	../dfg/CDFG_Wire.o \
	../dfg/CDFG_Reg.o \
	../dfg/CDFG_Mem.o \
	../dfg/CDFG_Addr.o \
	../dfg/CDFG_Ram.o \
	../dfg/CDFG_Array.o \
	../dfg/CDFG_Scheduler.o \
	../dfg/CModule.o \
	../dfg/CDFG_Element.o \
	../dfg/CDFG_FcmpElem.o \
	../dfg/CStateMachineGen.o \


TARGET := xml_test
TARGETS := $(TARGET)

include ../Makefile.in

$(TARGET): $(OBJS) $(EXTRA_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(EXTRA_OBJS) $(LIBS) $(LDFLAGS)

instance_test.o: instance_test.cpp
CInstancingOperator.o: CInstancingOperator.cpp CInstancingOperator.hpp
COperatorGenerator.o: COperatorGenerator.cpp COperatorGenerator.hpp
