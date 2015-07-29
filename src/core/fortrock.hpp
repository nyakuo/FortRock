#ifndef _FORTROCK_H
#define _FORTROCK_H
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"

#include <iostream>
#include <list>
#include <map>
#include <regex>

#include "../dfg/CModule.hpp"
#include "../dfg/COutput.hpp"
#include "../dfg/CModuleGenerator.hpp"
#include "../xml/CInstancingOperator.hpp"
using namespace llvm;

/**
 * @class FortRock
 * @brief FortRockのメインのパスの実装
 */
class FortRock : public ModulePass  {
public:
  static char ID;
  FortRock(void)
    : ModulePass(ID),
      _step(0),
      _state(1)
  {}

  bool runOnModule(Module & M);

  virtual const char *getPassName(void) const
  { return "FortRock: Fortran to Verilog backend"; }

private:
  std::shared_ptr<CModuleGenerator> _module_gen;
  unsigned _step;  //! 現在処理中の命令の実行ステップ番号
  unsigned _state; //! 現在処理中の命令の実行ステート番号

  //! ステートマシンの遷移前の状態を保持するレジスタの名前
  const std::string _PREV_STATE_NAME = "prev_state";

  //! ステートマシンの現在の状態を保持するレジスタの名前
  const std::string _CUR_STATE_NAME = "current_state";

  //! ステートマシンの終了状態のステート(label)名
  const std::string _FINISH_STATE_NAME = "finish_state";

  //! ステートマシンのステップ信号名
  const std::string _STEP_NAME = "step";

  //! 演算器の設定ファイル名
  const std::string _OPERATOR_CONFIG_FILENAME = "./config/operator_config.xml";

  // 補助関数
  std::string _get_module_name(const Module & M);
  std::string _get_value_name(const Value * v);
  unsigned _get_required_bit_width(const unsigned & num);

  // モジュール定義
  void _set_IO(const Module::FunctionListType::iterator & funct);
  void _grub_variables(const Module::FunctionListType::iterator &funct);
  void _grub_global_variables(const Module & M);
  void _grub_labels(const Module::FunctionListType::iterator &funct);

  // 命令パース
  void _parse_instructions(const Instruction * inst);
  void _add_ret_inst(const Instruction * inst);
  void _add_br_inst(const Instruction * inst);
  void _add_load_inst(const Instruction * inst);
  void _add_store_inst(const Instruction * inst);
  void _add_icmp_inst(const Instruction * inst);
  void _add_phi_inst(const Instruction * inst);
  void _add_select_inst(const Instruction * inst);
  void _add_srem_inst(const Instruction * inst);
  void _add_mul_inst(const Instruction * inst);
  void _add_sdiv_inst(const Instruction * inst);
  void _add_add_inst(const Instruction * inst);
  void _add_sub_inst(const Instruction * inst);
  void _add_switch_inst(const Instruction * inst);
  void _add_shift_inst(const Instruction * inst,
                       const bool is_left,
                       const bool is_logical);
  void _add_and_inst(const Instruction * inst);
  void _add_or_inst(const Instruction * inst);
  void _add_xor_inst(const Instruction * inst);
  void _add_trunc_inst(const Instruction * inst);
  void _add_getelementptr_inst(const Instruction * inst);
}; // FortRock

#endif
