#include "CDFG_SwitchElem.hpp"

/**
   コンストラクタ
  @param[in] num_input 入力の数
  @param[in] state_node stateノードの参照
  @param[in] prev_state prev-stateノードの参照
  @param[in] step_node stepノードの参照
  @param[in] default_label default遷移先の参照
  @param[in] cond_node 遷移条件の参照
  @param[in] state 実行開始ステート
  @param[in] step 実行開始ステップ
 */
CDFG_SwitchElem::CDFG_SwitchElem
(const unsigned & num_input,
 const std::shared_ptr<CDFG_Node> & state_node,
 const std::shared_ptr<CDFG_Node> & prev_state,
 const std::shared_ptr<CDFG_Node> & step_node,
 const std::shared_ptr<CDFG_Node> & default_label,
 const std::shared_ptr<CDFG_Node> & cond_node,
 const unsigned & state,
 const unsigned & step)
  : _state_node(state_node),
    _prev_state(prev_state),
    _step_node(step_node),
    _default_label(default_label),
    _condition_node(cond_node),
    CDFG_Element(CDFG_Operator::eType::Switch,
                 num_input,
                 state,
                 step)
{} // CDFG_SwitchElem

/**
   遷移条件と遷移先の設定
   @param[in] at リスト内の挿入位置
   @param[in] cond 条件定数
   @param[in] to 遷移先のラベルの参照
 */
void
CDFG_SwitchElem::set_condition_at
(const std::shared_ptr<CDFG_Node> & cond,
 const unsigned & at)
{
  this->_conditions[at] = cond;
} // set_condition_at

/**
   case文の条件コード(Verilog HDL)を取得
   @return case文の条件コード(Verilog HDL)
 */
std::string
CDFG_SwitchElem::get_condition_code
(void)
{
  return this->_get_input_str(this->_condition_node);
} // get_condition_code

/**
   遷移条件の取得
   @param[in] at 取得する条件の位置
   @return 遷移条件
 */
std::string
CDFG_SwitchElem::get_condition_at
(const unsigned & at)
{
  return this->_conditions[at]->get_verilog_name();
} // get_condition_at

/**
   default_labelのコード(Verilog HDL)を取得
   @return default_labelのコード(Verilog HDL)
 */
std::string
CDFG_SwitchElem::get_default_label
(void)
{
  return this->_default_label->get_verilog_name();
} // get_default_label

/**
   入力元のコード(Verilog HDL)の取得
   @param[in] at 取得する入力の位置
   @return 入力元のコード(Verilog HDL)
 */
std::string
CDFG_SwitchElem::input_from_str
(const unsigned & at)
{
  return this->get_input_at(at)->get_verilog_name();
} // input_from_str

/**
   入力先のコード(Verilog HDL)の取得
   @param[in] at don't care
   @return 入力先のコード(Verilog HDL)
 */
std::string
CDFG_SwitchElem::input_to_str
(const unsigned & at)
{
  return this->output_to_str();
} // input_to_str

/**
   出力先のコード(Verilog HDL)の取得
   @return 出力先のコード(Verilog HDL)
 */
std::string
CDFG_SwitchElem::output_to_str
(void)
{
  return this->_state_node->get_verilog_name();
} // output_to_str

/**
   prev_stateの更新及びstepの初期化コード(Verilog HDL)の取得
   @param[in] indent インデント
   @return prev_stateの更新及びstepの初期化コード(Verilog HDL)
 */
std::string
CDFG_SwitchElem::other_str
(const std::string & indent)
{
  return indent
    + this->_prev_state->get_verilog_name()
    + " <= "
    + this->_state_node->get_verilog_name()
    + ";\n"

    + indent
    + this->_step_node->get_verilog_name()
    + " <= 0;\n";
} // other_str
