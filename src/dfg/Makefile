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
	CDFG_IcmpElem.o \
	CDFG_LoadElem.o \
	CDFG_TruncElem.o \
	CDFG_ShiftElem.o \
	CDFG_LogicalElem.o \
	CDFG_SwitchElem.o \
	CDFG_PhiElem.o \
	CDFG_RetElem.o \
	CDFG_BrElem.o \
	CDFG_SelectElem.o \
	CDFG_ZextElem.o \
	CDFG_Operator.o \
	CDFG_Scheduler.o \
	CDFG_Optimizer.o \
	CStateMachineGen.o \
	COutput.o \
	CModuleGenerator.o \
	CModule.o \

TARGETS := $(OBJS)

include ../Makefile.in

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
CDFG_IcmpElem.o: CDFG_IcmpElem.cpp CDFG_IcmpElem.hpp
CDFG_LoadElem.o: CDFG_LoadElem.cpp CDFG_LoadElem.hpp
CDFG_StoreElem.o: CDFG_StoreElem.cpp CDFG_StoreElem.hpp
CDFG_TruncElem.o: CDFG_TruncElem.cpp CDFG_TruncElem.hpp
CDFG_ShiftElem.o: CDFG_ShiftElem.cpp CDFG_ShiftElem.hpp
CDFG_LogicalElem.o: CDFG_LogicalElem.cpp CDFG_LogicalElem.hpp
CDFG_SwitchElem.o: CDFG_SwitchElem.cpp CDFG_SwitchElem.hpp
CDFG_PhiElem.o: CDFG_PhiElem.cpp CDFG_PhiElem.hpp
CDFG_RetElem.o: CDFG_RetElem.cpp CDFG_RetElem.hpp
CDFG_BrElem.o: CDFG_BrElem.cpp CDFG_BrElem.hpp
CDFG_SelectElem.o: CDFG_SelectElem.cpp CDFG_SelectElem.hpp
CDFG_ZextElem.o: CDFG_ZextElem.cpp CDFG_ZextElem.hpp
CDFG_Operator.o: CDFG_Operator.cpp CDFG_Operator.hpp
CDFG_Scheduler.o: CDFG_Scheduler.cpp CDFG_Scheduler.hpp
CDFG_Optimizer.o: CDFG_Optimizer.cpp CDFG_Optimizer.hpp
CStateMachineGen.o: CStateMachineGen.cpp CStateMachineGen.hpp
COutput.o: COutput.cpp COutput.hpp
CModule.o: CModule.cpp CModule.hpp
CModuleGenerator.o: CModuleGenerator.cpp CModuleGenerator.hpp
