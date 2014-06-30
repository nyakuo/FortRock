// -*- C++ -*-
/**
 * @class Variable
 * LLVMの変数を管理するクラス
 */
class Variable {
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

  std::string name;
  std::string asm_name; // アセンブリ上での名前

  unsigned bit_width;

public:
  Variable(void) : _is_input(false), _is_output(false),
                   type(REG), name("0null"),
                   asm_name("asm_null"), bit_width(0) {}

  // setter
  void set_name(const std::string name)    { this->name = name; }
  void set_asm_name(const std::string asm_name) { this->asm_name = asm_name; }
  void set_bit_width(const unsigned width) { this->bit_width = width; }
  void set_type(const variable_type type)  { this->type = type; }
  bool set_input(bool is_input)            { this->_is_input = is_input; }
  bool set_output(bool is_output)          { this->_is_output = is_output; }

  // getter
  std::string get_name(void)        { return name; }
  std::string get_asm_name(void)    { return asm_name; }
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
    return this->asm_name == b.asm_name;
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
