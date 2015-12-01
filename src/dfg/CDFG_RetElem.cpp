#include "CDFG_RetElem.hpp"

/**
   コンストラクタ
   @brief 終了ステートの参照を設定
   @param[in] finish_state 終了ステートラベルの参照
   @param[in] state_node stateノードの参照
   @param[in] step_node stepノードの参照
   @param[in] prev_state prev_stateノードの参照
   @param[in] state 命令実行開始ステート
   @param[in] step 命令実行開始ステップ
 */
CDFG_RetElem::CDFG_RetElem
(const std::shared_ptr<CDFG_Node> & finish_state,
 const std::shared_ptr<CDFG_Node> & state_node,
 const std::shared_ptr<CDFG_Node> & step_node,
 const std::shared_ptr<CDFG_Node> & prev_state,
 const std::shared_ptr<CDFG_Node> & fin_signal,
 const std::shared_ptr<CDFG_Node> & true_signal,
 const unsigned & state,
 const unsigned & step)
  : _finish_state(std::dynamic_pointer_cast<CDFG_Label>(finish_state)),
    _state_node(state_node),
    _step_node(step_node),
    _prev_state(prev_state),
    _fin_signal(fin_signal),
    _true_signal(true_signal),
    CDFG_Element(CDFG_Operator::eType::Ret,
                 0,
                 state, step)
{} // CDFG_RetElem

/**
   終了状態のコード(Verilog HDL)の取得
   @param[in] indent インデント
   @return 終了状態のコード(Verilog HDL)
 */
std::string
CDFG_RetElem::finish_state_str
(const std::string & indent)
{
  return indent
    + this->_fin_signal->get_verilog_name()
    + " <= "
    + this->_true_signal->get_verilog_name()
    + ";\n"
    + indent
    + this->_state_node->get_verilog_name()
    + " <= 0;\n";
} // finish_state_str

/**
   終了ステート番号を取得
   @return 終了ステート番号
 */
unsigned
CDFG_RetElem::get_finish_state
(void)
{
  return this->_finish_state->get_state();
} // get_finish_state

/**
   終了ステートへの遷移命令のコード(Verilog HDL)の取得
   @param[in] inent インデント
   @return 終了ステートへの遷移命令のコード(Verilog HDL)
 */
std::string
CDFG_RetElem::other_str
(const std::string & indent)
{
  return indent
    + this->_state_node->get_verilog_name()
    + " <= "
    + this->_finish_state->get_verilog_name()
    + ";\n"
    + indent
    + this->_step_node->get_verilog_name()
    + " <= 0;\n";
} // other_str
