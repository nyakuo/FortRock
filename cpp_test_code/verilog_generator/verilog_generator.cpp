/**
   テストプログラム
   命令列のDFG化に関するクラスの実装
   およびVerilogコードの出力機能に関する検証
 */

#include <iostream>
#include <fstream>
#include <list>

#include "CDFG_Element.h"
#include "CDFG_Node.h"

class CModuleGenerator {
private:
  const std::string OUTPUT_FILE = "output.v";
  const unsigned INDENT_WIDTH;

  unsigned __indent_level;
  std::string __filename;
  std::string __module_name;
  std::ofstream __ofs;

  std::list< std::shared_ptr< CDFG_Node > > __input_list;
  std::list< std::shared_ptr< CDFG_Node > > __output_list;
  std::list< std::shared_ptr< CDFG_Element > > __DFG;

  std::string __print_spaces(const int num_space);
  inline std::string __indent(void) { return this->__print_spaces(this->__indent_level * this->INDENT_WIDTH); }
  inline void __indent_left(void) { if(this->__indent_level > 0) --__indent_level; }
  inline void __indent_right(void) { ++this->__indent_level; }

  void __generate_header(void);
  void __generate_define(void);
  void __generate_parameter(void);
  void __generate_assign(void);
  void __generate_calculator(void);
  void __generate_always(void);
  void __generate_footer(void);

public:
  int generate(void);

  CModuleGenerator(const std::string & filename);
};

CModuleGenerator::CModuleGenerator(const std::string & filename)
  : __filename(filename),
    __indent_level(0),
    INDENT_WIDTH(3) {
  // テスト用初期化処理
  {
    this->__module_name = "test";

    // ノード確保
    // 入力
    std::shared_ptr< CDFG_Node > a = std::shared_ptr< CDFG_Node >(new CDFG_Node("input_a",
                                                                                8,
                                                                                true,
                                                                                CDFG_Node::eNode::IN));
    std::shared_ptr< CDFG_Node > b = std::shared_ptr< CDFG_Node >(new CDFG_Node("input_b",
                                                                                8,
                                                                                true,
                                                                                CDFG_Node::eNode::IN));

    // 出力
    std::shared_ptr< CDFG_Node > out = std::shared_ptr< CDFG_Node > (new CDFG_Node("out",
                                                                                   8,
                                                                                   true,
                                                                                   CDFG_Node::eNode::OUT));

    // 演算器
    std::shared_ptr< CDFG_Node > add = std::shared_ptr< CDFG_Node >(new CDFG_Node("my_add",
                                                                                  8,
                                                                                  true,
                                                                                  CDFG_Node::eNode::ADD));

    std::shared_ptr< CDFG_Node > minus = std::shared_ptr< CDFG_Node >(new CDFG_Node("my_minus",
                                                                                    8,
                                                                                    true,
                                                                                    CDFG_Node::eNode::MINUS));
    std::shared_ptr< CDFG_Node > p3 = std::shared_ptr< CDFG_Node >(new CDFG_Node("3",
                                                                                 8,
                                                                                 true,
                                                                                 CDFG_Node::eNode::PARAM));

    // テンプレートレジスタ
    std::shared_ptr< CDFG_Node > t1 = std::shared_ptr< CDFG_Node >(new CDFG_Node("t1",
                                                                                 8,
                                                                                 true,
                                                                                 CDFG_Node::eNode::REG));

    // DFG
    // 入出力
    this->__input_list.push_back(a);
    this->__input_list.push_back(b);
    this->__output_list.push_back(out);

    // t1 = input_a + input_b
    std::shared_ptr< CDFG_Element > elem = std::shared_ptr< CDFG_Element >(new CDFG_Element());

    elem->set_input_size(2);
    elem->set_output_size(1);
    elem->set_ope(add);
    elem->set_input(a, 0);
    elem->set_input(b, 1);
    elem->set_output(t1, 0);
    elem->set_state(0);

    this->__DFG.push_back(elem);
    elem.reset();
    elem = std::shared_ptr< CDFG_Element >(new CDFG_Element());

    elem->set_input_size(2);
    elem->set_output_size(1);
    elem->set_ope(minus);
    elem->set_input(t1, 0);
    elem->set_input(p3, 1);
    elem->set_output(out, 0);
    elem->set_state(1);

    this->__DFG.push_back(elem);
    elem.reset();
  }
}

std::string CModuleGenerator::__print_spaces(const int num_space) {
  std::string ret_str = "";

  for(int i=0; i<num_space; ++i)
    ret_str += ' ';

  return ret_str;
}

int CModuleGenerator::generate(void) {
  //  this->__ofs.open(this->__filename, std::ios::out);
  this->__ofs.open(this->OUTPUT_FILE, std::ios::out);
  if (!this->__ofs)
    return -1;

  std::cout.rdbuf(this->__ofs.rdbuf());
  std::cout << "before\n";
  this->__generate_header();

  // this->__generate_define();
  // this->__generate_parameter();
  // this->__generate_assign();
  // this->__generate_calculator();
  // this->__generate_always();

  this->__generate_footer();

  this->__ofs.close();
  return 0;
}

void CModuleGenerator::__generate_define(void) {
}

void CModuleGenerator::__generate_header(void) {
  this->__ofs << "`default_nettype none\n\n"
              << "module " << this->__module_name << std::endl;

  this->__indent_right();

  this->__ofs << this->__indent() << "(\n";
  this->__ofs << this->__indent() << "input wire i_clk,\n"
              << this->__indent() << "input wire i_res_p,\n"
              << this->__indent() << "input wire i_req_p,\n"
              << this->__indent() << "input wire i_ce_p,\n"
              << this->__indent() << "output wire o_fin_p,\n";

  // 入力信号の出力
  std::list< std::shared_ptr< CDFG_Node > >::iterator ite = this->__input_list.begin();
  std::list< std::shared_ptr< CDFG_Node > >::iterator end = this->__input_list.end();
  bool someIO = this->__input_list.size() != 0 && this->__output_list.size() != 0;
  while(ite != end) {
    this->__ofs << this->__indent() << "input wire ";
    if ((*ite)->get_bit_width() > 1) {
      this->__ofs << "[" << (*ite)->get_bit_width() - 1 << ":0] ";
    }
    this->__ofs << (*ite)->get_name();
    ++ite;

    if(ite == end) {
      if (someIO)
        this->__ofs << ",\n";
    }
    else
      this->__ofs << ",\n";
  }


  // 出力信号の出力
  ite = this->__output_list.begin();
  end = this->__output_list.end();
  while(ite != end) {
    this->__ofs << this->__indent() << "input wire ";
    if ((*ite)->get_bit_width() > 1) {
      this->__ofs << "[" << (*ite)->get_bit_width() - 1 << ":0] ";
    }
    this->__ofs << (*ite)->get_name();
    ++ite;
    if(ite != end)
      this->__ofs << ",";
    this->__ofs << "\n";
  }


  this->__ofs << this->__indent() << ");" << std::endl;
}

void CModuleGenerator::__generate_footer(void) {
  this->__ofs << "endmodule\n\n"
              << "`default_nettype wire" << std::endl;
}


int main(int argc, char **argv) {
  const std::string INPUT_FILE = "test_input.xml";

  CModuleGenerator generator(INPUT_FILE);
  generator.generate();

  return 0;
}
