/**
   テストプログラム
   命令列のDFG化に関するクラスの実装
   およびVerilogコードの出力機能に関する検証
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <array>
#include <algorithm>

#include "CDFG_Element.hpp"
#include "CDFG_Node.hpp"
#include "CDFG_Operator.hpp"
#include "CStateMachineGen.hpp"

class CModuleGenerator {
private:
  const std::string OUTPUT_FILE = "output.v";
  const unsigned INDENT_WIDTH;

  unsigned _indent_level;
  std::string _filename;
  std::string _module_name;
  std::ofstream _ofs;

  std::list<std::shared_ptr<CDFG_Element> > _dfg; /** モジュール内のElementのリスト */
  std::list<std::shared_ptr<CDFG_Node> > _node_list; /** モジュール内のNode(reg, wire, parameter)のリスト */
  std::list<std::shared_ptr<CDFG_Operator> >_operator_list; /** モジュール内の演算モジュールリスト */

  std::string _print_spaces(const int & num_space);
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

    // ノード確保
    // 入力
    auto i_clk = std::make_shared<CDFG_Node>
      (CDFG_Node("i_clk", 1, false, CDFG_Node::eNode::CLK));
    auto i_res = std::make_shared<CDFG_Node>
      (CDFG_Node("i_res_p", 1, false, CDFG_Node::eNode::RES));
    auto i_req = std::make_shared<CDFG_Node>
      (CDFG_Node("i_req_p", 1, false, CDFG_Node::eNode::REQ));
    auto i_ce = std::make_shared<CDFG_Node>
      (CDFG_Node("i_ce_p", 1, false, CDFG_Node::eNode::CE));
    auto a = std::make_shared<CDFG_Node>
      (CDFG_Node("i_a", 8, true, CDFG_Node::eNode::IN));
    auto b = std::make_shared<CDFG_Node>
      (CDFG_Node("i_b", 8, true, CDFG_Node::eNode::IN));

    // 出力
    auto out = std::make_shared<CDFG_Node>
      (CDFG_Node("o_out", 8, true, CDFG_Node::eNode::OUT));
    auto o_fin = std::make_shared<CDFG_Node>
      (CDFG_Node("o_fin_p", 1, false, CDFG_Node::eNode::FIN));

    // 定数
    auto p_3 = std::make_shared<CDFG_Node>
      (CDFG_Node("p_3", 8, true, CDFG_Node::eNode::PARAM, 3));
    auto p_true = std::make_shared<CDFG_Node>
      (CDFG_Node("TRUE", 1, false, CDFG_Node::eNode::PARAM, 1));
    auto p_false = std::make_shared<CDFG_Node>
      (CDFG_Node("FALSE", 1, false, CDFG_Node::eNode::PARAM, 0));
    auto p_zero = std::make_shared<CDFG_Node>
      (CDFG_Node("ZERO", 1, false, CDFG_Node::eNode::PARAM, 0));


    // テンプレートレジスタ
    auto t1 = std::make_shared<CDFG_Node>
      (CDFG_Node("t1", 8, true, CDFG_Node::eNode::REG));

    // 演算器の入出力
    auto adder1_i_a = std::make_shared<CDFG_Node>
      (CDFG_Node("mr_adder1_i_a", 8, true, CDFG_Node::eNode::REG));
    auto adder1_i_b = std::make_shared<CDFG_Node>
      (CDFG_Node("mr_adder1_i_b", 8, true, CDFG_Node::eNode::REG));
    auto adder1_out = std::make_shared<CDFG_Node>
      (CDFG_Node("mw_adder1_output", 8, true, CDFG_Node::eNode::WIRE));

    auto adder2_i_a = std::make_shared<CDFG_Node>
      (CDFG_Node("mr_adder2_i_a", 8, true, CDFG_Node::eNode::REG));
    auto adder2_i_b = std::make_shared<CDFG_Node>
      (CDFG_Node("mr_adder2_i_b", 8, true, CDFG_Node::eNode::REG));
    auto adder2_out = std::make_shared<CDFG_Node>
      (CDFG_Node("mw_adder2_output", 8, true, CDFG_Node::eNode::WIRE));

    // システム変数
    auto s_state = std::make_shared<CDFG_Node>
      (CDFG_Node("r_sys_state", 8, false, CDFG_Node::eNode::STATE));
    auto s_step = std::make_shared<CDFG_Node>
      (CDFG_Node("r_sys_step", 8, false, CDFG_Node::eNode::STEP));

    // 信号登録
    this->_node_list.emplace_back(i_clk);
    this->_node_list.emplace_back(i_res);
    this->_node_list.emplace_back(i_req);
    this->_node_list.emplace_back(i_ce);
    this->_node_list.emplace_back(o_fin);
    this->_node_list.emplace_back(a);
    this->_node_list.emplace_back(b);
    this->_node_list.emplace_back(out);
    this->_node_list.emplace_back(p_3);
    this->_node_list.emplace_back(p_true);
    this->_node_list.emplace_back(p_false);
    this->_node_list.emplace_back(p_zero);
    this->_node_list.emplace_back(t1);
    this->_node_list.emplace_back(adder1_i_a);
    this->_node_list.emplace_back(adder1_i_b);
    this->_node_list.emplace_back(adder1_out);
    this->_node_list.emplace_back(adder2_i_a);
    this->_node_list.emplace_back(adder2_i_b);
    this->_node_list.emplace_back(adder2_out);
    this->_node_list.emplace_back(s_state);
    this->_node_list.emplace_back(s_step);

    // 演算器
    // todo: CLKやCEの接続を行うとElementの入出力と整合性がとれなくなる
    auto add = std::make_shared<CDFG_Operator>
      (CDFG_Operator("my_add1",
                     "my_add",
                     1,
                     CDFG_Operator::eType::ADD));

    add->add_input_port("i_a", adder1_i_a);
    add->add_input_port("i_b", adder1_i_b);
    add->add_output_port("o_output", adder1_out);
    add->add_input_port("clock", i_clk);
    add->add_input_port("ce", i_ce);

    auto sub = std::make_shared<CDFG_Operator>
      (CDFG_Operator("my_sub1",
                     "my_sub",
                     1,
                     CDFG_Operator::eType::SUB));

    sub->add_input_port("i_a", adder2_i_a);
    sub->add_input_port("i_b", adder2_i_b);
    sub->add_output_port("o_output", adder2_out);
    sub->add_input_port("clock", i_clk);
    sub->add_input_port("ce", i_ce);

    // 演算器登録
    this->_operator_list.emplace_back(add);
    this->_operator_list.emplace_back(sub);

    // DFG
    // t1 = input_a + input_b
    auto elem1 = std::make_shared<CDFG_Element>(CDFG_Element(add));
    elem1->set_input(a, 0);
    elem1->set_input(b, 1);
    elem1->set_output(t1, 0);
    elem1->set_state(1);
    elem1->set_step(0);
    this->_dfg.emplace_back(elem1);

    // out = t1 + p_3
    auto elem2 = std::make_shared<CDFG_Element>(CDFG_Element(sub));
    elem2->set_input(t1, 0);
    elem2->set_input(p_3, 1);
    elem2->set_output(out, 0);
    elem2->set_state(1);
    elem2->set_step(2);
    this->_dfg.emplace_back(elem2);
  }
}

/**
   スペース文字の出力
   @param[in] num_space 出力するスペース文字の数
   @return スペース文字列
*/
std::string CModuleGenerator::_print_spaces(const int & num_space) {
  std::string ret_str = "";

  for(int i=0; i<num_space; ++i)
    ret_str += ' ';

  return ret_str;
}

int CModuleGenerator::generate(void) {
  //  this->_ofs.open(this->_filename, std::ios::out);
  this->_ofs.open(this->OUTPUT_FILE, std::ios::out);
  if (!this->_ofs)
    return -1;

  this->_generate_header();
  this->_generate_define();
  // this->_generate_assign();
  this->_generate_calculator();
  this->_generate_always();
  this->_generate_footer();

  this->_ofs.close();
  return 0;
}

/**
   moduleの入出力定義部の出力
 */
void CModuleGenerator::_generate_header(void) {
  this->_ofs << "`default_nettype none\n\n"
              << "module " << this->_module_name << std::endl;

  this->_indent_right();
  this->_ofs << this->_indent() << "(\n";

  // 入出力信号の出力
  for (auto & io : this->_node_list) {
    switch (io->get_type()) {
    case CDFG_Node::eNode::IN:
    case CDFG_Node::eNode::CLK:
    case CDFG_Node::eNode::RES:
    case CDFG_Node::eNode::REQ:
    case CDFG_Node::eNode::CE:
      this->_ofs << this->_indent() << "input wire ";
      break;

    case CDFG_Node::eNode::OUT:
    case CDFG_Node::eNode::FIN:
      this->_ofs << this->_indent() << "output reg ";
      break;

    default:
      continue;
    }

    if(io->get_is_signed())
      this->_ofs << "signed ";

    if (io->get_bit_width() > 1) {
      this->_ofs << "[" << io->get_bit_width() - 1 << ":0] ";
    }

    // todo: 最後のoutput出力には","を付けない
    this->_ofs << io->get_name()
               << ',';
    this->_ofs << '\n';
  }

  this->_ofs << this->_indent() << ");" << std::endl;
}

/**
   moduleの信号宣言部の出力
 */
void CModuleGenerator::_generate_define(void) {
  unsigned type;
  const auto reg = 0;
  const auto wire = 1;
  const auto param = 2;
  const auto none = 3;
  std::array<std::stringstream, 3> streams;
  std::string types[3] = {"reg", "wire", "parameter"};

  for (auto & node : this->_node_list) {
    switch(node->get_type()) {
    case CDFG_Node::eNode::REG:
    case CDFG_Node::eNode::STATE:
    case CDFG_Node::eNode::STEP:
      type = reg;
      break;

    case CDFG_Node::eNode::WIRE:
      type = wire;
      break;

    case CDFG_Node::eNode::PARAM:
      type = param;
      break;

    default:
      continue;
    }

    streams[type] << this->_indent() << types[type] << ' ';

    if (node->get_is_signed()) {
      streams[type] << "signed ";
    }

    if (node->get_bit_width() > 1) {
      streams[type] << "[" << node->get_bit_width() - 1 << ":0] ";
    }

    streams[type] << node->get_name();

    if (type == param)
      streams[param] << " = " << node->get_bit_width()
                     << "'h" << std::hex << node->get_parameter();

    streams[type] << ";\n";
  }

  for (auto & s : streams) {
    if (s.rdbuf()->in_avail() != 0) {
      this->_ofs << s.str() << std::endl;
    }
  }
}

/**
   moduleのassign文の定義
   演算器の入出力ポートの割り当て

   @note 実質必要がないので，現状では空
 */
void CModuleGenerator::_generate_assign(void) {
}

/**
   moduleの演算器定義の出力
 */
void CModuleGenerator::_generate_calculator(void) {
  // 各演算器の入出力信号の接続
  for (auto & module : this->_operator_list) {
    this->_ofs << this->_indent() << module->get_mod_name() << ' '
               << module->get_name() << "(\n";

    this->_indent_right();
    for (unsigned at = 0; at < module->get_num_input(); ++at) {
      this->_ofs << this->_indent() << '.'
                 << module->get_input_signal_at(at)
                 << '(' << module->get_input_node_at(at)->get_name() << "),"
                 << std::endl;
    }

    for (unsigned at = 0; at < module->get_num_output(); ++at) {
      this->_ofs << this->_indent() << '.'
                 << module->get_output_signal_at(at)
                 << '(' << module->get_output_node_at(at)->get_name() << ')'
                 << std::endl;
    }

    this->_indent_left();
    this->_ofs << this->_indent() << ");" << std::endl;
  }
}

/**
   moduleのステートマシンの出力
 */
void CModuleGenerator::_generate_always(void) {
  std::string clk_name;
  std::string res_name;
  std::string req_name;
  std::string ce_name;
  std::shared_ptr<CDFG_Node> node_state;
  std::shared_ptr<CDFG_Node> node_step;
  std::string fin_name;

  // 出力に必要な情報(信号名など)を取得
  for (auto & node : this->_node_list) {
    auto type = node->get_type();
    if (type == CDFG_Node::eNode::CLK)
      clk_name = node->get_name();

    else if (type == CDFG_Node::eNode::RES)
      res_name = node->get_name();

    else if (type == CDFG_Node::eNode::REQ)
      req_name = node->get_name();

    else if (type == CDFG_Node::eNode::CE)
      ce_name = node->get_name();

    else if (type == CDFG_Node::eNode::STATE)
      node_state = node;

    else if (type == CDFG_Node::eNode::STEP)
      node_step = node;

    else if (type == CDFG_Node::eNode::FIN)
      fin_name = node->get_name();
  }

  this->_ofs << this->_indent() << "always @(posedge "
             << clk_name
             << ')' << std::endl;

  // リセットの出力
  this->_indent_right();
  this->_ofs << this->_indent() << "begin" << std::endl;
  this->_indent_right();
  this->_ofs << this->_indent() << "if ("
             << res_name << " ==  TRUE)\n";
  this->_indent_right();
  this->_ofs << this->_indent() << "begin\n";
  this->_indent_right();
  this->_ofs << this->_indent() << fin_name << " <= FALSE;\n"
             << this->_indent() << node_state->get_name() << " <= ZERO;\n"
             << this->_indent() << node_step->get_name() << " <= ZERO;\n";
  this->_indent_left();
  this->_ofs << this->_indent() << "end\n";
  this->_indent_left();

  // clock enable == TRUE
  this->_ofs << this->_indent() << "else if ("
             << ce_name << " == TRUE)\n";
  this->_indent_right();
  this->_ofs << this->_indent() << "begin\n";
  this->_indent_right();

  // ステートマシンのリセット
  this->_ofs << this->_indent() << "case (" << node_state->get_name() <<  ")\n";
  this->_indent_right();
  this->_ofs << this->_indent() << node_state->get_bit_width() << "'h0:\n";
  this->_indent_right();
  this->_ofs << this->_indent() << "begin\n";
  this->_indent_right();
  this->_ofs << this->_indent() << fin_name << " <= FALSE;\n";
  this->_ofs << this->_indent() << node_step->get_name() << " <= ZERO;\n";
  this->_ofs << this->_indent() << "if (" << req_name << ")\n";
  this->_indent_right();
  this->_ofs << this->_indent() << "begin\n";
  this->_indent_right();
  this->_ofs << this->_indent() << node_state->get_name() << " <= "
             << node_state->get_bit_width() << "'h1;\n";
  this->_indent_left();
  this->_ofs << this->_indent() << "end\n";
  this->_indent_left();

  this->_indent_left();
  this->_ofs << this->_indent() << "end\n";

  // ステートマシンの出力
  // DFG ---> list(接続タイミング)
  this->_indent_right();
  this->_indent_right();
  this->_indent_right();
  this->_indent_right();
  CStateMachineGen sm_gen;
  for (auto & elem : this->_dfg) {
    auto ope = elem->get_operator();
    auto state = elem->get_state();
    auto step = elem->get_step();
    auto latency = ope->get_latency();
    std::string process_str = "";

    // 入力の接続
    ope = elem->get_operator();
    for (auto i=0; i<ope->get_num_input(); ++i) {
      auto node = ope->get_input_node_at(i);
      if ((unsigned)node->get_type() &
          ((unsigned)CDFG_Node::eNode::REG | (unsigned)CDFG_Node::eNode::OUT)) {
        process_str.append(this->_indent()
                             + node->get_name()
                             + " <= "
                             + elem->get_input_at(i)->get_name()
                             + ";\n");
      }
    }
    sm_gen.add_state_process(state, step, process_str);

    // 出力の接続
    process_str = "";
    for (auto i=0; i<ope->get_num_output(); ++i) {
      auto node = ope->get_output_node_at(i);

      if ((unsigned)node->get_type() &
          ((unsigned)CDFG_Node::eNode::WIRE)) {
        process_str.append(this->_indent()
                             + elem->get_output_at(i)->get_name()
                             + " <= "
                             + node->get_name()
                             + ";\n");
      }
    }
    sm_gen.add_state_process(state,
                             step + latency,
                             process_str);
  }

  this->_indent_left();
  this->_indent_left();
  this->_indent_left();
  this->_indent_left();
  this->_indent_left();

  // list(接続タイミング) ---> always
  auto state_step_list = sm_gen.get_state_step_list();
  for(auto ite_state_step = state_step_list.begin();
      ite_state_step != state_step_list.end();
      ) {
    // ステートの出力
    this->_ofs << this->_indent()
               << node_state->get_bit_width()
               << "'h" << std::hex << ite_state_step->first << ":\n";
    this->_indent_right();
    this->_ofs << this->_indent()
               << "begin\n";

    // ステップの出力
    this->_indent_right();
    this->_ofs << this->_indent()
               << "case (" << node_step->get_name() << ")\n";
    auto range = state_step_list.equal_range(ite_state_step->first);
    for (auto ite = range.first;
         ite != range.second;
         ++ite, ++ite_state_step) {
      this->_indent_right();
      this->_ofs << this->_indent()
                 << node_step->get_bit_width()
                 << "'h" << std::hex << ite->second << ":\n";
      this->_indent_right();
      this->_ofs << this->_indent() << "begin\n";
      this->_indent_right();

      this->_ofs  << sm_gen.gen_state_machine(ite->first,
                                              ite->second);

      this->_indent_left();
      this->_ofs << this->_indent() << "end\n";
      this->_indent_left();
      this->_indent_left();
    }
    this->_ofs << this->_indent() << "endcase\n";

    this->_indent_left();

    this->_ofs << this->_indent()
               << "end\n";
    this->_indent_left();
  }

  this->_indent_left();
  this->_ofs << this->_indent() << "endcase\n";
  this->_indent_left();
  this->_ofs << this->_indent() << "end\n";
  this->_indent_left();
  this->_indent_left();
  this->_ofs << this->_indent() << "end\n";
}

/**
  module定義の尾部の出力
  @note デバッグ用データの出力などに利用
 */
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
