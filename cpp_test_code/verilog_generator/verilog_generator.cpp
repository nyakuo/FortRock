/**
   テストプログラム
   命令列のDFG化に関するクラスの実装
   およびVerilogコードの出力機能に関する検証
 */

#include <iostream>
#include <fstream>
#include <list>

#include "CDFG_Element.h"
#include "CDFG_Connect.h"
using namespace std;

class CInput : public CDFG_Element {
public:
};

class CRegister : public CDFG_Element {
public:

};

class CWire : public CDFG_Element {
public:

};

class COperator : public CDFG_Element {

public:
  enum EOPE_TYPE {
    OPE_ADD = 0,
    OPE_MINUS,
    OPE_MUL,
    OPE_DIV,
  };

  EOPE_TYPE ope_type;
  unsigned ope_num;

};

class CModuleGenerator {
private:
  const string OUTPUT_FILE = "output.v";
  const unsigned INDENT_WIDTH;

  unsigned __indent_level;
  string __filename;
  string __module_name;
  ofstream __ofs;

  list<CDFG_Element> __input_list;
  list<CDFG_Element> __output_list;
  list<CDFG_Element> __DFG;

  string __print_spaces(const int num_space);
  inline string __indent(void) { return this->__print_spaces(this->__indent_level * this->INDENT_WIDTH); }
  inline void __indent_left(void) { if(this->__indent_level > 0) --__indent_level; }
  inline void __indent_right(void) { ++this->__indent_level; }

  // int __write_line(const string & line);
  void __generate_header(void);
  void __generate_footer(void);

public:
  int generate(void);

  CModuleGenerator(const string & filename);
};

CModuleGenerator::CModuleGenerator(const string & filename)
  : __filename(filename),
    __indent_level(0),
    INDENT_WIDTH(3) {
  // テスト用初期化
  this->__module_name = "auto";

  //  this->__input_list.append(CDFG_Element(
}

string CModuleGenerator::__print_spaces(const int num_space) {
  std::string ret_str = "";

  for(int i=0; i<num_space; ++i)
    ret_str += ' ';

  return ret_str;
}

// int CModuleGenerator::__write_line(const string & line) {
//   this->__ofs << this->__indent();
//   this->__ofs << line << endl;

//   return 0;
// }

void CModuleGenerator::__generate_header(void) {
  this->__ofs << "`default_nettype none" << endl << endl
              << "module " << this->__module_name << endl;

  this->__indent_right();

  this->__ofs << this->__indent() << "(" << endl;
  this->__ofs << this->__indent() << "input wire i_clk," << endl
              << this->__indent() << "input wire i_res_p," << endl
              << this->__indent() << "input wire i_req_p," << endl
              << this->__indent() << "input wire i_ce_p," << endl
              << this->__indent() << "output wire o_fin_p," << endl;

  this->__ofs << this->__indent() << ");" << endl;
}

void CModuleGenerator::__generate_footer(void) {
  this->__ofs << "endmodule" << endl << endl
              << "`default_nettype wire" << endl;
}

int CModuleGenerator::generate(void) {
  //  this->__ofs.open(this->__filename, ios::out);
  this->__ofs.open(this->OUTPUT_FILE, ios::out);
  if (!this->__ofs)
    return -1;

  this->__generate_header();

  this->__generate_footer();

  this->__ofs.close();
  return 0;
}

int main(int argc, char **argv) {
  const string INPUT_FILE = "test_input.xml";

  CModuleGenerator generator(INPUT_FILE);
  generator.generate();

  return 0;
}
