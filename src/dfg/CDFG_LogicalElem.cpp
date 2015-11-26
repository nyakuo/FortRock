#include "CDFG_LogicalElem.hpp"

/**
   コンストラクタ
   @param[in] type 演算の種類
   @param[in] state 命令の実行開始ステート
   @param[in] step 命令の実行開始ステップ
 */
CDFG_LogicalElem::CDFG_LogicalElem
(const CDFG_Operator::eType & type,
 const unsigned & state,
 const unsigned & step)
  : CDFG_Element(type,
                 2, // num input
                 state,
                 step)
{}

/**
   入力元のコード(Verilog HDL)の取得
   @param[in] at 取得する入力の位置 (0 or 1)
   @return 入力元のコード(Verilog HDL)
 */
std::string
CDFG_LogicalElem::input_from_str
(const unsigned & at)
{
  return this->_get_input_str(at);
} // input_from_str

/**
   入力先のコード(Verilog HDL)の取得
   @param[in] at don't care
   @return 入力先のコード(Verilog HDL)
 */
std::string
CDFG_LogicalElem::input_to_str
(const unsigned & at)
{
  return this->output_to_str();
} // input_to_str

/**
   出力元のコード(Verilog HDL)の取得
   @return 出力元のコード(Verilog HDL)
 */
std::string
CDFG_LogicalElem::output_from_str
(void)
{
  return "(" + this->input_from_str(0)
    + this->_get_logical_str()
    + this->input_from_str(1) + ")";
} // output_from_str

/**
   出力先のコード(Verilog HDL)の取得
   @return 出力先のコード(Verilog HDL)
 */
std::string
CDFG_LogicalElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
} // output_to_str

/**
   論理演算のコード(Verilog HDL)を取得する
   @return 論理演算のコード(Verilog HDL)
 */
std::string
CDFG_LogicalElem::_get_logical_str
(void)
{
  switch (this->get_operator()->get_type())
    {
    case CDFG_Operator::eType::And:
      return " & ";
    case CDFG_Operator::eType::Or:
      return " | ";
    case CDFG_Operator::eType::Xor:
      return " ^ ";
    default:;
      return "";
    }
} // _get_logical_str
