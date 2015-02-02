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

#include "label.h"
#include "variable.h"
using namespace llvm;

#define PREV_STATE_STR "prev_state"
#define CUR_STATE_STR "current_state"

/**
 * LLVMのInstructionのOpcode
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
  unsigned            indent_level;
  const unsigned      indent_width;
  std::list<Variable> variables;
  std::list<Label>    labels;

public:
  static char ID;
  FortRock(void) : ModulePass(ID), indent_width(2),
                   indent_level(0) {}

  virtual const char *getPassName(void) const { return "FortRock: Fortran to Verilog backend"; }
  bool runOnModule(Module &M);

private:
  inline void indent_right(void) { ++indent_level; }
  inline void indent_left(void) { if(indent_level > 0) --indent_level; }
  inline std::string indent(void) {
    return print_spaces(indent_level * indent_width);
  }
  inline int state_bit_width(void) {
    return (int)round(sqrt(labels.size() + 1));
  }

  std::string print_header(const Module &M);
  std::string print_arguments(const Module::FunctionListType::iterator &funct);
  int grub_variables(const Module::FunctionListType::iterator &funct);
  int grub_labels(const Module::FunctionListType::iterator &funct);
  std::string print_varlist(void);
  std::string print_always(const Module::FunctionListType::iterator &funct);
  std::string print_spaces(const int num_space);
  std::string print_bit_width(Variable var);
  std::string print_instruction(const Instruction * inst);

  // print instruction functions
  std::string print_RET(const Instruction * inst);
  std::string print_LOAD(const Instruction * inst);
  std::string print_STORE(const Instruction * inst);
  std::string print_ICMP(const Instruction * inst);
  std::string print_PHI(const Instruction * inst);
  std::string print_SELECT(const Instruction * inst);
  std::string print_SREM(const Instruction * inst);
  std::string print_MUL(const Instruction * inst);
  std::string print_SDIV(const Instruction * inst);
  std::string print_BR(const Instruction * inst);
};

/**
 * moduleのheader(固定であるclk, res, fin)を出力する
 */
std::string FortRock::print_header(const Module &M) {
  std::string mod_name = M.getModuleIdentifier();
  std::regex pattern(".*/(.*)\\.s");
   std::smatch match;
   std::regex_match(mod_name, match, pattern);

   // ファイル名の抽出
   mod_name = std::string(match[match.size()-1].str(),
                          1,
                          match[match.size()-1].str().length() - 3);

  return  "// " + M.getModuleIdentifier() + "\n"
    + "module " + mod_name + "(clk, res, fin";
}

/**
 * moduleのarg-listの後半部分(各モジュールによって異なる部分)を出力する
 */
std::string FortRock::print_arguments(const Module::FunctionListType::iterator & funct) {
  std::string ret_str = "";
  Type *type = NULL;
  Variable var;
  int num;

  Function::arg_iterator arg_it  = funct->arg_begin();
  Function::arg_iterator arg_end = funct->arg_end();

  for(num = 1; // 最後の引数(返り値)を知るため
      arg_it != arg_end;
      ++num, ++arg_it) {
    type = arg_it->getType();

    if(type->isPointerTy()) // ポインタならポインタの型を取得
      type = type->getPointerElementType();

    if(type->isIntegerTy()) {
      var.set_name(arg_it->getName());
      var.set_asm_name(arg_it->getName());
      var.set_bit_width(type->getPrimitiveSizeInBits());
      var.set_type(Variable::REG);

      var.set_input(true);

      if(num == funct->arg_size()) { // 最後の引数は返り値
        var.set_input(false);
        var.set_output(true);
      }

      variables.push_back(var);

      ret_str += ", ";
      ret_str += arg_it->getName();
    }
    else {
      ret_str += "ERROR: can not solve the arg type\n";
    }
  }

  ret_str += ");\n";

  return ret_str;
}

bool FortRock::runOnModule(Module &M) {
  errs() << print_header(M);
  indent_right();

  // -------------------- Functions --------------------
  Module::FunctionListType::iterator it = M.begin();
  Module::FunctionListType::iterator end = M.end();

  if(++it != end) {
    errs() << "ERROR: 1つのファイルに記述できるのは1つのモジュールだけです\n";
    return true;
  }

  --it;

  if(it->getArgumentList().size() != 0)
    errs() << print_arguments(it);

  grub_labels(it);
  grub_variables(it);

  errs() << print_varlist();
  errs() << print_always(it);

  errs() << "endmodule\n";
  // --------------------------------------------------


  // -------------------- debug --------------------
  std::list<Variable>::iterator debit = variables.begin();
  std::list<Variable>::iterator debend = variables.end();
  errs() << "/*\n";

  for(;debit != debend; ++debit)
    errs() << debit->to_string() << "\n";

  std::list<Label>::iterator l_it = labels.begin();
  std::list<Label>::iterator l_end = labels.end();

  for(; l_it != l_end; ++l_it)
    errs() << l_it->to_string() << "\n";

  errs() << "*/\n";
  // -----------------------------------------------

  return false;
}

/**
 * プログラムで使用するすべてのレジスタを取得し
 * variablesに格納する
 * Labelについても列挙し，格納する
 */
int FortRock::grub_variables(const Module::FunctionListType::iterator &funct) {
  std::list<Instruction*> insts;

  inst_iterator it  = inst_begin(*funct);
  inst_iterator end = inst_end(*funct);

  Instruction * inst;
  Value * value;
  Type * type;
  Variable var;

  int num_variable = 0;

  it  = inst_begin(*funct);
  end = inst_end(*funct);

  for(; it != end; ++it)
    insts.push_back(&*it);

  while(!insts.empty()) {
    inst = insts.front();
    insts.pop_front();

    if(!inst->use_empty()) {
      // 命令に対応するオペランド数の指定
      int getop = 1;
      switch(inst->getOpcode()) {
      case LOAD:   getop = 1; break;
      case ICMP:   getop = 2; break;
      case PHI:    getop = 2; break;
      case SELECT: getop = 3; break;
      case SREM:   getop = 2; break;
      case MUL:    getop = 2; break;
      case SDIV:   getop = 2; break;
      default:
        errs() << "ERROR:" << inst->getOpcodeName() << " "
               << inst->getOpcode() << " 未定義のオペランド\n";
        getop = 0; break;
      }

      // 未定義のregの追加
      for(int i=0; i<getop; ++i) {
        value = inst->getOperand(i);
        type = value->getType();

        if(type->isPointerTy())
          type = type->getPointerElementType();

        if(!value->hasName()) // 定数はパス
          continue;

        var.set_name(value->getName());
        var.set_asm_name(value->getName());
        var.set_bit_width(type->getPrimitiveSizeInBits());
        var.set_type(Variable::REG);

        var.set_input(false);
        var.set_output(false);

        if(std::find(variables.begin(),
                     variables.end(),
                     var) == variables.end()) {
          variables.push_back(var);
          ++num_variable;
        }
      }
    }
    else {
      BranchInst *binst;

      switch(inst->getOpcode()) {
      case RET:        break;
      case BR:
        binst = dyn_cast<BranchInst>(inst);
        value = binst->getCondition();
        type = value->getType();

        if(type->isPointerTy())
          type = type->getPointerElementType();

        var.set_name(value->getName());
        var.set_asm_name(value->getName());
        var.set_bit_width(type->getPrimitiveSizeInBits());
        var.set_type(Variable::REG);

        var.set_input(false);
        var.set_output(false);

        if(std::find(variables.begin(),
                     variables.end(),
                     var) == variables.end()) {
          variables.push_back(var);
          ++num_variable;
        }
        break;

      case STORE:
        for(int i=0; i<2; ++i) {
          value = inst->getOperand(i);
          type = value->getType();

          if(type->isPointerTy())
            type = type->getPointerElementType();

          var.set_name(value->getName());
          var.set_asm_name(value->getName());
          var.set_bit_width(type->getPrimitiveSizeInBits());
          var.set_type(Variable::REG);
          var.set_input(false);
          var.set_output(false);

          if(std::find(variables.begin(),
                       variables.end(),
                       var) == variables.end()) {
            variables.push_back(var);
            ++num_variable;
          }
        }
        break;

      default:
        errs() << "ERROR:" << inst->getOpcodeName() << " "
               << inst->getOpcode() << " 未定義のオペランド\n";
        break;
      }
    }
  }

  // reg名のサニタイジング
  std::list<Variable>::iterator v_it  = variables.begin();
  std::list<Variable>::iterator v_end = variables.end();

  for(; v_it != v_end; ++v_it) {
    if(!v_it->is_input() && !v_it->is_output())
      v_it->sanitize_name("reg_");
  }

  // state管理用regの追加 --------------------
  var.set_name(PREV_STATE_STR);
  var.set_asm_name(PREV_STATE_STR);
  var.set_bit_width(state_bit_width());
  var.set_type(Variable::REG);
  var.set_input(false);
  var.set_output(false);

  variables.push_back(var);
  ++num_variable;

  var.set_name(CUR_STATE_STR);
  var.set_asm_name(CUR_STATE_STR);

  variables.push_back(var);
  ++num_variable;

  return num_variable;
}

int FortRock::grub_labels(const Module::FunctionListType::iterator &funct) {
  Function::BasicBlockListType::iterator bb_it = funct->begin();
  Function::BasicBlockListType::iterator bb_end = funct->end();
  Label label;
  int num_label = 0;

  // labelの追加
  for(; bb_it!=bb_end; ++bb_it) {
    label.set_name(bb_it->getName());
    label.set_asm_name(bb_it->getName());
    if(std::find(labels.begin(),
                 labels.end(),
                 label) == labels.end()) {
      labels.push_back(label);
      ++num_label;
    }
  }

  // todo: labelのparameter追加

  // label名に対応する値の定義 --------------------
  std::list<Label>::iterator l_it  = labels.begin();
  std::list<Label>::iterator l_end = labels.end();

  for(int i=0; l_it != l_end; ++l_it, ++i) {
    std::string label_name;
    label_name = std::to_string(state_bit_width()) + "'d" + std::to_string(i);
    l_it->set_name(label_name);
  }

  return num_label;
}

/**
 * moduleのI/Oやレジスタ，ワイヤの宣言の出力
 * @return input, output, reg, wireの宣言部
 */
std::string FortRock::print_varlist(void) {
  std::string ret_str;
  std::list<Variable>::iterator it;
  std::list<Variable>::iterator end;

  unsigned now_width;
  bool printed;

  ret_str += indent() + "input clk, res;\n";
  ret_str += indent() + "output fin;\n\n";

  ret_str +=  indent() + "reg fin;\n\n";

  // input
  {
    variables.sort();
    it = variables.begin();
    end = variables.end();

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

  ret_str = PREV_STATE_STR;
  ret_str += " = ";
  ret_str += CUR_STATE_STR;
  ret_str += ";\n";

  ret_str += indent();
  ret_str += CUR_STATE_STR;
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

  ret_str = PREV_STATE_STR;
  ret_str += " = ";
  ret_str += CUR_STATE_STR;
  ret_str += ";\n";
  ret_str += indent();
  ret_str += CUR_STATE_STR;
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
  ret_str += PREV_STATE_STR;
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
  ret_str += indent() + PREV_STATE_STR + " = "
    + std::to_string(state_bit_width()) + "'b0;\n";
  ret_str += indent() + CUR_STATE_STR + " = "
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

  ret_str += indent() + "case (" + CUR_STATE_STR + ")\n";
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

/**
 * スペース文字の出力
 * @arg num_space 出力するスペースの文字数
 * @return スペースの文字列
 */
std::string FortRock::print_spaces(const int num_space) {
  std::string ret_str = "";

  for(int i=0; i<num_space; ++i)
    ret_str += ' ';

  return ret_str;
}

/**
 * I/Oのビット幅を出力する
 * @arg var reg, wire, input, outputのいずれか
 * @return スペースで整形されたビット幅を含む文字列
 */
std::string FortRock::print_bit_width(Variable var) {
  std::string ret_str;

  if( var.get_bit_width() == 1 ) return print_spaces(10);

  ret_str =  "["
    + std::to_string(var.get_bit_width() - 1)
    + ":0]";

  return ret_str + print_spaces(10 - ret_str.length());
}

char FortRock::ID = 0;

// ==================================================
// External Interface declaration
// ==================================================

static RegisterPass<FortRock> X("fortrock", "Fortran to Verilog", false, false);
