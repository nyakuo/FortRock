#include "CModuleGenerator.hpp"

/**
   コンストラクタ
   @brief テスト用の入力(DFG)を与えている
   @param[in] filename 出力ファイル名
 */
CModuleGenerator::CModuleGenerator(const std::string & filename) {
  this->_module = std::make_shared<CModule>("test");
  this->_cout.open_ofstream(filename);

  // ノード確保
  // 基本入出力
  auto i_clk = std::make_shared<CDFG_Node>
    (CDFG_Node("i_clk", 1, false, CDFG_Node::eNode::CLK));
  auto i_res = std::make_shared<CDFG_Node>
    (CDFG_Node("i_res_p", 1, false, CDFG_Node::eNode::RES));
  auto i_req = std::make_shared<CDFG_Node>
    (CDFG_Node("i_req_p", 1, false, CDFG_Node::eNode::REQ));
  auto i_ce = std::make_shared<CDFG_Node>
    (CDFG_Node("i_ce_p", 1, false, CDFG_Node::eNode::CE));
  auto o_fin = std::make_shared<CDFG_Node>
    (CDFG_Node("o_fin_p", 1, false, CDFG_Node::eNode::FIN));

  // 入力
  auto a = std::make_shared<CDFG_Node>
    (CDFG_Node("i_a", 8, true, CDFG_Node::eNode::IN));
  auto b = std::make_shared<CDFG_Node>
    (CDFG_Node("i_b", 8, true, CDFG_Node::eNode::IN));

  // 出力
  auto out = std::make_shared<CDFG_Node>
    (CDFG_Node("o_out", 8, true, CDFG_Node::eNode::OUT));

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
  this->_module->add_node(i_clk);
  this->_module->add_node(i_res);
  this->_module->add_node(i_req);
  this->_module->add_node(i_ce);
  this->_module->add_node(o_fin);
  this->_module->add_node(a);
  this->_module->add_node(b);
  this->_module->add_node(out);
  this->_module->add_node(p_3);
  this->_module->add_node(p_true);
  this->_module->add_node(p_false);
  this->_module->add_node(p_zero);
  this->_module->add_node(t1);
  this->_module->add_node(adder1_i_a);
  this->_module->add_node(adder1_i_b);
  this->_module->add_node(adder1_out);
  this->_module->add_node(adder2_i_a);
  this->_module->add_node(adder2_i_b);
  this->_module->add_node(adder2_out);
  this->_module->add_node(s_state);
  this->_module->add_node(s_step);

  // 演算器
  //! @todo CLKやCEの接続を行うとElementの入出力と整合性がとれなくなる
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
  this->_module->add_operator(add);
  this->_module->add_operator(sub);

  // DFG
  // t1 = input_a + input_b
  auto elem1 = std::make_shared<CDFG_Element>(CDFG_Element(add));
  elem1->set_input(a, 0);
  elem1->set_input(b, 1);
  elem1->set_output(t1, 0);
  elem1->set_state(1);
  elem1->set_step(0);
  this->_module->add_element(elem1);

  // out = t1 + p_3
  auto elem2 = std::make_shared<CDFG_Element>(CDFG_Element(sub));
  elem2->set_input(t1, 0);
  elem2->set_input(p_3, 1);
  elem2->set_output(out, 0);
  elem2->set_state(1);
  elem2->set_step(3);
  this->_module->add_element(elem2);
}

int CModuleGenerator::generate(void) {
  this->_generate_header();
  this->_generate_define();
  this->_generate_assign();
  this->_generate_calculator();
  this->_generate_always();
  this->_generate_footer();

  return 0;
}

/**
   moduleの入出力定義部の出力
*/
void CModuleGenerator::_generate_header(void) {
  std::list<std::string> io_list;
  this->_cout << "`default_nettype none\n\n"
              << "module " << this->_module->get_name() << std::endl;
  this->_cout.indent_right();
  this->_cout << "(\n";

  // 入出力信号の出力
  for (auto & io : this->_module->get_node_list()) {
    std::string io_str;
    switch (io->get_type()) {
    case CDFG_Node::eNode::IN:
    case CDFG_Node::eNode::CLK:
    case CDFG_Node::eNode::RES:
    case CDFG_Node::eNode::REQ:
    case CDFG_Node::eNode::CE:
      io_str = "input wire ";
      break;

    case CDFG_Node::eNode::OUT:
    case CDFG_Node::eNode::FIN:
      io_str = "output reg ";
      break;

    default:
      continue;
    }

    if(io->get_is_signed())
      io_str += "signed ";

    if (io->get_bit_width() > 1) {
      io_str += "["
        + std::to_string(io->get_bit_width() - 1)
        + ":0] ";
    }

    io_str += io->get_name();
    io_list.push_back(io_str);
  }

  for (auto ite = io_list.begin();
       ite != io_list.end();
       ) {
    this->_cout << (*ite);
    if ((++ite) != io_list.end())
      this->_cout <<= ',';
    this->_cout << "\n";
  }

  this->_cout << ");" << std::endl;
}

/**
   moduleの信号宣言部の出力
*/
void CModuleGenerator::_generate_define(void) {
  unsigned type;
  std::string types[3] = {"reg", "wire", "parameter"};
  std::array<std::stringstream, 3> streams;

  const auto reg = 0;
  const auto wire = 1;
  const auto param = 2;
  const auto none = 3;

  for (auto & node : this->_module->get_node_list()) {
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

    streams[type] << this->_cout.output_indent() << types[type] << ' ';

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

  for (auto & s : streams)
    if (s.rdbuf()->in_avail() != 0)
      this->_cout <<= s.str() + "\n";
}

/**
   moduleのassign文の定義
   演算器の入出力ポートの割り当て

   @note phi命令の実装に使用する
*/
void CModuleGenerator::_generate_assign(void) {
}

/**
   moduleの演算器定義の出力
*/
void CModuleGenerator::_generate_calculator(void) {
  // 各演算器の入出力信号の接続
  for (auto & module : this->_module->get_operator_list()) {
    this->_cout << module->get_mod_name() << ' '
                << module->get_name() << "(\n";

    this->_cout.indent_right();
    for (unsigned at = 0; at < module->get_num_input(); ++at) {
      this->_cout << '.'
                  << module->get_input_signal_at(at)
                  << '(' << module->get_input_node_at(at)->get_name() << "),"
                  << std::endl;
    }

    for (unsigned at = 0; at < module->get_num_output(); ++at) {
      this->_cout << '.'
                  << module->get_output_signal_at(at)
                  << '(' << module->get_output_node_at(at)->get_name() << ')'
                  << std::endl;
    }

    this->_cout.indent_left();
    this->_cout << ");\n" << std::endl;
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
  for (auto & node : this->_module->get_node_list()) {
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

  this->_cout << "always @(posedge "
              << clk_name
              << ')' << std::endl;

  // リセットの出力
  this->_cout.indent_right();
  this->_cout << "begin" << std::endl;
  this->_cout.indent_right();
  this->_cout << "if ("
              << res_name << " ==  TRUE)\n";
  this->_cout.indent_right();
  this->_cout << "begin\n";
  this->_cout.indent_right();
  this->_cout << fin_name << " <= FALSE;\n";
  this->_cout << node_state->get_name() << " <= ZERO;\n";
  this->_cout << node_step->get_name() << " <= ZERO;\n";
  this->_cout.indent_left();
  this->_cout << "end\n";
  this->_cout.indent_left();

  // clock enable == TRUE
  this->_cout << "else if (" << ce_name << " == TRUE)\n";
  this->_cout.indent_right();
  this->_cout << "begin\n";
  this->_cout.indent_right();

  // ステートマシンのリセット
  this->_cout << "case (" << node_state->get_name() <<  ")\n";
  this->_cout.indent_right();
  this->_cout << node_state->get_bit_width() << "'h0:\n";
  this->_cout.indent_right();
  this->_cout << "begin\n";
  this->_cout.indent_right();
  this->_cout << fin_name << " <= FALSE;\n";
  this->_cout << node_step->get_name() << " <= ZERO;\n";
  this->_cout << "if (" << req_name << ")\n";
  this->_cout.indent_right();
  this->_cout << "begin\n";
  this->_cout.indent_right();
  this->_cout << node_state->get_name() << " <= "
              << node_state->get_bit_width() << "'h1;\n";
  this->_cout.indent_left();
  this->_cout << "end\n";
  this->_cout.indent_left(2);
  this->_cout << "end\n";

  // ステートマシンの出力
  // DFG ---> list(接続タイミング)
  this->_cout.indent_right(4);
  CStateMachineGen sm_gen;
  for (auto & elem : this->_module->get_element_list()) {
    auto ope = elem->get_operator();
    auto state = elem->get_state();
    auto step = elem->get_step();
    auto latency = ope->get_latency();
    std::string process_str ("");

    // 入力の接続
    ope = elem->get_operator();
    for (auto i=0; i<ope->get_num_input(); ++i) {
      auto node = ope->get_input_node_at(i);
      if ((unsigned)node->get_type() &
          ((unsigned)CDFG_Node::eNode::REG | (unsigned)CDFG_Node::eNode::OUT)) {
        process_str.append(this->_cout.output_indent()
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
        process_str.append(this->_cout.output_indent()
                           + elem->get_output_at(i)->get_name()
                           + " <= "
                           + node->get_name()
                           + ";\n");
      }
    }
    sm_gen.add_state_process(state,
                             step + latency + 1,
                             process_str);
  }

  this->_cout.indent_left(5);

  // list(接続タイミング) ---> always
  auto state_step_list = sm_gen.get_state_step_list();
  for(auto ite_state_step = state_step_list.begin();
      ite_state_step != state_step_list.end(); ) {
    // ステートの出力
    this->_cout << node_state->get_bit_width()
                << "'h" << std::hex << ite_state_step->first << ":\n";
    this->_cout.indent_right();
    this->_cout << "begin\n";

    // ステップの出力
    this->_cout.indent_right();
    this->_cout << node_step->get_name() << " <= "
                << node_step->get_name() << " + 1'h1;\n";
    this->_cout << "case (" << node_step->get_name() << ")\n";
    auto range = state_step_list.equal_range(ite_state_step->first);
    for (auto ite = range.first;
         ite != range.second;
         ++ite, ++ite_state_step) {
      this->_cout.indent_right();
      this->_cout << node_step->get_bit_width()
                  << "'h" << std::hex << ite->second << ":\n";
      this->_cout.indent_right();
      this->_cout << "begin\n";
      this->_cout.indent_right();

      this->_cout  <<= sm_gen.gen_state_machine(ite->first,
                                                ite->second);

      this->_cout.indent_left();
      this->_cout << "end\n";
      this->_cout.indent_left(2);
    }
    this->_cout << "endcase\n";
    this->_cout.indent_left();
    this->_cout << "end\n";
    this->_cout.indent_left();
  }

  this->_cout.indent_left();
  this->_cout << "endcase\n";
  this->_cout.indent_left();
  this->_cout << "end\n";
  this->_cout.indent_left(2);
  this->_cout << "end\n";
  this->_cout.indent_left();
}

/**
   module定義の尾部の出力
   @note デバッグ用データの出力などに利用
*/
void CModuleGenerator::_generate_footer(void) {
  this->_cout.indent_left();
  this->_cout << "endmodule\n\n"
              << "`default_nettype wire" << std::endl;
}
