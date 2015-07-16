#include "CModuleGenerator.hpp"

/**
   コンストラクタ
   @brief テスト用の入力(DFG)を与えている
   @param[in] filename 出力ファイル名
 */
CModuleGenerator::CModuleGenerator(const std::string & filename,
                                   const std::string & module_name) {
  this->_module = std::make_shared<CModule>(module_name);
  this->_cout.open_ofstream(filename);

  // 基本的なノードの確保
  // 基本入出力
  auto i_clk = std::make_shared<CDFG_Node>
    ("clk", 1, false, CDFG_Node::eNode::CLK);
  auto i_res = std::make_shared<CDFG_Node>
    ("res_p", 1, false, CDFG_Node::eNode::RES);
  auto i_req = std::make_shared<CDFG_Node>
    ("req_p", 1, false, CDFG_Node::eNode::REQ);
  auto i_ce = std::make_shared<CDFG_Node>
    ("ce_p", 1, false, CDFG_Node::eNode::CE);
  auto o_fin = std::make_shared<CDFG_Node>
    ("fin_p", 1, false, CDFG_Node::eNode::FIN);
  // 定数
  auto p_true = std::make_shared<CDFG_Node>
    ("TRUE", 1, false, CDFG_Node::eNode::TRUE, 1);
  auto p_false = std::make_shared<CDFG_Node>
    ("FALSE", 1, false, CDFG_Node::eNode::FALSE, 0);
  auto p_zero = std::make_shared<CDFG_Node>
    ("ZERO", 1, false, CDFG_Node::eNode::ZERO, 0);

  this->_module->add_node(i_clk);
  this->_module->add_node(i_res);
  this->_module->add_node(i_req);
  this->_module->add_node(i_ce);
  this->_module->add_node(o_fin);
  this->_module->add_node(p_true);
  this->_module->add_node(p_false);
  this->_module->add_node(p_zero);
}

/**
   モジュールにノードを追加する
   @note FortRock 本体から呼び出すために使用
 */
void CModuleGenerator::add_node
(std::shared_ptr<CDFG_Node> & node) {
  this->_module->add_node(node);
}

/**
   モジュールに演算器を追加する
 */
void CModuleGenerator::add_operator
(std::shared_ptr<CDFG_Operator> & ope) {
  this->_module->add_operator(ope);
}

/**
   モジュールにElement(処理)を追加する
 */
void CModuleGenerator::add_element
(std::shared_ptr<CDFG_Element> & elem) {
  this->_module->add_element(elem);
}

/**
   モジュール内のノードを検索する
   @param[in] node_name 検索対象のアセンブリ上での名前
   @note FortRock 本体から呼び出すために使用
*/
bool CModuleGenerator::find_node
(const std::string & node_name) {
  return this->_module->find_node(node_name);
}

/**
   モジュール内のノードを検索する
   @note FortRock 本体から呼び出すために使用
*/
bool CModuleGenerator::find_node
(std::shared_ptr<CDFG_Node> & node) {
  return this->_module->find_node(node->get_asm_name());
}

/**
   モジュール内のノードを取得する
   @note FortRock 本体から呼び出すために使用
 */
std::shared_ptr<CDFG_Node>
CModuleGenerator::get_node
(const std::string & node_name){
  return this->_module->get_node(node_name);
}

/**
   モジュール内のノードを取得する
   @note FortRock 本体から呼び出すために使用
         (stateなどの取得に使用)
*/
std::shared_ptr<CDFG_Node>
CModuleGenerator::get_node
(const CDFG_Node::eNode & type) {
  return this->_module->get_node(type);
}

/**
   モジュール内の演算器の取得
   @note FortRock本体から取得するために使用
   @todo 取得する演算器の番号による指定など
   @attention 暫定版
 */
std::shared_ptr<CDFG_Operator>
CModuleGenerator::get_operator
(const CDFG_Operator::eType & type) {
  auto l = this->_module->get_operator_list();
  auto ite =
    std::find_if(l.begin(), l.end(),
                 [type](std::shared_ptr<CDFG_Operator> obj) -> bool {
                   return obj->get_type() == type;
                 });

  return *ite;
}

/**
   モジュールの最大ステップを取得
   @note step信号を定義する際にのビット幅を
         調べるためにFortRockが使用
   @return step信号に必要なビット幅
 */
unsigned
CModuleGenerator::get_max_step(void) {
  return this->_module->get_max_step();
}

/**
   テストデータ(DFG)の生成
   @note テスト用
 */
void CModuleGenerator::_generate_test_data(void) {
  // ノード確保
  // 基本入出力
  auto i_clk = std::make_shared<CDFG_Node>
    ("i_clk", 1, false, CDFG_Node::eNode::CLK);
  auto i_res = std::make_shared<CDFG_Node>
    ("i_res_p", 1, false, CDFG_Node::eNode::RES);
  auto i_req = std::make_shared<CDFG_Node>
    ("i_req_p", 1, false, CDFG_Node::eNode::REQ);
  auto i_ce = std::make_shared<CDFG_Node>
    ("i_ce_p", 1, false, CDFG_Node::eNode::CE);
  auto o_fin = std::make_shared<CDFG_Node>
    ("o_fin_p", 1, false, CDFG_Node::eNode::FIN);

  // 入力
  auto a = std::make_shared<CDFG_Node>
    ("i_a", 8, true, CDFG_Node::eNode::IN);
  auto b = std::make_shared<CDFG_Node>
    ("i_b", 8, true, CDFG_Node::eNode::IN);

  // 出力
  auto out = std::make_shared<CDFG_Node>
    ("o_out", 8, true, CDFG_Node::eNode::OUT);

  // 定数
  auto p_3 = std::make_shared<CDFG_Node>
    ("p_3", 8, true, CDFG_Node::eNode::PARAM, 3);
  auto p_true = std::make_shared<CDFG_Node>
    ("TRUE", 1, false, CDFG_Node::eNode::PARAM, 1);
  auto p_false = std::make_shared<CDFG_Node>
    ("FALSE", 1, false, CDFG_Node::eNode::PARAM, 0);
  auto p_zero = std::make_shared<CDFG_Node>
    ("ZERO", 1, false, CDFG_Node::eNode::PARAM, 0);

  // テンプレートレジスタ
  auto t1 = std::make_shared<CDFG_Node>
    ("t1", 8, true, CDFG_Node::eNode::REG);

  // 演算器の入出力
  auto adder1_i_a = std::make_shared<CDFG_Node>
    ("mr_adder1_i_a", 8, true, CDFG_Node::eNode::REG);
  auto adder1_i_b = std::make_shared<CDFG_Node>
    ("mr_adder1_i_b", 8, true, CDFG_Node::eNode::REG);
  auto adder1_out = std::make_shared<CDFG_Node>
    ("mw_adder1_output", 8, true, CDFG_Node::eNode::WIRE);

  auto adder2_i_a = std::make_shared<CDFG_Node>
    ("mr_adder2_i_a", 8, true, CDFG_Node::eNode::REG);
  auto adder2_i_b = std::make_shared<CDFG_Node>
    ("mr_adder2_i_b", 8, true, CDFG_Node::eNode::REG);
  auto adder2_out = std::make_shared<CDFG_Node>
    ("mw_adder2_output", 8, true, CDFG_Node::eNode::WIRE);

  // システム変数
  auto s_state = std::make_shared<CDFG_Node>
    ("r_sys_state", 8, false, CDFG_Node::eNode::STATE);
  auto s_step = std::make_shared<CDFG_Node>
    ("r_sys_step", 8, false, CDFG_Node::eNode::STEP);

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
    ("my_add1",
     "my_add",
     1,
     CDFG_Operator::eType::ADD);

  add->add_input_port("i_a", adder1_i_a);
  add->add_input_port("i_b", adder1_i_b);
  add->add_output_port("o_output", adder1_out);
  add->add_input_port("clock", i_clk);
  add->add_input_port("ce", i_ce);

  auto sub = std::make_shared<CDFG_Operator>
    ("my_sub1",
     "my_sub",
     1,
     CDFG_Operator::eType::SUB);

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
  auto elem1 = std::make_shared<CDFG_Element>(add);
  elem1->set_input(a, 0);
  elem1->set_input(b, 1);
  elem1->set_output(t1, 0);
  elem1->set_state(1);
  elem1->set_step(0);
  this->_module->add_element(elem1);

  // out = t1 + p_3
  auto elem2 = std::make_shared<CDFG_Element>(sub);
  elem2->set_input(t1, 0);
  elem2->set_input(p_3, 1);
  elem2->set_output(out, 0);
  elem2->set_state(1);
  elem2->set_step(3);
  this->_module->add_element(elem2);
}

/**
   ステートマシン回路をファイルへ出力
 */
int CModuleGenerator::generate(void) {
  this->_generate_header();
  this->_generate_define();
  this->_generate_assign();
  this->_generate_function();
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
    } // switch

    if(io->get_is_signed())
      io_str += "signed ";

    if (io->get_bit_width() > 1) {
      io_str += "["
        + std::to_string(io->get_bit_width() - 1)
        + ":0] ";
    } // if

    io_str += io->get_verilog_name();
    io_list.push_back(io_str);
  } // for

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
    case CDFG_Node::eNode::PREV_STATE:
    case CDFG_Node::eNode::STEP:
      type = reg;
      break;

    case CDFG_Node::eNode::WIRE:
      type = wire;
      break;

    case CDFG_Node::eNode::TRUE:
    case CDFG_Node::eNode::FALSE:
    case CDFG_Node::eNode::ZERO:
    case CDFG_Node::eNode::FINISH_LABEL:
    case CDFG_Node::eNode::LABEL:
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

    streams[type] << node->get_verilog_name();

    if (type == param)
      streams[param] << " = "
                     << node->get_param_str();

    streams[type] << ";\n";
  }

  for (auto & s : streams)
    if (s.rdbuf()->in_avail() != 0)
      this->_cout <<= s.str() + "\n";
}

/**
   moduleのassign文の定義
   @note function文の assign に使用
*/
void CModuleGenerator::_generate_assign(void) {
  auto prev_state_node = this->get_node
    (CDFG_Node::eNode::PREV_STATE);

  for (auto & elem : this->_module->get_element_list()) {
    if (elem->get_operator()->get_type()
        == CDFG_Operator::eType::PHI) {
      auto dest_node = elem->get_output_at(0);

      this->_cout << "wire ["
                  << dest_node->get_bit_width() - 1
                  << ":0] w_phi_"
                  << dest_node->get_safe_name()
                  << ";\n";

      this->_cout << "assign w_phi_"
                  << dest_node->get_safe_name()
                  << " = phi_" << dest_node->get_verilog_name()
                  << "("
                  << prev_state_node->get_verilog_name();

      for (auto i=1;
           i < elem->get_num_input();
           i += 2) {
        this->_cout <<= ", " + elem->get_input_at(i)->get_verilog_name();
      } // for : i

      this->_cout <<= ");\n\n";

    } // if : PHI
  } // for : elem
} // _generate_assign

/**
   module の function 文の定義
   @note PHI命令の実装に用いる
   @attention 少なくとも2つ以上の条件が存在すると仮定
 */
void CModuleGenerator::_generate_function(void) {
  auto prev_state =
    this->get_node(CDFG_Node::eNode::PREV_STATE);

  for (auto & elem : this->_module->get_element_list()) {
    auto ope = elem->get_operator();

    if (ope->get_type() == CDFG_Operator::eType::PHI) {
      auto dest_node = elem->get_output_at(0);
      this->_cout << "function ["
                  << dest_node->get_bit_width() - 1
                  << ":0] phi_"
                  << dest_node->get_verilog_name()
                  << ";\n";

      this->_cout.indent_right();

      this->_cout << "input ["
                  << prev_state->get_bit_width() - 1
                  << ":0] "
                  << prev_state->get_verilog_name()
                  << ";\n";

      for (auto i=1;
           i < elem->get_num_input();
           i += 2) {
        this->_cout << "input ["
                    << elem->get_input_at(i)->get_bit_width() - 1
                    << ":0] "
                    << elem->get_input_at(i)->get_verilog_name()
                    << ";\n";
      }

      this->_cout << "case ("
                  << prev_state->get_verilog_name()
                  << ")\n";

      this->_cout.indent_right();

      // 条件による代入文
      for (auto i = 0;
           i < ope->get_num_input();
           i += 2) {
        this->_cout << elem->get_input_at(i)->get_verilog_name()
                    << ": phi_"
                    << dest_node->get_verilog_name()
                    << " = "
                    << elem->get_input_at(i+1)->get_verilog_name()
                    << ";\n";
      }

      auto zero_node = this->get_node(CDFG_Node::eNode::ZERO);
      this->_cout << "default: phi_"
                  << dest_node->get_verilog_name()
                  << " = "
                  << zero_node->get_verilog_name()
                  << ";\n";

      this->_cout.indent_left();

      this->_cout << "endcase\n";

      this->_cout.indent_left();

      this->_cout << "endfunction\n\n";
    } // if : phi
  } // for : elem
} // _generate_function

/**
   moduleの演算器定義の出力
*/
void CModuleGenerator::_generate_calculator(void) {
  // 各演算器の入出力信号の接続
  for (auto & module : this->_module->get_operator_list()) {
    this->_cout << module->get_mod_name() << ' '
                << module->get_name() << "(\n";

    this->_cout.indent_right();

    // 入力の接続
    unsigned at;
    for (at = 0; at < module->get_num_input(); ++at) {
      this->_cout << '.'
                  << module->get_input_signal_at(at)
                  << '(' << module->get_input_node_at(at)->get_verilog_name() << "),"
                  << std::endl;
    }

    // 出力の接続
    for (at = 0; at < module->get_num_output() - 1; ++at) {
      this->_cout << '.'
                  << module->get_output_signal_at(at)
                  << '(' << module->get_output_node_at(at)->get_verilog_name() << "),"
                  << std::endl;
    }

    // 最後の出力の接続
    this->_cout << '.'
                << module->get_output_signal_at(at)
                << '(' << module->get_output_node_at(at)->get_verilog_name() << ')'
                << std::endl;

    this->_cout.indent_left();
    this->_cout << ");\n" << std::endl;
  }
}

/**
   moduleのステートマシンの出力
*/
void CModuleGenerator::_generate_always(void) {
  // 出力に必要な情報(信号名など)を取得
  auto clk_name   = this->get_node(CDFG_Node::eNode::CLK)->get_verilog_name();
  auto res_name   = this->get_node(CDFG_Node::eNode::RES)->get_verilog_name();
  auto req_name   = this->get_node(CDFG_Node::eNode::REQ)->get_verilog_name();
  auto ce_name    = this->get_node(CDFG_Node::eNode::CE)->get_verilog_name();
  auto state_node = this->get_node(CDFG_Node::eNode::STATE);
  auto step_node  = this->get_node(CDFG_Node::eNode::STEP);
  auto fin_name   = this->get_node(CDFG_Node::eNode::FIN)->get_verilog_name();
  auto true_node  = this->get_node(CDFG_Node::eNode::TRUE);
  auto false_node = this->get_node(CDFG_Node::eNode::FALSE);
  auto zero_node  = this->get_node(CDFG_Node::eNode::ZERO);
  auto prev_state = this->get_node(CDFG_Node::eNode::PREV_STATE);

  this->_cout << "always @(posedge "
              << clk_name
              << ')' << std::endl;

  // リセットの出力
  this->_cout.indent_right();
  this->_cout << "begin" << std::endl;
  this->_cout.indent_right();
  this->_cout << "if ("
              << res_name
              << " ==  "
              << true_node->get_verilog_name()
              << ")\n";
  this->_cout.indent_right();
  this->_cout << "begin\n";
  this->_cout.indent_right();
  this->_cout << fin_name << " <= "
              << false_node->get_verilog_name()
              << ";\n";
  this->_cout << state_node->get_verilog_name()
              << " <= "
              << zero_node->get_verilog_name()
              << ";\n";
  this->_cout << step_node->get_verilog_name()
              << " <= "
              << zero_node->get_verilog_name()
              << ";\n";
  this->_cout.indent_left();
  this->_cout << "end\n";
  this->_cout.indent_left();

  // clock enable == TRUE
  this->_cout << "else if ("
              << ce_name
              << " == "
              << true_node->get_verilog_name()
              << ")\n";
  this->_cout.indent_right();
  this->_cout << "begin\n";
  this->_cout.indent_right();

  // ステートマシンのリセット
  this->_cout << "case (" << state_node->get_verilog_name() <<  ")\n";
  this->_cout.indent_right();
  this->_cout << state_node->get_bit_width() << "'h0:\n";
  this->_cout.indent_right();
  this->_cout << "begin\n";
  this->_cout.indent_right();
  this->_cout << fin_name
              << " <= "
              << false_node->get_verilog_name()
              << ";\n";
  this->_cout << step_node->get_verilog_name()
              << " <= "
              << zero_node->get_verilog_name()
              << ";\n";
  this->_cout << "if (" << req_name << ")\n";
  this->_cout.indent_right();
  this->_cout << "begin\n";
  this->_cout.indent_right();
  this->_cout << state_node->get_verilog_name() << " <= "
              << state_node->get_bit_width() << "'h1;\n";
  this->_cout.indent_left();
  this->_cout << "end\n";
  this->_cout.indent_left(2);
  this->_cout << "end\n";

  // ステートマシンの出力
  // DFG ---> list(接続タイミング)
  this->_cout.indent_right(4);
  CStateMachineGen sm_gen;
  for (auto & elem : this->_module->get_element_list()) {
    auto ope     = elem->get_operator();
    auto state   = elem->get_state();
    auto step    = elem->get_step();
    auto latency = ope->get_latency();
    std::string process_str ("");

    switch (ope->get_type()) {
    case CDFG_Operator::eType::ADD:
    case CDFG_Operator::eType::SUB:
    case CDFG_Operator::eType::ADD_SUB:
    case CDFG_Operator::eType::DIV:
    case CDFG_Operator::eType::FUNC:
    case CDFG_Operator::eType::MUL:
    case CDFG_Operator::eType::SREM:
    case CDFG_Operator::eType::SDIV:
      {
        // 入力の接続
        for (auto i=0; i<ope->get_num_input(); ++i) {
          auto node = ope->get_input_node_at(i);
          if ((unsigned)node->get_type() &
              ((unsigned)CDFG_Node::eNode::REG | // clk など回避
               (unsigned)CDFG_Node::eNode::OUT)) {
            process_str.append(this->_cout.output_indent()
                               + node->get_verilog_name()
                               + " <= "
                               + elem->get_input_at(i)->get_verilog_name()
                               + ";\n");
          }
        }
        sm_gen.add_state_process(state, step, process_str);

        // 出力の接続
        auto at = 0;
        if (ope->get_type() == CDFG_Operator::eType::SREM) // 2入力2出力演算器
          at = 1;

        process_str = "";
        auto ope_node = ope->get_output_node_at(at);

        process_str.append(this->_cout.output_indent()
                           + elem->get_output_at(at)->get_verilog_name()
                           + " <= "
                           + ope_node->get_verilog_name()
                           + ";\n");

      sm_gen.add_state_process(state,
                               step + latency + 1,
                               process_str);
        break;
      } // case

    case CDFG_Operator::eType::LOAD:
    case CDFG_Operator::eType::STORE:
      {
        auto in = elem->get_input_at(0);
        auto out = elem->get_output_at(0);

        process_str.append(this->_cout.output_indent()
                           + out->get_verilog_name()
                           + " <= "
                           + in->get_verilog_name()
                           + ";\n");
        sm_gen.add_state_process(state,
                                 step,
                                 process_str);
        break;
      }

    case CDFG_Operator::eType::ICMP:
      {
        auto in_0 = elem->get_input_at(0);
        auto in_1 = elem->get_input_at(1);
        auto out = elem->get_output_at(0);

        process_str.append(this->_cout.output_indent()
                           + out->get_verilog_name()
                           + " <= ("
                           + in_0->get_verilog_name()
                           + " < " //! @todo 他の比較条件へ対応
                           + in_1->get_verilog_name()
                           + ");\n");

        sm_gen.add_state_process(state,
                                 step,
                                 process_str);
        break;
      }

    case CDFG_Operator::eType::SELECT:
    case CDFG_Operator::eType::BR:
      {
        auto out  = elem->get_output_at(0);

        if (elem->get_num_input() == 3) { // 条件付き分岐
          auto tf   = elem->get_input_at(0);
          auto in_0 = elem->get_input_at(1);
          auto in_1 = elem->get_input_at(2);

          process_str.append(this->_cout.output_indent()
                             + out->get_verilog_name()
                             + " <= ( " + tf->get_verilog_name() + " ) ? "
                             + in_0->get_verilog_name()
                             + " : "
                             + in_1->get_verilog_name()
                             + ";\n");
        }
        else { // 無条件分岐
          auto label = elem->get_input_at(0);

          process_str.append(this->_cout.output_indent()
                             + out->get_verilog_name()
                             + " <= "
                             + label->get_verilog_name()
                             + ";\n");
        }

        sm_gen.add_state_process(state,
                                 step,
                                 process_str);

        if (ope->get_type() == CDFG_Operator::eType::BR) {
          // prev_stateの変更
          process_str.assign(this->_cout.output_indent()
                             + prev_state->get_verilog_name()
                             + " <= "
                             + out->get_verilog_name()
                             + ";\n");

          sm_gen.add_state_process(state,
                                   step,
                                   process_str);

          // stepの初期化
          process_str.assign(this->_cout.output_indent()
                             + step_node->get_verilog_name()
                             + " <= 0;\n");

          sm_gen.add_state_process(state,
                                   step,
                                   process_str);
        }
        break;
      }
      case CDFG_Operator::eType::RET:
        {
          auto finish_state_label = elem->get_input_at(0);

          //! @todo 返り値への対応

          // 終了状態への遷移
          process_str.append(this->_cout.output_indent()
                             + state_node->get_verilog_name()
                             + " <= "
                             + finish_state_label->get_verilog_name()
                             + ";\n"
                             + this->_cout.output_indent()
                             + step_node->get_verilog_name()
                             + " <= 0;\n");

          sm_gen.add_state_process(state,
                                   step,
                                   process_str);

          // 終了状態
          process_str.assign(this->_cout.output_indent()
                             + fin_name
                             + " <= "
                             + true_node->get_verilog_name()
                             + ";\n"
                             + this->_cout.output_indent()
                             + state_node->get_verilog_name()
                             + " <= "
                             + zero_node->get_verilog_name()
                             + ";\n");

          sm_gen.add_state_process(finish_state_label->get_parameter(),
                                   0 /* step */,
                                   process_str);

          break;
        }

    case CDFG_Operator::eType::PHI:
      {
        auto dest_node = elem->get_output_at(0);

        process_str.append(this->_cout.output_indent()
                           + dest_node->get_verilog_name()
                           + " <= w_phi_"
                           + dest_node->get_safe_name()
                           + ";\n");

        sm_gen.add_state_process(state,
                                 step,
                                 process_str);

        break;
      }

    case CDFG_Operator::eType::SWITCH:
      {
        auto condition_node = elem->get_input_at(0);
        auto default_label = elem->get_input_at(1);

        process_str.append(this->_cout.output_indent()
                           + "case ("
                           + condition_node->get_verilog_name()
                           + ")\n");

        this->_cout.indent_right();

        // case文の内部を出力
        for (auto i=2; i<elem->get_num_input(); i+=2) {
          auto val_node = elem->get_input_at(i);
          auto label_node = elem->get_input_at(i+1);

          process_str.append(this->_cout.output_indent());

          if (val_node->get_parameter() < 0) // 負の数
            process_str.append("-");

          auto asm_name = val_node->get_asm_name();
          asm_name.erase
            (std::remove(asm_name.begin(),
                         asm_name.end(),
                         '-'),
             asm_name.end());

          process_str.append(val_node->get_param_str()
                             + ": "
                             + state_node->get_verilog_name()
                             + " <= "
                             + label_node->get_verilog_name()
                             + ";\n");
        }

        // defaultの出力
        process_str.append(this->_cout.output_indent()
                           + "default: "
                           + state_node->get_verilog_name()
                           + " <= "
                           + default_label->get_verilog_name()
                           + ";\n");

        this->_cout.indent_left();

        process_str.append(this->_cout.output_indent()
                           + "endcase\n");

        // prev_stateの保存
        process_str.append(this->_cout.output_indent()
                           + prev_state->get_verilog_name()
                           + " <= "
                           + state_node->get_verilog_name()
                           + ";\n");

        // stepノードの初期化
        process_str.append(this->_cout.output_indent()
                           + step_node->get_verilog_name()
                           + " <= 0;\n");

        sm_gen.add_state_process(state,
                                 step,
                                 process_str);
        break;
      }

      //! @todo 未対応の命令に対応

    default:
      break;
    } // switch : ope type
  } // for : element list

  this->_cout.indent_left(5);

  // list(接続タイミング) ---> always
  auto state_step_list = sm_gen.get_state_step_list();
  for(auto ite_state_step = state_step_list.begin();
      ite_state_step != state_step_list.end(); ) {
    // caseの出力
    auto label_node = this->_module->get_label_node
      (ite_state_step->first);

    this->_cout << label_node->get_param_str()
                << ": //"
                << label_node->get_verilog_name()
                << "\n";

    this->_cout.indent_right();
    this->_cout << "begin\n";

    // ステップの出力
    this->_cout.indent_right();
    this->_cout << step_node->get_verilog_name() << " <= "
                << step_node->get_verilog_name() << " + 1'h1;\n";
    this->_cout << "case (" << step_node->get_verilog_name() << ")\n";

    auto range = state_step_list.equal_range(ite_state_step->first);

    for (auto ite = range.first;
         ite != range.second;
         ++ite, ++ite_state_step) {
      this->_cout.indent_right();
      this->_cout << step_node->get_bit_width()
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
