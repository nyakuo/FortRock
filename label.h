/**
 * @class Label
 * LLVMのLabelを表すクラス
 */
class Label {
  std::string name;
  std::string asm_name;

public:
 Label() : name("undefined"),
            asm_name("asm_undefined") {}

  // setter
  void set_name(const std::string name)         { this->name = name; }
  void set_asm_name(const std::string asm_name) { this->asm_name = asm_name; }

  // getter
  std::string get_name(void)     { return name; }
  std::string get_asm_name(void) { return asm_name; }

  std::string to_string(void) {
    std::string ret_str = "name: " + name;
    ret_str += " asm_name: " + asm_name;
    return ret_str;
  }

  bool operator==(const Label &b) { // for find
    return this->asm_name == b.asm_name;
  }
};
