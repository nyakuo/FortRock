#include "CDFG_PhiElem.hpp"

/**
   コンストラクタ
   @param[in] num_input 入力の数
   @param[in] state 実行開始ステート
   @param[in] step 実行開始ステップ
 */
CDFG_PhiElem::CDFG_PhiElem
(const unsigned & num_input,
 const std::shared_ptr<CDFG_Node> prev_state,
 const unsigned & state,
 const unsigned & step)
  : _prev_state(prev_state),
    CDFG_Element(CDFG_Operator::eType::Phi,
                 num_input,
                 state,
                 step)
{
  this->_labels.resize(num_input);
}

/**
   phi命令の条件ラベルの設定
   @param[in] node ラベルの参照
   @param[in] at 設定する位置
 */
void
CDFG_PhiElem::set_label
(const std::shared_ptr<CDFG_Node> & node,
 const unsigned & at)
{
  this->_labels[at] = node;
} // set_label

/**
   prev_stateノードの参照を取得
   @return prev_stateノードの参照
 */
const std::shared_ptr<CDFG_Node> &
CDFG_PhiElem::get_prev_state
(void) const
{
  return this->_prev_state;
} // get_prev_state

/**
   遷移先ラベル名の取得
   @param[in] at 取得する遷移先ラベルの位置
   @return 遷移先ラベル名
 */
const std::string
CDFG_PhiElem::get_label_str
(const unsigned & at) const
{
  return this->_labels[at]->get_verilog_name();
} // get_label_str

/**
   入力部のコード(Verilog HDL)を取得する
   @param[in] at 取得する入力の位置
   @return 入力部のコード(Verilog HDL)
 */
std::string
CDFG_PhiElem::input_from_str
(const unsigned & at)
{
  return this->_get_input_str(at);
} // input_from_str

/**
   値の代入先の取得
   @param[in] at don't care
 */
std::string
CDFG_PhiElem::input_to_str
(const unsigned & at)
{
  return "phi_" + this->get_output_at(0)->get_verilog_name();
} // input_to_str

/**
   出力先のレジスタ名(Verilog HDL)の取得
   @return 出力先のレジスタ名(Verilog HDL)
 */
std::string
CDFG_PhiElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
} // output_to_str

/**
   出力元のコード(Verilog HDL)の取得
   @brief functionにassignされたwire
   @return 出力元のコード(Verilog HDL)
 */
std::string
CDFG_PhiElem::output_from_str
(void)
{
  return "w_phi_" + this->get_output_at(0)->get_verilog_name();
} // output_from_str
