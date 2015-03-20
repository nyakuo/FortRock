/**
   テストプログラム
   命令列のDFG化に関するクラスの実装
   およびVerilogコードの出力機能に関する検証
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <array>

#include "CDFG_Element.h"
#include "CDFG_Node.h"

class CModuleGenerator {
private:
  const std::string OUTPUT_FILE = "output.v";
  const unsigned INDENT_WIDTH;

  unsigned _indent_level;
  std::string _filename;
  std::string _module_name;
  std::ofstream _ofs;

  std::list< std::shared_ptr< CDFG_Node > > _input_list;
  std::list< std::shared_ptr< CDFG_Node > > _output_list;
  std::list< std::shared_ptr< CDFG_Element > > _DFG;
  std::list< std::shared_ptr< CDFG_Node > > _node_list;

  std::string _print_spaces(const int num_space);
  inline std::string _indent(void) { return this->_print_spaces(this->_indent_level * this->INDENT_WIDTH); }
  inline void _indent_left(void) { if(this->_indent_level > 0) --_indent_level; }
  inline void _indent_right(void) { ++this->_indent_level; }

  void _generate_header(void);
  void _generate_define(void);
  void _generate_assign(void);
  void _generate_calculator(void);
  void _generate_always(void);
  void _generate_footer(void);

public:
  int generate(void);

  CModuleGenerator(const std::string & filename);
};

CModuleGenerator::CModuleGenerator(const std::string & filename)
  : _filename(filename),
    _indent_level(0),
    INDENT_WIDTH(3) {
  // テスト用初期化処理
  {
    this->_module_name = "test";

    // ノード確保
    // 入力
    auto a = std::make_shared< CDFG_Node >(CDFG_Node("i_a",
                                                     8,
                                                     true,
                                                     CDFG_Node::eNode::IN));
    auto b = std::make_shared< CDFG_Node >(CDFG_Node("i_b",
                                                     8,
                                                     true,
                                                     CDFG_Node::eNode::IN));

    // 出力
    auto out = std::make_shared< CDFG_Node > (CDFG_Node("o_out",
                                                        8,
                                                        true,
                                                        CDFG_Node::eNode::OUT));

    // 演算器
    auto add = std::make_shared< CDFG_Node >(CDFG_Node("my_add",
                                                       8,
                                                       true,
                                                       CDFG_Node::eNode::ADD));

    auto minus = std::make_shared< CDFG_Node >(CDFG_Node("my_minus",
                                                         8,
                                                         true,
                                                         CDFG_Node::eNode::MINUS));

    // 定数
    auto p3 = std::make_shared< CDFG_Node >(CDFG_Node("p3",
                                                      8,
                                                      true,
                                                      CDFG_Node::eNode::PARAM));

    // テンプレートレジスタ
    auto t1 = std::make_shared< CDFG_Node >(CDFG_Node("t1",
                                                      8,
                                                      true,
                                                      CDFG_Node::eNode::REG));

    // 資源登録
    // 入出力
    // TODO: this->_input_list & this->_output_list は不必要?
    this->_input_list.emplace_back(a);
    this->_input_list.emplace_back(b);
    this->_output_list.emplace_back(out);
    // 回路内
    this->_node_list.emplace_back(a);
    this->_node_list.emplace_back(b);
    this->_node_list.emplace_back(out);
    this->_node_list.emplace_back(add);
    this->_node_list.emplace_back(minus);
    this->_node_list.emplace_back(p3);
    this->_node_list.emplace_back(t1);

    // DFG
    // t1 = input_a + input_b
    auto elem = std::make_shared< CDFG_Element >(CDFG_Element());

    elem->set_input_size(2);
    elem->set_output_size(1);
    elem->set_ope(add);
    elem->set_input(a, 0);
    elem->set_input(b, 1);
    elem->set_output(t1, 0);
    elem->set_state(0);

    this->_DFG.emplace_back(elem);
    elem.reset();
    elem = std::make_shared< CDFG_Element >(CDFG_Element());

    elem->set_input_size(2);
    elem->set_output_size(1);
    elem->set_ope(minus);
    elem->set_input(t1, 0);
    elem->set_input(p3, 1);
    elem->set_output(out, 0);
    elem->set_state(1);

    this->_DFG.emplace_back(elem);
    elem.reset();
  }
}

std::string CModuleGenerator::_print_spaces(const int num_space) {
  auto ret_str = "";

  for(int i=0; i<num_space; ++i)
    ret_str += ' ';

  return ret_str;
}

int CModuleGenerator::generate(void) {
  //  this->_ofs.open(this->_filename, std::ios::out);
  this->_ofs.open(this->OUTPUT_FILE, std::ios::out);
  if (!this->_ofs)
    return -1;

  std::cout.rdbuf(this->_ofs.rdbuf());
  std::cout << "before\n";
  this->_generate_header();
  this->_generate_define();
  // this->_generate_assign();
  // this->_generate_calculator();
  // this->_generate_always();

  this->_generate_footer();

  this->_ofs.close();
  return 0;
}
void CModuleGenerator::_generate_header(void) {
  this->_ofs << "`default_nettype none\n\n"
              << "module " << this->_module_name << std::endl;

  this->_indent_right();

  this->_ofs << this->_indent() << "(\n";
  this->_ofs << this->_indent() << "input wire i_clk,\n"
              << this->_indent() << "input wire i_res_p,\n"
              << this->_indent() << "input wire i_req_p,\n"
              << this->_indent() << "input wire i_ce_p,\n"
              << this->_indent() << "output wire o_fin_p,\n";

  // 入力信号の出力
  std::list< std::shared_ptr< CDFG_Node > >::iterator ite = this->_input_list.begin();
  std::list< std::shared_ptr< CDFG_Node > >::iterator end = this->_input_list.end();
  bool someIO = this->_input_list.size() != 0 && this->_output_list.size() != 0;
  while(ite != end) {
    this->_ofs << this->_indent() << "input wire ";

    if((*ite)->get_is_signed())
      this->_ofs << "signed ";

    if ((*ite)->get_bit_width() > 1) {
      this->_ofs << "[" << (*ite)->get_bit_width() - 1 << ":0] ";
    }
    this->_ofs << (*ite)->get_name();
    ++ite;

    if(ite == end) {
      if (someIO)
        this->_ofs << ",\n";
    }
    else
      this->_ofs << ",\n";
  }

  // 出力信号の出力
  ite = this->_output_list.begin();
  end = this->_output_list.end();
  while(ite != end) {
    this->_ofs << this->_indent() << "output reg ";
    if ((*ite)->get_bit_width() > 1) {
      this->_ofs << "[" << (*ite)->get_bit_width() - 1 << ":0] ";
    }
    this->_ofs << (*ite)->get_name();
    ++ite;
    if(ite != end)
      this->_ofs << ",";
    this->_ofs << "\n";
  }


  this->_ofs << this->_indent() << ");" << std::endl;
}

void CModuleGenerator::_generate_define(void) {
  unsigned type;
  const auto reg = 0;
  const auto wire = 1;
  const auto param = 2;
  const auto none = 3;
  std::array<std::stringstream, 3> streams;
  std::string types[3] = {"reg", "wire", "parameter"};

  for (auto &node : this->_node_list) {
    switch(node->get_type()) {
    case CDFG_Node::eNode::REG:
      type = reg;
      break;

    case CDFG_Node::eNode::WIRE:
      type = wire;
      break;

    case CDFG_Node::eNode::PARAM:
      type = param;
      break;

    default:
      type = none;
      break;
    }

    if (type != none) {
      streams[type] << this->_indent() << types[type] << ' ';

      if (node->get_is_signed()) {
        streams[type] << "signed ";
      }

      if (node->get_bit_width() > 1) {
        streams[type] << "[" << node->get_bit_width() - 1 << ":0] ";
      }

      streams[type] << node->get_name() << ";\n";
    }
  }

  // todo: state信号の出力
  // fin, state信号
  streams[reg] << this->_indent() << "reg r_sys_fin;" << std::endl;

  for (auto &s : streams) {
    if (s.rdbuf()->in_avail() != 0) {
      this->_ofs << s.str() << std::endl;
    }
  }
  // todo: 演算器のI/Oポートの定義
}

void CModuleGenerator::_generate_footer(void) {
  this->_ofs << "endmodule\n\n"
              << "`default_nettype wire" << std::endl;
}


int main(int argc, char **argv) {
  const auto INPUT_FILE = "test_input.xml";

  CModuleGenerator generator(INPUT_FILE);
  generator.generate();

  return 0;
}
