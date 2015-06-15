// -*- C++ -*-
#ifndef __VARIABLE_H
#define __VARIABLE_H
#include "FRObj.h"

/**
 * @class Variable
 * LLVMの変数を管理するクラス
 */
class Variable : public FRObj {
 public:
    enum variable_type {
    REG = 0,
    WIRE,
    PARAMETER,
  };

 private:
  variable_type type;

  bool _is_input; // input output が2つあるのは Verilog の inout に対応するため
  bool _is_output;

  unsigned bit_width;

public:
  Variable(void) : _is_input(false), _is_output(false),
                   type(REG),
                   bit_width(0),
                   FRObj("undefined", "asm_undefined") {}

  // setter
  void set_bit_width(const unsigned width) { this->bit_width = width; }
  void set_type(const variable_type type)  { this->type = type; }
  bool set_input(bool is_input)            { this->_is_input = is_input; }
  bool set_output(bool is_output)          { this->_is_output = is_output; }

  // getter
  unsigned get_bit_width(void)      { return bit_width; }
  variable_type get_type(void)      { return type; }

  bool is_input(void)               { return _is_input; }
  bool is_output(void)              { return _is_output; }

  std::string to_string(void) {
    std::string ret_str;

    ret_str += "name: " + name;
    ret_str += "\nasm_name: " + asm_name;
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
    case REG:       return "reg";
    case WIRE:      return "wire";
    case PARAMETER: return "parameter";
    }
      
    return "null";
  }
};
#endif
