#include "CModuleGenerator.hpp"

/**
   コンストラクタ
   @brief テスト用の入力(DFG)を与えている
   @param[in] filename 出力ファイル名
*/
CModuleGenerator::CModuleGenerator
(const std::string & filename,
 const std::string & module_name)
{
  this->_module = std::make_shared<CModule>(module_name);
  this->_cout.open_ofstream(filename);

  // 基本的なノードの確保
  // 基本入出力
  auto i_clk = std::make_shared<CDFG_Wire>
    ("clk", 1, false, CDFG_Wire::eWireType::Clk);
  auto i_res = std::make_shared<CDFG_Wire>
    ("res_p", 1, false, CDFG_Wire::eWireType::Res);
  auto i_req = std::make_shared<CDFG_Wire>
    ("req_p", 1, false, CDFG_Wire::eWireType::Req);
  auto i_ce = std::make_shared<CDFG_Wire>
    ("ce_p", 1, false, CDFG_Wire::eWireType::Ce);
  auto o_fin = std::make_shared<CDFG_Reg>
    ("fin_p", 1, false, CDFG_Reg::eRegType::Fin);

  // 定数
  auto p_true = std::make_shared<CDFG_Parameter>
    ("TRUE", CDFG_Parameter::eParamType::True);
  auto p_false = std::make_shared<CDFG_Parameter>
    ("FALSE", CDFG_Parameter::eParamType::False);
  auto p_zero = std::make_shared<CDFG_Parameter>
    ("ZERO", CDFG_Parameter::eParamType::Zero);

  this->_module->add_node(i_clk);
  this->_module->add_node(i_res);
  this->_module->add_node(i_req);
  this->_module->add_node(i_ce);
  this->_module->add_node(o_fin);
  this->_module->add_node(p_true);
  this->_module->add_node(p_false);
  this->_module->add_node(p_zero);
} // CModuleGenerator

/**
   モジュールにノードを追加する
   @note FortRock 本体から呼び出すために使用
*/
void CModuleGenerator::add_node
(const std::shared_ptr<CDFG_Node> & node)
{
  this->_module->add_node(node);
} // add_node

/**
   モジュールに演算器を追加する
*/
void CModuleGenerator::add_operator
(std::shared_ptr<CDFG_Operator> & ope)
{
  this->_module->add_operator(ope);
} // add_operator

/**
   モジュールにElement(処理)を追加する
*/
void CModuleGenerator::add_element
(const std::shared_ptr<CDFG_Element> elem)
{
  this->_module->add_element(elem);
} // add_element

/**
   モジュール内のノードを検索する
   @param[in] node_name 検索対象のアセンブリ上での名前
   @note FortRock 本体から呼び出すために使用
*/
bool CModuleGenerator::find_node
(const std::string & node_name,
 const CDFG_Node::eType & type)
{
  return this->_module->find_node(node_name,
                                  type);
} // find_node (name, type)

/**
   モジュール内のノードを検索する
   @note FortRock 本体から呼び出すために使用
*/
bool CModuleGenerator::find_node
(std::shared_ptr<CDFG_Node> node)
{
  return this->_module->find_node(node->get_asm_name(),
                                  node->get_type());
} // find_node (node)

/**
   モジュール内のノードを取得する
   @param[in] node_name ノードの名前
   @param[in] type ノードの種類
   @note FortRock 本体から呼び出すために使用
*/
std::shared_ptr<CDFG_Node> &
CModuleGenerator::get_node
(const std::string & node_name,
 const CDFG_Node::eType & type)
{
  return this->_module->get_node(node_name, type);
} // get_node (name, type)

/**
   モジュール内のノードを取得
   @param[in] type ノードの種類
   @note FortRock 本体から呼び出すために使用
   (stateなどの取得に使用)
*/
std::shared_ptr<CDFG_Node> &
CModuleGenerator::get_node
(const CDFG_Node::eType & type)
{
  return this->_module->get_node(type);
} // get_node (type)

/**
   モジュール内のノードを取得
   @brief Labelを取得するために使用
   @param[in] type ラベルの種類
*/
std::shared_ptr<CDFG_Node> &
CModuleGenerator::get_node
(const CDFG_Label::eLabelType & type)
{
  return this->_module->get_node(type);
} // get_node (label)

/**
   モジュール内のノードを取得
   @brief Parameterを取得するために使用
   @param[in] type ラベルの種類
*/
std::shared_ptr<CDFG_Node> &
CModuleGenerator::get_node
(const CDFG_Parameter::eParamType & type)
{
  return this->_module->get_node(type);
} // get_node (param)

/**
   モジュール内のノードを取得
   @brief regを取得するために使用
   @param[in] type regの種類
*/
std::shared_ptr<CDFG_Node> &
CModuleGenerator::get_node
(const CDFG_Reg::eRegType & type)
{
  return this->_module->get_node(type);
} // get_node (reg)

/**
   モジュール内のノードを取得
   @brief wireを取得するために使用
   @param[in] type wireの種類
*/
std::shared_ptr<CDFG_Node> &
CModuleGenerator::get_node
(const CDFG_Wire::eWireType & type)
{
  return this->_module->get_node(type);
} // get_node (wire)

/**
   モジュール内の演算器の取得
   @note FortRock本体から取得するために使用
   @todo 取得する演算器の番号による指定など
   @attention 暫定版
*/
std::shared_ptr<CDFG_Operator> &
CModuleGenerator::get_operator
(const CDFG_Operator::eType & type)
{
  auto l = this->_module->get_operator_list();
  auto ite = std::find_if
    (l.begin(), l.end(),
     [type](std::shared_ptr<CDFG_Operator> obj) -> bool
     {
       return obj->get_type() == type;
     });

  return *ite;
} // get_operator

/**
   モジュールの最大ステップを取得
   @note step信号を定義する際にのビット幅を
   調べるためにFortRockが使用
   @return step信号に必要なビット幅
*/
unsigned
CModuleGenerator::get_max_step(void)
{
  return this->_module->get_max_step();
} // get_max_step

/**
   テストデータ(DFG)の生成
   @note テスト用
*/
void CModuleGenerator::_generate_test_data(void)
{
  // ノード確保
  // 基本入出力
  auto i_clk = std::make_shared<CDFG_Wire>
    ("clk", 1, false, CDFG_Wire::eWireType::Clk);
  auto i_res = std::make_shared<CDFG_Wire>
    ("res_p", 1, false, CDFG_Wire::eWireType::Res);
  auto i_req = std::make_shared<CDFG_Wire>
    ("req_p", 1, false, CDFG_Wire::eWireType::Req);
  auto i_ce = std::make_shared<CDFG_Wire>
    ("ce_p", 1, false, CDFG_Wire::eWireType::Ce);
  auto o_fin = std::make_shared<CDFG_Reg>
    ("fin_p", 1, false, CDFG_Reg::eRegType::Fin);

  // 入力
  auto a = std::make_shared<CDFG_Wire>
    ("i_a", 8, true, CDFG_Wire::eWireType::In_orig);
  auto b = std::make_shared<CDFG_Wire>
    ("i_b", 8, true, CDFG_Wire::eWireType::In_orig);

  // 出力
  auto out = std::make_shared<CDFG_Reg>
    ("o_out", 8, true, CDFG_Reg::eRegType::Out);

  // 定数
  std::shared_ptr<CDFG_Node> p_3 = std::make_shared<CDFG_Parameter>
    ("p_3", 8, 3L);
  std::shared_ptr<CDFG_Node> p_true = std::make_shared<CDFG_Parameter>
    ("TRUE", CDFG_Parameter::eParamType::True);
  std::shared_ptr<CDFG_Node> p_false = std::make_shared<CDFG_Parameter>
    ("FALSE", CDFG_Parameter::eParamType::False);
  std::shared_ptr<CDFG_Node> p_zero = std::make_shared<CDFG_Parameter>
    ("ZERO", CDFG_Parameter::eParamType::Zero);

  // テンプレートレジスタ
  auto t1 = std::make_shared<CDFG_Reg>
    ("t1", 8, true, CDFG_Reg::eRegType::Reg);

  // 演算器の入出力
  auto adder1_i_a = std::make_shared<CDFG_Reg>
    ("mr_adder1_i_a", 8, true, CDFG_Reg::eRegType::Reg);
  auto adder1_i_b = std::make_shared<CDFG_Reg>
    ("mr_adder1_i_b", 8, true, CDFG_Reg::eRegType::Reg);
  auto adder1_out = std::make_shared<CDFG_Wire>
    ("mw_adder1_output", 8, true, CDFG_Wire::eWireType::Wire);

  auto adder2_i_a = std::make_shared<CDFG_Reg>
    ("mr_adder2_i_a", 8, true, CDFG_Reg::eRegType::Reg);
  auto adder2_i_b = std::make_shared<CDFG_Reg>
    ("mr_adder2_i_b", 8, true, CDFG_Reg::eRegType::Reg);
  auto adder2_out = std::make_shared<CDFG_Wire>
    ("mw_adder2_output", 8, true, CDFG_Wire::eWireType::Wire);

  // システム変数
  auto s_state = std::make_shared<CDFG_Reg>
    ("r_sys_state", 8, false, CDFG_Reg::eRegType::State);
  auto s_step = std::make_shared<CDFG_Reg>
    ("r_sys_step", 8, false, CDFG_Reg::eRegType::Step);

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
  ///< @todo CLKやCEの接続を行うとElementの入出力と整合性がとれなくなる
  auto add = std::make_shared<CDFG_Operator>
    ("my_add1",
     "my_add",
     1,
     CDFG_Operator::eType::Add);

  add->add_input_port("i_a", adder1_i_a);
  add->add_input_port("i_b", adder1_i_b);
  add->add_output_port("o_output", adder1_out);
  add->add_input_port("clock", i_clk);
  add->add_input_port("ce", i_ce);

  auto sub = std::make_shared<CDFG_Operator>
    ("my_sub1",
     "my_sub",
     1,
     CDFG_Operator::eType::Sub);

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
  auto elem1
    = std::make_shared<CDFG_Element>
    (add,
     1, /* state */
     0  /* step */ );

  elem1->set_input(a, 0);
  elem1->set_input(b, 1);
  elem1->set_output(t1, 0);

  this->_module->add_element(elem1);

  // out = t1 + p_3
  auto elem2
    = std::make_shared<CDFG_Element>
    (sub,
     1, /* state */
     3  /* step */);

  elem2->set_input(t1, 0);
  elem2->set_input(p_3, 1);
  elem2->set_output(out, 0);

  this->_module->add_element(elem2);
} // _generate_test_data

/**
   ステートマシン回路をファイルへ出力
*/
int CModuleGenerator::generate(void)
{
  // スケジューリング
  CDFG_Scheduler scheduler(this->_module);
  scheduler.do_schedule();

  // 最適化
  CDFG_Optimizer optimizer(this->_module);
  //  optimizer.do_optimize();

  // step信号のビット幅の修正
  std::dynamic_pointer_cast<CDFG_Reg>
    (this->get_node(CDFG_Reg::eRegType::Step))
    ->set_bit_width(FortRock::get_required_bit_width
                    (this->get_max_step()));

  this->_generate_header();
  this->_generate_define();
  this->_generate_define_array();
  this->_generate_assign();
  this->_generate_function();
  this->_generate_calculator();
  this->_generate_always();
  this->_generate_footer();

  return 0;
} // generate

/**
   moduleの入出力定義部の出力
*/
void CModuleGenerator::_generate_header(void)
{
  std::list<std::string> io_list;
  this->_cout << "`default_nettype none\n\n"
              << "module "
              << this->_module->get_name()
              << std::endl;

  this->_cout.indent_right();

  this->_cout << "(\n";

  // 入出力信号の出力
  for (auto & io : this->_module->get_node_list())
    {
      std::string io_str;

      switch (io->get_type()) {
      case CDFG_Node::eType::Wire:
        if (io->is_input())
          io_str = "input wire ";
        else
          continue;
        break;

      case CDFG_Node::eType::Reg:
        if (io->is_output())
          io_str = "output reg ";
        else
          continue;
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
       )
    {
      this->_cout << (*ite);

      if ((++ite) != io_list.end())
        this->_cout <<= ',';

      this->_cout << "\n";
    }

  this->_cout << ");" << std::endl;
} // _generate_header

/**
   moduleの信号宣言部の出力
*/
void CModuleGenerator::_generate_define(void)
{
  unsigned type;
  std::string types[3] = {"reg", "wire", "parameter"};
  std::array<std::stringstream, 3> streams;

  const auto reg   = 0;
  const auto wire  = 1;
  const auto param = 2;
  const auto none  = 3;

  for (auto & node : this->_module->get_node_list())
    {
      switch(node->get_type()) {
      case CDFG_Node::eType::Reg:
        type = reg;
        break;

      case CDFG_Node::eType::Wire:
        type = wire;
        break;

      case CDFG_Node::eType::Label:
      case CDFG_Node::eType::Param:
        type = param;
        break;

      default:
        continue;
      }

      // 入出力信号は再定義しない
      if (node->is_input() || node->is_output())
        continue;

      streams[type] << this->_cout.output_indent()
                    << types[type]
                    << ' ';

      if (node->get_is_signed())
        streams[type] << "signed ";

      if (node->get_bit_width() > 1)
        streams[type] << "["
                      << node->get_bit_width() - 1
                      << ":0] ";

      streams[type] << node->get_verilog_name();

      if (type == param)
        streams[param] << " = "
                       << node->to_string();

      streams[type] << ";\n";
    } // for : node

  for (auto & s : streams)
    if (s.rdbuf()->in_avail() != 0)
      this->_cout <<= s.str() + "\n";
} // _generate_define

/**
   配列の宣言部の出力
*/
void CModuleGenerator::_generate_define_array(void)
{
  for (auto & node : this->_module->get_node_list())
    {
      if (node->get_type() != CDFG_Node::eType::Mem)
        continue;

      auto type = std::dynamic_pointer_cast<CDFG_Mem>
        (node)->get_mem_type();

      switch (type) {
      case CDFG_Mem::eMemType::Array:
        {
          auto array = std::dynamic_pointer_cast<CDFG_Array>(node);
          this->_cout << array->define_string() << std::endl;
          break;
        }

      case CDFG_Mem::eMemType::Ram:
        {
          auto ram = std::dynamic_pointer_cast<CDFG_Ram>(node);
          this->_cout << ram->define_string() << std::endl;
          break;
        }

      case CDFG_Mem::eMemType::Other:
      default:
        break;
      } // switch : type
    } // for : node
} // _generate_define_array

void CModuleGenerator::_generate_assign(void)
{
  for (auto & elem : this->_module->get_element_list())
    {
      if (elem->get_operator()->get_type()
          == CDFG_Operator::eType::Phi) {
        auto phi = std::dynamic_pointer_cast<CDFG_PhiElem>(elem);

        this->_cout << "wire ["
                    << phi->get_output_at(0)->get_bit_width() - 1
                    << ":0] "
                    << phi->output_from_str()
                    << ";\n";

        this->_cout << "assign "
                    << phi->output_from_str()
                    << " = phi_"
                    << phi->output_to_str()
                    << "("
                    << phi->get_prev_state()->get_verilog_name();

        for (auto i=0;
             i < phi->get_num_input();
             ++i)
          this->_cout <<= ", " + phi->input_from_str(i);

        this->_cout <<= ");\n\n";
      } // if : PHI
    } // for : elem
} // _generate_assign

/**
   module の function 文の定義
   @note PHI命令の実装に用いる
   @attention 少なくとも2つ以上の条件が存在すると仮定
   @todo 同一の定数が使用される場合に対応
*/
void CModuleGenerator::_generate_function(void)
{
  auto prev_state =
    this->get_node(CDFG_Reg::eRegType::Prev_state);

  for (auto & elem : this->_module->get_element_list())
    {
      auto ope = elem->get_operator();

      // PHI 命令以外は無視
      if (ope->get_type() != CDFG_Operator::eType::Phi)
        continue;

      auto phi = std::dynamic_pointer_cast<CDFG_PhiElem>
        (elem);

      this->_cout << "function ["
                  << phi->get_output_at(0)->get_bit_width() - 1
                  << ":0] phi_"
                  << phi->output_to_str()
                  << ";\n";

      this->_cout.indent_right();

      auto prev_state = phi->get_prev_state();

      this->_cout << "input ["
                  << prev_state->get_bit_width() - 1
                  << ":0] "
                  << prev_state->get_verilog_name()
                  << ";\n";

      for (auto i=0; i < elem->get_num_input();
           ++i)
        this->_cout << "input ["
                    << phi->get_input_at(i)->get_bit_width() - 1
                    << ":0] "
                    << phi->input_from_str(i)
                    << ";\n";

      // case文の出力
      {
        this->_cout << "case ("
                    << prev_state->get_verilog_name()
                    << ")\n";

        this->_cout.indent_right();

        // 条件による代入文
        for (auto i = 0; i < ope->get_num_input();
             ++i)
          this->_cout << phi->get_label_str(i)
                      << ": "
                      << phi->input_to_str(i)
                      << " = "
                      << phi->input_from_str(i)
                      << ";\n";

        auto zero_node = this->get_node
          (CDFG_Parameter::eParamType::Zero);

        this->_cout << "default: phi_"
                    << phi->output_to_str()
                    << " = "
                    << zero_node->get_verilog_name()
                    << ";\n";

        this->_cout.indent_left();

        this->_cout << "endcase\n";

        this->_cout.indent_left();
      }

      this->_cout << "endfunction\n\n";
    } // for : elem
} // _generate_function

/**
   moduleの演算器定義の出力
*/
void CModuleGenerator::_generate_calculator(void)
{
  // 各演算器の入出力信号の接続
  for (auto & module : this->_module->get_operator_list())
    {
      this->_cout << module->get_mod_name()
                  << ' '
                  << module->get_name()
                  << "(\n";

      this->_cout.indent_right();

      // 入力の接続
      unsigned at = 0;
      for (at = 0;
           at < module->get_num_input();
           ++at)
        this->_cout << '.'
                    << module->get_input_signal_at(at)
                    << '('
                    << module->get_input_node_at(at)->get_verilog_name()
                    << "),"
                    << std::endl;

      // 出力の接続

      for (at = 0;
           at < module->get_num_output() - 1;
           ++at)
        this->_cout << '.'
                    << module->get_output_signal_at(at)
                    << '('
                    << module->get_output_node_at(at)->get_verilog_name()
                    << "),"
                    << std::endl;

      // 最後の出力の接続
      this->_cout << '.'
                  << module->get_output_signal_at(at)
                  << '('
                  << module->get_output_node_at(at)->get_verilog_name()
                  << ')'
                  << std::endl;

      this->_cout.indent_left();

      this->_cout << ");\n"
                  << std::endl;
    }
} // _generate_calculator

/**
   moduleのステートマシンの出力
*/
void CModuleGenerator::_generate_always(void)
{
  // 出力に必要な情報(信号名など)を取得
  auto clk_name   = this->get_node(CDFG_Wire::eWireType::Clk)->get_verilog_name();
  auto res_name   = this->get_node(CDFG_Wire::eWireType::Res)->get_verilog_name();
  auto req_name   = this->get_node(CDFG_Wire::eWireType::Req)->get_verilog_name();
  auto ce_name    = this->get_node(CDFG_Wire::eWireType::Ce)->get_verilog_name();
  auto state_node = this->get_node(CDFG_Reg::eRegType::State);
  auto prev_state = this->get_node(CDFG_Reg::eRegType::Prev_state);
  auto step_node  = this->get_node(CDFG_Reg::eRegType::Step);
  auto fin_name   = this->get_node(CDFG_Reg::eRegType::Fin)->get_verilog_name();
  auto true_node  = this->get_node(CDFG_Parameter::eParamType::True);
  auto false_node = this->get_node(CDFG_Parameter::eParamType::False);
  auto zero_node  = this->get_node(CDFG_Parameter::eParamType::Zero);
  auto out_node   = this->get_node(CDFG_Reg::eRegType::Out);

  this->_cout << "always @(posedge "
              << clk_name
              << ')'
              << std::endl;

  // リセットの出力
  this->_cout.indent_right();

  this->_cout << "begin"
              << std::endl;

  this->_cout.indent_right();

  this->_cout << "if ("
              << res_name
              << " ==  "
              << true_node->get_verilog_name()
              << ")\n";

  this->_cout.indent_right();

  this->_cout << "begin\n";

  this->_cout.indent_right();

  this->_cout << fin_name
              << " <= "
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
  this->_cout << "case ("
              << state_node->get_verilog_name()
              <<  ")\n";

  this->_cout.indent_right();

  this->_cout << state_node->get_bit_width()
              << "'h0:\n";

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

  this->_cout << out_node->get_verilog_name()
              << " <= "
              << zero_node->get_verilog_name()
              << ";\n";

  this->_cout << "if ("
              << req_name
              << ")\n";

  this->_cout.indent_right();

  this->_cout << "begin\n";

  this->_cout.indent_right();

  // if文の中身 (リセット)
  this->_cout << state_node->get_verilog_name()
              << " <= "
              << state_node->get_bit_width()
              << "'h1;\n";

  this->_cout << prev_state->get_verilog_name()
              << " <= "
              << zero_node->get_verilog_name()
              << ";\n";

  // 入力のコピー
  for (auto & elem : this->_module->get_element_list())
    {
      auto ope = elem->get_operator();

      if (ope->get_type() == CDFG_Operator::eType::Copy)
        {
          auto orig_node = elem->get_input_at(0);
          auto copy_node = elem->get_output_at(0);
          this->_cout << copy_node->get_verilog_name()
                      << " <= "
                      << orig_node->get_verilog_name()
                      << ";\n";
        }
    } // for : elem

  // 配列の初期化
  //! @todo 配列以外の対応
  for (auto & node : this->_module->get_node_list())
    if (node->get_type() == CDFG_Node::eType::Mem)
      {
        auto mem
          = std::dynamic_pointer_cast<CDFG_Mem>(node);
        this->_cout
          <<= mem->init_string(this->_cout.output_indent());
      }

  this->_cout.indent_left();

  this->_cout << "end\n";

  this->_cout.indent_left(2);

  this->_cout << "end\n";

  // DFG ---> list(接続タイミング)
  this->_cout.indent_right(4);
  CStateMachineGen sm_gen;

  // ステートマシンの出力
  for (auto & elem : this->_module->get_element_list())
    {
      auto ope     = elem->get_operator();
      auto state   = elem->get_state();
      auto step    = elem->get_step();
      auto latency = ope->get_latency();
      auto indent  = this->_cout.output_indent();
      std::string process_str ("");

      switch (ope->get_type()) {
        // 2入力1出力の普通の命令
      case CDFG_Operator::eType::Add:
      case CDFG_Operator::eType::Fadd:
      case CDFG_Operator::eType::Sub:
      case CDFG_Operator::eType::Fsub:
      case CDFG_Operator::eType::Add_sub:
      case CDFG_Operator::eType::Fdiv:
      case CDFG_Operator::eType::Sdiv:
        //      case CDFG_Operator::eType::Func:
      case CDFG_Operator::eType::Mul:
      case CDFG_Operator::eType::Fmul:
      case CDFG_Operator::eType::Srem:
        {
          // 入力の接続
          for (auto i=0; i<ope->get_num_input(); ++i)
            {
              auto node = ope->get_input_node_at(i);

              // clk, res, req, ceの入力の回避
              if (node->get_type() == CDFG_Node::eType::Reg)
                process_str.append
                  (indent
                   + elem->input_to_str(i)
                   + " <= "
                   + elem->input_from_str(i)
                   + ";\n");

            } // for : i

          sm_gen.add_state_process(state, step, process_str);

          // 出力の接続
          process_str.assign
            (indent
             + elem->output_to_str()
             + " <= "
             + elem->output_from_str()
             + ";\n");

          sm_gen.add_state_process
            (state,
             step + latency + 1,
             process_str);
          break;
        }

      case CDFG_Operator::eType::Load:
        {
          auto load = std::dynamic_pointer_cast<CDFG_LoadElem>(elem);

          // メモリアクセス制御
          if (load->is_mem_load())
            sm_gen.add_state_process
              (state, step,
               load->other_str(indent));

          // 出力の代入
          sm_gen.add_state_process
            (state, step + load->get_operator()->get_latency(),
             indent
             + load->output_to_str()
             + " <= "
             + load->output_from_str()
             + ";\n");

          break;
        } // Load

      case CDFG_Operator::eType::Store:
        {
          auto store = std::dynamic_pointer_cast<CDFG_StoreElem>(elem);

          // メモリのアクセス制御の出力
          if (store->is_mem_store())
            sm_gen.add_state_process
              (state, step, store->other_str(indent));

          // 入出力
          sm_gen.add_state_process
            (state, step,
             indent
             + store->output_to_str()
             + " <= "
             + store->input_from_str()
             + ";\n");
          break;
        } // Store

      case CDFG_Operator::eType::Icmp:
        {
          sm_gen.add_state_process
            (state, step,
             indent
             + elem->output_to_str()
             + " <= "
             + elem->output_from_str()
             + ";\n");
          break;
        } // Icmp

      case CDFG_Operator::eType::Fcmp:
        {
          // IPコアに入力を接続
          for (auto i=0; i<2; ++i)
            sm_gen.add_state_process
              (state, step,
               indent
               + elem->input_to_str(i)
               + " <= "
               + elem->input_from_str(i)
               + ";\n");

          // IPコアの出力を接続
          sm_gen.add_state_process
            (state, step + latency + 1,
             indent
             + elem->output_to_str()
             + " <= "
             + elem->output_from_str()
             + ";\n");

          break;
        } // Fcmp

    case CDFG_Operator::eType::Select:
      {
        sm_gen.add_state_process
          (state, step,
           indent + elem->output_to_str()
           + " <= "
           + elem->output_from_str()
           + ";\n");
      } // Select

    case CDFG_Operator::eType::Br:
      {
        sm_gen.add_state_process
          (state, step, elem->other_str(indent));
        break;
      } // Br

    case CDFG_Operator::eType::Ret:
      {
        auto ret = std::dynamic_pointer_cast<CDFG_RetElem>
          (elem);

        // 終了状態への遷移
        sm_gen.add_state_process
          (state, step, ret->other_str(indent));

        // 終了ステートの処理
        sm_gen.add_state_process
          (ret->get_finish_state(),
           0 /* step */,
           ret->finish_state_str(indent));
        break;
      } // Ret

    case CDFG_Operator::eType::Phi:
      {
        sm_gen.add_state_process
          (state, step,
           indent
           + elem->output_to_str()
           + " <= "
           + elem->output_from_str()
           + ";\n");
        break;
      } // Phi

    case CDFG_Operator::eType::Switch:
      {
        auto sw_inst = std::dynamic_pointer_cast<CDFG_SwitchElem>
          (elem);

        // case文の条件部を出力
        process_str.append
          (indent
           + "case ("
           + sw_inst->get_condition_code()
           + ")\n");

        this->_cout.indent_right();

        // case文の内部(分岐)を出力
        for (auto i=0; i<sw_inst->get_num_input(); ++i)
          {
            process_str.append
              (this->_cout.output_indent()
               + sw_inst->get_condition_at(i)
               + ": "
               + sw_inst->output_to_str()
               + " <= "
               + sw_inst->input_from_str(i)
               + ";\n");
          } // for : i

        // defaultの出力
        process_str.append
          (this->_cout.output_indent()
           + "default: "
           + sw_inst->output_to_str()
           + " <= "
           + sw_inst->get_default_label()
           + ";\n");

        // endcaseの出力
        this->_cout.indent_left();
        process_str.append
          (this->_cout.output_indent()
           + "endcase\n");

        // prev_stateの更新, stepの初期化
        process_str.append
          (sw_inst->other_str(indent));

        sm_gen.add_state_process
          (state, step, process_str);
        break;
      } // Switch

    case CDFG_Operator::eType::Lshiftl:
    case CDFG_Operator::eType::Lshiftr:
    case CDFG_Operator::eType::Ashiftr:
      {
        sm_gen.add_state_process
          (state, step,
           indent
           + elem->output_to_str()
           + " <= "
           + elem->output_from_str()
           + ";\n");
        break;
      } // Lshiftl, Lshiftr, Ashiftr

    case CDFG_Operator::eType::And:
    case CDFG_Operator::eType::Or:
    case CDFG_Operator::eType::Xor:
      {
        sm_gen.add_state_process
          (state, step,
           indent
           + elem->output_to_str()
           + elem->output_from_str()
           + ";\n");
        break;
      } // And, Or, Xor

    case CDFG_Operator::eType::Trunc:
      {
        sm_gen.add_state_process
          (state, step,
           indent
           + elem->output_to_str()
           + " <= "
           + elem->input_from_str()
           + ";\n");
        break;
      } // Trunc

    case CDFG_Operator::eType::Zext:
      {
        sm_gen.add_state_process
          (state, step,
           indent
           + elem->output_to_str()
           + " <= "
           + elem->output_from_str()
           + ";\n");
        break;
      } // Zext

    default:
      break;
    } // switch : ope type
  } // for : element list

  this->_cout.indent_left(5);

  // list(接続タイミング) --> always
  auto state_step_list = sm_gen.get_state_step_list();
  for(auto ite_state_step = state_step_list.begin();
      ite_state_step != state_step_list.end(); )
    {
      // caseの出力
      auto label_node
        = this->_module->get_label_node
        (ite_state_step->first);

      this->_cout << label_node->to_string()
                  << ": //"
                  << label_node->get_verilog_name()
                  << "\n";

      this->_cout.indent_right();

      this->_cout << "begin\n";

      // ステップの出力
      this->_cout.indent_right();

      this->_cout << step_node->get_verilog_name()
                  << " <= "
                  << step_node->get_verilog_name()
                  << " + 1'h1;\n";
      this->_cout << "case ("
                  << step_node->get_verilog_name()
                  << ")\n";

      auto range
        = state_step_list.equal_range
        (ite_state_step->first);

      for (auto ite = range.first;
           ite != range.second;
           ++ite, ++ite_state_step)
        {
          this->_cout.indent_right();

          this->_cout << step_node->get_bit_width()
                      << "'h"
                      << std::hex
                      << ite->second << ":\n";

          this->_cout.indent_right();

          this->_cout << "begin\n";

          this->_cout.indent_right();

          this->_cout  <<= sm_gen.gen_state_machine
            (ite->first, ite->second);

          this->_cout.indent_left();

          this->_cout << "end\n";

          this->_cout.indent_left(2);
        } // for : ite

      this->_cout << "endcase\n";
      this->_cout.indent_left();
      this->_cout << "end\n";
      this->_cout.indent_left();
    } // for : ite_state_step

  this->_cout.indent_left();
  this->_cout << "endcase\n";
  this->_cout.indent_left();
  this->_cout << "end\n";
  this->_cout.indent_left(2);
  this->_cout << "end\n";
  this->_cout.indent_left();
} // _generate_always

/**
   module定義の尾部の出力
   @note デバッグ用データの出力などに利用
*/
void CModuleGenerator::_generate_footer(void)
{
  this->_cout.indent_left();
  this->_cout << "endmodule\n\n"
              << "`default_nettype wire"
              << std::endl;
} // _generate_footer
