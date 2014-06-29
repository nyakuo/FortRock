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
using namespace llvm;

namespace {

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

  enum variable_type {
    REG = 0,
    WIRE,
    PARAMETER,
  };

  class Label {
    std::string name;
    unsigned num;

  public:
    Label() : name("undefined"), num(0) {}

    // setter
    void set_name(const std::string name) { this->name = name; }
    void set_num(const unsigned num) { this->num = num; }

    // getter
    std::string get_name(void) { return name; }
    unsigned get_num(void) { return num; }

    std::string to_string(void) {
      std::string ret_str = "name: " + name;
      ret_str += " num: " + std::to_string(num);

      return ret_str;
    }

    bool operator==(const Label &b) { // for find
      return this->name == b.name;
    }
  };

  class Variable {
    variable_type type;

    bool _is_input; // input output が2つあるのは Verilog の inout に対応するため
    bool _is_output;

    std::string name;
    std::string asm_name; // アセンブリ上での名前

    unsigned bit_width;

  public:
    Variable(void) : _is_input(false), _is_output(false),
                     type(REG), name("0null"), bit_width(0) {}

    // setter
    void set_name(const std::string name)    { this->name = name; }
    void set_bit_width(const unsigned width) { this->bit_width = width; }
    void set_type(const variable_type type)  { this->type = type; }
    bool set_input(bool is_input)            { this->_is_input = is_input; }
    bool set_output(bool is_output)          { this->_is_output = is_output; }

    // getter
    std::string get_name(void)        { return name;}
    unsigned get_bit_width(void)      { return bit_width; }
    variable_type get_type(void)      { return type; }

    bool is_input(void)               { return _is_input; }
    bool is_output(void)              { return _is_output; }

    std::string to_string(void) {
      std::string ret_str;

      ret_str += "name: " + name;
      ret_str += "\nbit_width: ";
      ret_str += std::to_string(bit_width);
      ret_str += "\nis_input: ";
      ret_str += (_is_input ? "true" : "false");
      ret_str += "\nis_output: ";
      ret_str += (_is_output ? "true" : "false");
      ret_str += "\ntype: ";
      ret_str += type_to_string(type);
      
      return ret_str + "\n";
    }

    bool operator<(const Variable & b) { // for sort
      return this->bit_width < b.bit_width;
    }

    bool operator==(const Variable &b) { // for cmp
      return this->name == b.name;
    }

  private:
    std::string type_to_string(variable_type type) {
      switch(type) {
      case REG:       return "reg";
      case WIRE:      return "wire";
      case PARAMETER: return "parameter";
      }
      
      return "null";
    }
  };

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

    std::string print_header(const Module &M);
    std::string print_arguments(const Module::FunctionListType::iterator &funct);
    int grub_variables(const Module::FunctionListType::iterator &funct);
    std::string print_varlist(void);
    std::string print_always(const Module::FunctionListType::iterator &funct);
    std::string print_spaces(const int num_space);
    std::string print_bit_width(Variable var);
    std::string print_instruction(const Instruction * inst);

    // print instruction functions
    std::string print_LOAD(const Instruction * inst);
    std::string print_ICMP(const Instruction * inst);
    std::string print_PHI(const Instruction * inst);
    std::string print_SELECT(const Instruction * inst);
    std::string print_SREM(const Instruction * inst);
    std::string print_MUL(const Instruction * inst);
    std::string print_SDIV(const Instruction * inst);
  };
}

/**
 * moduleのheader(固定であるclk, res, fin)を出力する
 */
std::string FortRock::print_header(const Module &M) {
  return  "// " + M.getModuleIdentifier() + ".v\n"
         + "module " +  M.getModuleIdentifier()
         + "(clk, res, fin";
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
      var.set_bit_width(type->getPrimitiveSizeInBits());
      var.set_type(REG);

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
      ret_str += "ERROR: can not solb the arg type\n";
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

  grub_variables(it);

  errs() << print_varlist();
  errs() << print_always(it);

  errs() << "endmodule\n";

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

        var.set_name(value->getName());
        var.set_bit_width(type->getPrimitiveSizeInBits());
        var.set_type(REG);

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
      BasicBlock *bb;
      BranchInst *binst;
      Label label;
      switch(inst->getOpcode()) {
      case RET:        break;
      case BR:
        binst = dyn_cast<BranchInst>(inst);
        value = binst->getCondition();
        type = value->getType();

        if(type->isPointerTy())
          type = type->getPointerElementType();

        var.set_name(value->getName());
        var.set_bit_width(type->getPrimitiveSizeInBits());
        var.set_type(REG);

        var.set_input(false);
        var.set_output(false);

        if(std::find(variables.begin(),
                     variables.end(),
                     var) == variables.end()) {
          variables.push_back(var);
          ++num_variable;
        }

        for(int i=0; i<2; ++i) {
          bb = binst->getSuccessor(i);
          
          label.set_name(bb->getName());
          label.set_num(labels.size());

          if(std::find(labels.begin(),
                       labels.end(),
                       label) == labels.end()) {
            labels.push_back(label);
          }
        }
        break;

      case STORE:
        for(int i=0; i<2; ++i) {
          value = inst->getOperand(i);
          type = value->getType();

          if(type->isPointerTy())
            type = type->getPointerElementType();

          var.set_name(value->getName());
          var.set_bit_width(type->getPrimitiveSizeInBits());
          var.set_type(REG);

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
  return num_variable;
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
      if(REG == it->get_type()) { // reg の場合
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


std::string FortRock::print_LOAD(const Instruction * inst) {
  std::string ret_str;
  Value * operand;

  ret_str = inst->getName();
  operand = inst->getOperand(0);

  //  ret_str += " " + inst->getOperand(0)->getName();

  return ret_str;
}
std::string FortRock::print_ICMP(const Instruction * inst) {
  return "none";
}
std::string FortRock::print_PHI(const Instruction * inst) {
  return "NONE";
}
std::string FortRock::print_SELECT(const Instruction * inst) {
  return "none";
}
std::string FortRock::print_SREM(const Instruction * inst) {
  return "none";
}
std::string FortRock::print_MUL(const Instruction * inst) {
  return "none";
}
std::string FortRock::print_SDIV(const Instruction * inst) {
  return "none";
}

/**
 * 各命令の出力関数を呼び出す
 */
std::string FortRock::print_instruction(const Instruction * inst) {
  switch(inst->getOpcode()) {
  case LOAD:   return  print_LOAD(inst); 
  case ICMP:   return  print_ICMP(inst); 
  case PHI:    return  print_PHI(inst); 
  case SELECT: return  print_SELECT(inst); 
  case SREM:   return  print_SREM(inst); 
  case MUL:    return  print_MUL(inst); 
  case SDIV:   return  print_SDIV(inst); 
  default:
    errs() <<  "ERROR:" << inst->getOpcodeName() << " 未定義のオペランド\n";
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
  ret_str += indent() + "if(res)\n";  indent_right();
  ret_str += indent() + "begin\n";  indent_right();
  ret_str += indent() + "fin = 1'b0;\n";

  inst_iterator it  = inst_begin(*funct);
  inst_iterator end = inst_end(*funct);

  for(; it != end; ++it)
    insts.push_back(&*it);

  // 命令文の出力
  Instruction *inst = NULL;
  while(!insts.empty()) {
    inst = insts.front();
    insts.pop_front();

    if(!inst->use_empty()) {
      errs() << print_instruction(inst);
    }
  }

  indent_left();

  // todo: initialize

  ret_str += indent() + "end // if res\n";  indent_left();

  ret_str += indent() + "else if(fin == 1'b0)\n";  indent_right();
  ret_str += indent() + "begin\n"; indent_right();
  
  // todo: some states

  indent_left();
  ret_str += indent() + "end // if fin\n"; indent_left();

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
