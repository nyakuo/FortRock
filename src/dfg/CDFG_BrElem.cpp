#include "CDFG_BrElem.hpp"
/**
   コンストラクタ
   @param[in] state 命令実行開始ステート
   @param[in] step 命令実行開始ステップ
 */
CDFG_BrElem::CDFG_BrElem
(const std::shared_ptr<CDFG_Node> & prev_state,
 const std::shared_ptr<CDFG_Node> & step_node,
 const unsigned & state,
 const unsigned & step)
  : _prev_state(prev_state),
    _step_node(step_node),
    CDFG_Element(CDFG_Operator::eType::Br,
                 0, state, step)
{} // CDFG_BrElem

/**
   条件ノードの設定
   @param[in] tf 条件ノードの参照 (True or False)
 */
void
CDFG_BrElem::set_condition
(const std::shared_ptr<CDFG_Node> & tf)
{
  this->_tf_node = tf;
} // set_condition

/**
   状態遷移処理のコード(Verilog HDL)の取得
   @param[in] indent インデント
   @return 状態遷移処理のコード(Verilog HDL)
   @note input(0) : 無条件分岐及び条件付き分岐の遷移先1
         input(1) : 条件付き分岐の分岐先2
 */
std::string
CDFG_BrElem::other_str
(const std::string & indent)
{
  std::string ret_str("");

  auto out = this->get_output_at(0)->get_verilog_name();

  // 条件付き分岐の場合
  if (this->get_num_input() == 2)
      ret_str.assign
        (indent
         + out + " <= ("
         + this->_get_input_str(this->_tf_node)
         + " ) ? "
         + this->get_input_at(0)->get_verilog_name()
         + " : "
         + this->get_input_at(1)->get_verilog_name()
         + ";\n");
  // 無条件分岐の場合
  else
      ret_str.assign
        (indent
         + out + " <= "
         + this->get_input_at(0)->get_verilog_name()
         + ";\n");

  // prev_stateの変更
  ret_str.append
    (indent + this->_prev_state->get_verilog_name()
      + " <= " + out + ";\n");

  // stepの初期化
  ret_str.append
    (indent + this->_step_node->get_verilog_name()
     + " <= 0;\n");

  return ret_str;
} // other_str
