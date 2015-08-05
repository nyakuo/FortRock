# xml/Makefile

OBJS := \
	CInstancingOperator.o \
	instance_test.o \
	COperatorManager.o \
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
	../dfg/CModule.o \
	../dfg/CDFG_Element.o \
	../dfg/CStateMachineGen.o \

TARGET := xml_test
TARGETS := $(TARGET)

include ../Makefile.in

$(TARGET): $(OBJS) $(EXTRA_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(EXTRA_OBJS) $(LIBS) $(LDFLAGS)

instance_test.o: instance_test.cpp

COperatorManager.o: COperatorManager.cpp COperatorManager.hpp

CInstancingOperator.o: CInstancingOperator.cpp CInstancingOperator.hpp

COperatorGenerator.o: COperatorGenerator.cpp COperatorGenerator.hpp
