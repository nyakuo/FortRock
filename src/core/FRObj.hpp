#ifndef __FROBJ_H
#define __FROBJ_H

class FRObj {
protected:
  std::string name;
  std::string asm_name; // アセンブリ上での名前

public:
  FRObj(const std::string name, const std::string asm_name) : name(name), asm_name(asm_name) {}

  void sanitize_name(const std::string prefix) {
    int at;
    while((at = name.find('.', 0)) != std::string::npos)
      name.replace(at, 1, "_");

    name = prefix + name;
  }

  void set_name(const std::string name)         { this->name = name; }
  void set_asm_name(const std::string asm_name) { this->asm_name = asm_name; }

  std::string get_name(void)        { return name; }
  std::string get_asm_name(void)    { return asm_name; }

  std::string to_string(void) {
    std::string ret_str = "name: " + name;
    ret_str += " asm_name: " + asm_name;
    return ret_str;
  }

  bool operator==(const FRObj &b) { // for find
    return this->asm_name == b.asm_name;
  }
};
#endif
