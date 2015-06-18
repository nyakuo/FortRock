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

/**
   入力された数を表すために必要なビット幅を計算する
   @param[in] num 表現したい種類の数
   @return 必要なビット幅
 */
unsigned
FortRock::_get_required_bit_width
(const unsigned & num) {
  return (unsigned)round(sqrt(num + 1));
}

/**
   moduleの名前を取得
   @param[in] M モジュールの参照
   @return モジュールの名前
 */
std::string
FortRock::_get_module_name
(const Module & M) {
  std::string mod_name = M.getModuleIdentifier();
  std::regex pattern(".*/(.*)\\.s");
  std::smatch match;
  std::regex_match(mod_name, match, pattern);

  // ファイル名の抽出
  mod_name = std::string(match[match.size()-1].str(),
                         1,
                         match[match.size()-1].str().length() - 3);

  return mod_name;
} // _get_module_name

/**
   moduleの入出力を定義
 */
void FortRock::_set_IO
(const Module::FunctionListType::iterator & funct) {
  auto arg_it  = funct->arg_begin();
  auto arg_end = funct->arg_end();

  for(auto num = 1; // 最後の引数(返り値)を知るため
      arg_it != arg_end;
      ++num, ++arg_it) {
    auto type = arg_it->getType();

    if(type->isPointerTy()) // ポインタならポインタの型を取得
      type = type->getPointerElementType();

    //! @todo 浮動小数点対応
    if(type->isIntegerTy()) {
      auto node = std::make_shared<CDFG_Node>
        (CDFG_Node(arg_it->getName(),
                   type->getPrimitiveSizeInBits(),
                   true, //! @todo isSigned対応
                   CDFG_Node::eNode::IN));

      if(num == funct->arg_size()) // 最後の引数は返り値
        node->set_type(CDFG_Node::eNode::OUT);

      this->_module_gen->add_node(node);
    } // if
  } // for
} // _set_IO

/**
   FortRock passが最初に呼ばれる関数
 */
bool FortRock::runOnModule
(Module &M) {
  this->_module_gen = std::make_shared<CModuleGenerator>
  ("output.v", this->_get_module_name(M));

  // -------------------- Functions --------------------
  auto it = M.begin();
  auto end = M.end();

  try {
  if(++it != end)
    throw "ERROR: 1つのファイルに記述できるのは1つのモジュールだけです";
  }
  catch (char * str) {
    std::cerr << str << std::endl;
    return true;
  }

  this->_set_IO(--it);

  _grub_labels(it);
  _grub_variables(it);

  this->_module_gen->generate();
  // --------------------------------------------------

   // std::string err;
   // raw_fd_ostream file("output.txt", err, sys::fs::F_None);
   // file << "test\n";
   // file.close();
                       //                       sys::fs::OpenFlags::F_Text);

   // -------------------- debug --------------------
  // std::list<Variable>::iterator debit = variables.begin();
  // std::list<Variable>::iterator debend = variables.end();
  // errs() << "/*\n";

  // for(;debit != debend; ++debit)
  //   errs() << debit->to_string() << "\n";

  // std::list<Label>::iterator l_it = labels.begin();
  // std::list<Label>::iterator l_end = labels.end();

  // for(; l_it != l_end; ++l_it)
  //   errs() << l_it->to_string() << "\n";

  // errs() << "*/\n";
  // -----------------------------------------------

  return false;
} // runOnModule

/**
   各命令の処理関数を呼び出す
   @brief 処理関数がモジュールはDFGの登録などを行う
 */
void FortRock::_parse_instructions
(const Instruction * inst) {
  try {
    switch (inst->getOpcode()) {
    case RET:    this->_add_load_node(inst); break;
    case BR:     this->_add_load_node(inst); break;
    case LOAD:   this->_add_load_node(inst); break;
    case STORE:  this->_add_load_node(inst); break;
    case ICMP:   this->_add_load_node(inst); break;
    case PHI:    this->_add_load_node(inst); break;
    case SELECT: this->_add_load_node(inst); break;
    case SREM:   this->_add_load_node(inst); break;
    case MUL:    this->_add_load_node(inst); break;
    case SDIV:   this->_add_load_node(inst); break;
    default:
      throw std::string(std::string("ERROR:")
                        + std::string(inst->getOpcodeName())
                        + " "
                        + std::to_string(inst->getOpcode())
                        + " 未定義のオペランド\n");
    } // switch
  } // try
  catch (std::string err) {
    errs() << err;
  } // catch
} // _parse_instructions

void FortRock::_add_load_node
(const Instruction * inst) {
  //  auto elem = std::make_shared<CDFG_Element>
  //    (CDFG_Element(
}

/**
 * プログラムで使用するすべてのレジスタを取得し
 * variablesに格納する
 * Labelについても列挙し，格納する
 */
void FortRock::_grub_variables
(const Module::FunctionListType::iterator &funct) {
  BranchInst * binst;
  Value      * value;
  Type       * type;
  Variable     var;
  std::shared_ptr<CDFG_Node> node;

  for (auto it = inst_begin(*funct); //funct->begin();
           it != inst_end(*funct);   // funct->end();
       ++it) {

    if (!it->use_empty()) {
      // 命令に対応するオペランド数の指定
      int getop = 1;
      switch(it->getOpcode()) {
      case LOAD:   getop = 1; break;
      case ICMP:   getop = 2; break;
      case PHI:    getop = 2; break;
      case SELECT: getop = 3; break;
      case SREM:   getop = 2; break;
      case MUL:    getop = 2; break;
      case SDIV:   getop = 2; break;
      default:
        errs() << "ERROR:"
               << it->getOpcodeName() << " "
               << it->getOpcode() << " 未定義のオペランド\n";
        getop = 0;
        break;
      } // if

      // 未定義のregの追加
      for(auto i=0; i<getop; ++i) {
        value = it->getOperand(i);
        type = value->getType();

        if(type->isPointerTy())
          type = type->getPointerElementType();

        if(!value->hasName()) //! @todo 定数はパスしている
          continue;

        node = std::make_shared<CDFG_Node>
          (CDFG_Node(value->getName(),
                     type->getPrimitiveSizeInBits(),
                     true, //! @todo is signedが常にtrue
                     CDFG_Node::eNode::REG));

        if (!this->_module_gen->find_node(node))
          this->_module_gen->add_node(node);
      } // for
    } // if
    else {
      try {
        switch(it->getOpcode()) {
        case RET:
          break;

        case BR:
          binst = dyn_cast<BranchInst>(&*it);
          value = binst->getCondition();
          type = value->getType();

          if(type->isPointerTy())
            type = type->getPointerElementType();

          node = std::make_shared<CDFG_Node>
            (CDFG_Node(value->getName(),
                       type->getPrimitiveSizeInBits(),
                       true,
                       CDFG_Node::eNode::REG));

          if (!this->_module_gen->find_node(node))
            this->_module_gen->add_node(node);
          break;

        case STORE:
          for(auto i=0; i<2; ++i) {
            value = it->getOperand(i);
            type = value->getType();

            if(type->isPointerTy())
              type = type->getPointerElementType();

            node = std::make_shared<CDFG_Node>
              (CDFG_Node(value->getName(),
                         type->getPrimitiveSizeInBits(),
                         true,
                         CDFG_Node::eNode::REG));

            if (!this->_module_gen->find_node(node))
              this->_module_gen->add_node(node);
          }
          break;

        default:
          throw std::string(std::string("ERROR:")
                            + std::string(it->getOpcodeName())
                            + " "
                            + std::to_string(it->getOpcode())
                            + " 未定義のオペランド\n");
          break;
        } // switch
      } // try
      catch(std::string err) {
        errs() << err;
      }
    } // else
  } // for
} // _grub_variables

/**
   ステートマシンのラベル(ステート)の列挙
   @param[in] funct CFDの参照
   @note ラベルはNodeとしてインスタンス化され，追加される
   @attention このタイミングでState管理NodeがCModuleに追加される
 */
void FortRock::_grub_labels
(const Module::FunctionListType::iterator & funct) {
  int num_label
    = std::distance(funct->begin(), funct->end());
  auto label_bit_width
    = this->_get_required_bit_width(num_label + 1);

  // labelの追加
  auto ite = funct->begin();
  auto i = 0;
  for(;ite != funct->end();
      ++ite, ++i) {
    auto label_node = std::make_shared<CDFG_Node>
      (CDFG_Node(ite->getName(),
                 label_bit_width,
                 false,
                 CDFG_Node::eNode::LABEL,
                 i));

    if (!this->_module_gen->find_node(label_node))
      this->_module_gen->add_node(label_node);
  } // for

  // state_nodeの追加
  auto state_node = std::make_shared<CDFG_Node>
    (CDFG_Node(this->CUR_STATE_NAME,
               label_bit_width,
               false,
               CDFG_Node::eNode::STATE));
  auto prev_state_node = std::make_shared<CDFG_Node>
    (CDFG_Node(this->PREV_STATE_NAME,
               label_bit_width,
               false,
               CDFG_Node::eNode::PREV_STATE));
} // _grub_labels
#if 0
/**
 * moduleのI/Oやレジスタ，ワイヤの宣言の出力
 * @return input, output, reg, wireの宣言部
 */
std::string FortRock::print_varlist(void) {
  std::string ret_str;
  unsigned now_width;
  bool printed;

  auto it = variables.begin();
  auto end = variables.end();

  ret_str += indent() + "input clk, res;\n";
  ret_str += indent() + "output fin;\n\n";

  ret_str +=  indent() + "reg fin;\n\n";

  // input
  {
    variables.sort();

    now_width = -1;
    printed = false;

    for(; it != end; ++it) {

      if( it->is_input() && // inputの場合
          !it->is_output() ) {

        printed = true;

        if(now_width != it->get_bit_width()) {
          if(now_width != -1) { // 初回でない場合
            ret_str += ";\n";
          }
          now_width = it->get_bit_width();

          ret_str += indent() + "input  "
            + print_bit_width(*it)
            + " " + it->get_name();
        }
        else {
          ret_str += ", " + it->get_name();
        }
      } // if input
    } // for
    if(printed)
      ret_str += ";\n";
  }

  // output
  {
    it = variables.begin();

    printed = false;
    now_width = -1;

    for(; it!=end; ++it) {

      if(!it->is_input() && // outputの場合
         it->is_output() ) {
        printed = true;

        if(now_width != it->get_bit_width()) {
          if(now_width != -1) { // 初回でない場合
            ret_str += ";\n";
          }
          now_width = it->get_bit_width();

          ret_str += indent() + "output "
            + print_bit_width(*it)
            + " " + it->get_name();
        }
        else {
          ret_str += ", " + it->get_name();
        }
      } // if output
    } // for
    if(printed)
      ret_str += ";\n\n";
  }

  // reg
  {
    it = variables.begin();

    printed = false;
    now_width = -1;

    for(; it!=end; ++it) {
      if(Variable::REG == it->get_type() &&
         !it->is_input() ) {
        printed = true;

        if(now_width != it->get_bit_width()) {
          if(now_width != -1) { // 初回でない場合
            ret_str += ";\n";
          }
          now_width = it->get_bit_width();

          ret_str += indent() + "reg    "
            + print_bit_width(*it)
            + " " + it->get_name();
        }
        else {
          ret_str += ", " + it->get_name();
        }
      } // if reg
    } // for
    if(printed)
      ret_str += ";\n\n";
  }

  // todo: wire
  // todo: parameter

  return ret_str;
}

std::string FortRock::print_RET(const Instruction * inst) {
  std::string ret_str;

  ret_str = PREV_STR_NAME;
  ret_str += " = ";
  ret_str += CUR_STATE_NAME;
  ret_str += ";\n";

  ret_str += indent();
  ret_str += CUR_STATE_NAME;
  ret_str += " = " + std::to_string(state_bit_width());
  ret_str += "'d" + std::to_string(labels.size());
  ret_str += ";";

  return ret_str;
}

/**
 * BR命令の出力
 * ステートの保存と更新による分岐を行う
 */
std::string FortRock::print_BR(const Instruction * inst) {
  std::string ret_str;
  Value * value;
  Variable var;
  Label label;
  std::list<Variable>::iterator v_it;
  std::list<Label>::iterator l_it;
  BasicBlock * bb;
  BranchInst * bi;
  bi = dynamic_cast<BranchInst*>(const_cast<Instruction*>(inst));

  ret_str = PREV_STR_NAME;
  ret_str += " = ";
  ret_str += CUR_STATE_NAME;
  ret_str += ";\n";
  ret_str += indent();
  ret_str += CUR_STATE_NAME;
  ret_str += " = (1'b1 == ";
  value = bi->getCondition();
  var.set_asm_name(value->getName());
  v_it = std::find(variables.begin(),
                   variables.end(),
                   var);
  ret_str += v_it->get_name() + ") ? ";

  for(int i=0; i<bi->getNumSuccessors(); ++i) {
    bb = bi->getSuccessor(i);
    label.set_asm_name(bb->getName());
    l_it = std::find(labels.begin(),
                     labels.end(),
                     label);
    ret_str += l_it->get_name();
    if(i==0) ret_str += " : ";
  }

  ret_str += ";";

  return ret_str;
}

/**
 * LOAD命令の出力
 */
std::string FortRock::print_LOAD(const Instruction * inst) {
  std::string ret_str;
  Variable var;
  std::list<Variable>::iterator var_it;

  var.set_asm_name(inst->getName());
  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);
  ret_str = var_it->get_name();

  var.set_asm_name(inst->getOperand(0)->getName());

  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);
  ret_str += " = " + var_it->get_name() + ";";

  return ret_str;
}

/**
 * STORE命令の出力
 */
std::string FortRock::print_STORE(const Instruction * inst) {
  std::string ret_str;
  Variable var;
  std::list<Variable>::iterator var_it;

  // 保存対象のレジスタ名の取得
  //  var.set_asm_name(inst->getName());
  var.set_asm_name(inst->getOperand(1)->getName());
  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);

  ret_str = var_it->get_name();

  // 保存元のレジスタ名の取得
  var.set_asm_name(inst->getOperand(0)->getName());
  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);

  ret_str += " = " + var_it->get_name() + ";";
  return ret_str;
}

/**
 * ICMP命令の出力
 * @todo 定数を1に固定している
 */
std::string FortRock::print_ICMP(const Instruction * inst) {
  std::string ret_str;
  Value * operand;
  Variable var;
  std::list<Variable>::iterator var_it;
  CmpInst * cmp_inst;

  var.set_asm_name(inst->getName());
  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);
  ret_str = var_it->get_name() + " = (";

  operand = inst->getOperand(0);
  var.set_asm_name(operand->getName());

  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);

  ret_str += var_it->get_name() + " ";

  cmp_inst = dynamic_cast<CmpInst*>(const_cast<Instruction*>(inst));

  switch(cmp_inst->getPredicate()) {
  case CmpInst::ICMP_EQ: ret_str += "= "; break;
  case CmpInst::ICMP_NE: ret_str += "!= "; break;
  case CmpInst::ICMP_UGT:
  case CmpInst::ICMP_SGT: ret_str += "> "; break;
  case CmpInst::ICMP_UGE:
  case CmpInst::ICMP_SGE: ret_str += ">= "; break;
  case CmpInst::ICMP_ULT:
  case CmpInst::ICMP_SLT: ret_str += "< "; break;
  case CmpInst::ICMP_ULE:
  case CmpInst::ICMP_SLE: ret_str += "<= "; break;
  }

  operand = inst->getOperand(1);
  var.set_asm_name(operand->getName());

  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);

  if(var_it == variables.end()) {
    ret_str += "1);";
  }
  else
    ret_str += var_it->get_name() + ");";

  return ret_str;
}

std::string FortRock::print_PHI(const Instruction * inst) {
  std::string ret_str;
  std::string destination;
  PHINode * phinode;
  BasicBlock * bb;
  Value * value;
  Variable var;
  Label label;
  std::list<Variable>::iterator v_it;
  std::list<Label>::iterator l_it;

  var.set_asm_name(inst->getName());
  v_it = std::find(variables.begin(),
                   variables.end(),
                   var);
  destination = v_it->get_name();

  phinode = dynamic_cast<PHINode*>(const_cast<Instruction*>(inst));

  ret_str = "case(";
  ret_str += PREV_STR_NAME;
  ret_str += ")\n";
  indent_right();

  for(int i=0; i<phinode->getNumIncomingValues(); ++i) {
    bb = phinode->getIncomingBlock(i);
    label.set_asm_name(bb->getName());

    l_it = std::find(labels.begin(),
                     labels.end(),
                     label);

    ret_str += indent() + l_it->get_name() + " : "
      + destination + " = ";

    value = phinode->getIncomingValue(i);
    var.set_asm_name(value->getName());

    v_it = std::find(variables.begin(),
                     variables.end(),
                     var);
    ret_str += v_it->get_name() + ";\n";

  }

  indent_left();
  ret_str += indent() + "endcase";

  return ret_str;
}

/**
 * SELECT命令の出力
 * @todo 条件を定数に固定している
 */
std::string FortRock::print_SELECT(const Instruction * inst) {
  // dist = (a == 1'b1) ? b : c;
  //  %. = select i1 %2, i32 %1, i32 %0
  std::string ret_str;
  Value * operand;
  Variable var;
  std::list<Variable>::iterator var_it;

  var.set_asm_name(inst->getName());
  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);
  ret_str = var_it->get_name() + " = (";

  for(int i=0; i<3; ++i) {
    operand = inst->getOperand(i);
    var.set_asm_name(operand->getName());

    var_it = std::find(variables.begin(),
                       variables.end(),
                       var);

    ret_str += var_it->get_name();
    switch(i) {
    case 0: ret_str += " == 1'b1) ? "; break;
    case 1: ret_str += " : ";  break;
    case 2: ret_str += ";";    break;
    }
  }

  return ret_str;
}

/**
 * SREM命令の出力 (剰余)
 */
std::string FortRock::print_SREM(const Instruction * inst) {
  // a = b % c
  // %3 = srem i32 %., %.1
  std::string ret_str;
  Value * operand;
  Variable var;
  std::list<Variable>::iterator var_it;

  var.set_asm_name(inst->getName());
  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);
  ret_str = var_it->get_name() + " = ";

  for(int i=0; i<2; ++i) {
    operand = inst->getOperand(i);
    var.set_asm_name(operand->getName());

    var_it = std::find(variables.begin(),
                       variables.end(),
                       var);

    ret_str += var_it->get_name();
    switch(i) {
    case 0: ret_str += " % "; break;
    case 1: ret_str += ";";  break;
    }
  }

  return ret_str;
}

/**
 * MUL命令の出力
 */
std::string FortRock::print_MUL(const Instruction * inst) {
  std::string ret_str;
  Value * operand;
  Variable var;
  std::list<Variable>::iterator var_it;

  var.set_asm_name(inst->getName());
  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);
  ret_str = var_it->get_name() + " =";

  for(int i=0; i<2; ++i) {
    operand = inst->getOperand(i);
    var.set_asm_name(operand->getName());

    var_it = std::find(variables.begin(),
                       variables.end(),
                       var);
    if(i==0) ret_str += " " + var_it->get_name();
    else ret_str += " * " + var_it->get_name() + ";";
  }

  return ret_str;
}

/**
 * SDIV命令の出力
 */
std::string FortRock::print_SDIV(const Instruction * inst) {
  std::string ret_str;
  Value * operand;
  Variable var;
  std::list<Variable>::iterator var_it;

  var.set_asm_name(inst->getName());
  var_it = std::find(variables.begin(),
                     variables.end(),
                     var);
  ret_str = var_it->get_name() + " =";

  for(int i=0; i<2; ++i) {
    operand = inst->getOperand(i);
    var.set_asm_name(operand->getName());

    var_it = std::find(variables.begin(),
                       variables.end(),
                       var);
    if(i==0) ret_str += " " + var_it->get_name();
    else ret_str += " / " + var_it->get_name() + ";";
  }

  return ret_str;
}

/**
 * 各命令の出力関数を呼び出す
 */
std::string FortRock::print_instruction(const Instruction * inst) {
  switch(inst->getOpcode()) {
  case RET:    return print_RET(inst);
  case BR:     return print_BR(inst);
  case LOAD:   return print_LOAD(inst);
  case STORE:  return print_STORE(inst);
  case ICMP:   return print_ICMP(inst);
  case PHI:    return print_PHI(inst);
  case SELECT: return print_SELECT(inst);
  case SREM:   return print_SREM(inst);
  case MUL:    return print_MUL(inst);
  case SDIV:   return print_SDIV(inst);
  default:
    errs() <<  "ERROR:" << inst->getOpcodeName()
           << "(" << inst->getOpcode() << ")"
           << " 未定義のオペランド\n";
    return "";
  }
}

/**
 * always文の出力
 */
std::string FortRock::print_always(const Module::FunctionListType::iterator &funct) {
  std::list<Instruction*> insts;
  std::string ret_str = indent() + "always @(posedge clk)\n";  indent_right();

  ret_str += indent() + "begin\n";  indent_right();

  // 初期化部の出力 --------------------
  ret_str += indent() + "if(res)\n";  indent_right();
  ret_str += indent() + "begin\n";    indent_right();

  ret_str += indent() + "fin = 1'b0;\n";
  ret_str += indent() + PREV_STR_NAME + " = "
    + std::to_string(state_bit_width()) + "'b0;\n";
  ret_str += indent() + CUR_STATE_NAME + " = "
    + std::to_string(state_bit_width()) + "'b0;\n";

  std::list<Variable>::iterator v_it = variables.begin();
  std::list<Variable>::iterator v_end = variables.end();
  for(; v_it != v_end; ++v_it) {
    if(v_it->is_output()) {
      ret_str += indent() + v_it->get_name()
        + " = " + std::to_string(v_it->get_bit_width())
        + "'b0;\n";
      break;
    }
  }

  indent_left();
  ret_str += indent() + "end // if res\n";  indent_left();

  // ステートの出力 --------------------
  ret_str += indent() + "else if(fin == 1'b0)\n";  indent_right();
  ret_str += indent() + "begin\n"; indent_right();

  Function::BasicBlockListType::iterator bb_it  = funct->begin();
  Function::BasicBlockListType::iterator bb_end = funct->end();
  ilist_iterator<Instruction> it;
  ilist_iterator<Instruction> end;
  Instruction *inst = NULL;
  Label label;
  std::list<Label>::iterator l_it;

  ret_str += indent() + "case (" + CUR_STATE_NAME + ")\n";
  indent_right();
  for(; bb_it!=bb_end; ++bb_it) {
    it = bb_it->begin();
    end = bb_it->end();

    label.set_asm_name(bb_it->getName());
    l_it = std::find(labels.begin(),
                     labels.end(),
                     label);
    ret_str += indent() + l_it->get_name() + ":\n";
    indent_right();
    ret_str += indent() + "begin\n";

    for(; it != end; ++it)
      insts.push_back(&*it);

    indent_right();
    while(!insts.empty()) {
      inst = insts.front();
      insts.pop_front();

      ret_str += indent() + print_instruction(inst) + "\n";
    }
    indent_left();
    ret_str += indent() + "end\n";
    indent_left();
  }

  // 終了ステートの出力
  ret_str += indent() + std::to_string(state_bit_width());
  ret_str += "'d" + std::to_string(labels.size()) + ":\n";

  indent_right();
  ret_str += indent() + "begin\n";

  indent_right();
  ret_str += indent() + "fin = 1'b1;\n";

  indent_left();
  ret_str += indent() + "end\n"; indent_left(); indent_left();
  ret_str += indent() + "endcase\n"; indent_left();
  ret_str += indent() + "end // if fin\n"; indent_left();
  // ----------------------------------------

  indent_left();

  return ret_str + indent() + "end // always\n";
}
#endif
char FortRock::ID = 0;

// ==================================================
// External Interface declaration
// ==================================================

static RegisterPass<FortRock> X("fortrock", "Fortran to Verilog", false, false);
