#ifndef _FORTROCK_H
#define _FORTROCK_H
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Config/config.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/InstIterator.h"

#include <iostream>
#include <list>
#include <regex>

#include "variable.hpp"
#include "../dfg/CModule.hpp"
#include "../dfg/COutput.hpp"
#include "../dfg/CModuleGenerator.hpp"
using namespace llvm;

/**
 * LLVMのInstructionのOpcode
 * @todo LLVMのものに置き換える
 */
enum inst_opcode {
  RET    = 1,
  BR     = 2,
  LOAD   = 27,
  STORE  = 28,
  ICMP   = 46,
  PHI    = 48,
  SELECT = 50,
  SREM   = 18,
  MUL    = 12,
  SDIV   = 15,
};

/**
 * @class FortRock
 * FortRockのメインのパスの実装
 */
class FortRock : public ModulePass  {
public:
  static char ID;
  FortRock(void)
    : ModulePass(ID)
  {}

  virtual const char *getPassName(void) const { return "FortRock: Fortran to Verilog backend"; }
  bool runOnModule(Module &M);

private:
  std::shared_ptr<CModuleGenerator> _module_gen;

  //! ステートマシンの遷移前の状態を保持するレジスタの名前
  const std::string PREV_STATE_NAME = "prev_state";

  //! ステートマシンの現在の状態を保持するレジスタの名前
  const std::string CUR_STATE_NAME = "current_state";

  // 補助関数
  std::string _get_module_name(const Module & M);
  unsigned _get_required_bit_width(const unsigned & num);

  // モジュール定義
  void _set_IO(const Module::FunctionListType::iterator & funct);
  void _grub_variables(const Module::FunctionListType::iterator &funct);
  void _grub_labels(const Module::FunctionListType::iterator &funct);

  // 命令パース
  void _parse_instructions(const Instruction * inst);
  void _add_load_node(const Instruction * inst);
}; // FortRock
#endif
