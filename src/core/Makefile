# core/Makefile

OBJS := fortrock.o

EXTRA_OBJS := \
	../dfg/CDFG_Node.o \
	../dfg/CDFG_Element.o \
	../dfg/CDFG_Operator.o \
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

fortrock.o: fortrock.cpp fortrock.hpp
