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
#include <iostream>
#include <list>
using namespace llvm;

namespace {

  enum variable_type {
    reg = 0,
    wire,
    parameter,
  };

  class Variable {
    variable_type type;

    bool _is_input; // input output が2つあるのは Verilog の inout に対応するため
    bool _is_output;

    std::string name;

    unsigned bit_width;

  public:
    Variable(void) : _is_input(false), _is_output(false),
                     type(reg), name("0null"), bit_width(0) {}

    void set_name(const std::string name)    { this->name = name; }
    void set_bit_width(const unsigned width) { this->bit_width = width; }
    void set_type(const variable_type type)  { this->type = type; }
    bool set_input(bool is_input)            { this->_is_input = is_input; }
    bool set_output(bool is_output)          { this->_is_output = is_output; }

    std::string get_name(void)        { return name;}
    unsigned get_bit_width(void)      { return bit_width; }
    variable_type get_type(void)      { return type; }
    bool is_input(void)               { return _is_input; }
    bool is_output(void)               { return _is_output; }

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

  private:
    std::string type_to_string(variable_type type) {
      switch(type) {
      case reg:       return "reg";
      case wire:      return "wire";
      case parameter: return "parameter";
      }
      
      return "null";
    }
  };

  class FortRock : public ModulePass  {
    unsigned            indent_level;
    const unsigned      indent_width;
    std::list<Variable> variables;

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

    std::string print_header(const Module &m);
    std::string print_arguments(Module::FunctionListType::iterator it);
    std::string print_varlist(void);
    std::string print_always(Module::FunctionListType::iterator it);
    std::string print_spaces(const int num_space);
    std::string print_bit_width(Variable var);
  };

}

std::string FortRock::print_header(const Module &m) {
  return  "// " + m.getModuleIdentifier() + ".v\n"
         + "module " +  m.getModuleIdentifier()
         + "(clk, res, fin";
}

std::string FortRock::print_arguments(Module::FunctionListType::iterator it) {
  std::string ret_str = "";
  Type *type = NULL;
  Variable var;
  int num;

  Function::arg_iterator arg_it  = it->arg_begin();
  Function::arg_iterator arg_end = it->arg_end();

  for(num = 1; // 最後の引数(返り値)を知るため
      arg_it != arg_end;
      ++num, ++arg_it) {
    type = arg_it->getType();
    
    if(type->isPointerTy()) // ポインタならポインタの型を取得
      type = type->getPointerElementType();
    
    if(type->isIntegerTy()) {

      var.set_name(arg_it->getName());
      var.set_bit_width(type->getPrimitiveSizeInBits());
      var.set_type(reg);

      var.set_input(true);

      if(num == it->arg_size()) { // 最後の引数は返り値
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

  for(;it != end; ++it) {
    // Arguments
    if(it->getArgumentList().size() != 0)
      errs() << print_arguments(it);

    std::string always = print_always(it);

    // Variables
    errs() << print_varlist();

    // always
    errs() << always;

  } // for Functions

  errs() << "endmodule\n";

  // -------------------- debug --------------------
  std::list<Variable>::iterator debit = variables.begin();
  std::list<Variable>::iterator debend = variables.end();
  for(;debit != debend; ++debit)
    errs() << debit->to_string() << "\n";

  return false;
}

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
      if(reg == it->get_type()) { // reg の場合
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

std::string FortRock::print_always(Module::FunctionListType::iterator it) {
  std::string ret_str = indent() + "always @(posedge clk)\n";  indent_right();
  ret_str += indent() + "begin\n";  indent_right();
  ret_str += indent() + "if(res)\n";  indent_right();
  ret_str += indent() + "begin\n";  indent_right();

  indent_left();
  indent_left();
  indent_left();
  indent_left();

  return ret_str + indent() + "end // always\n";
}

std::string FortRock::print_spaces(const int num_space) {
  std::string ret_str = "";

  for(int i=0; i<num_space; ++i)
    ret_str += ' ';

  return ret_str;
}

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
