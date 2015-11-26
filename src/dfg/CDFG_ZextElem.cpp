#include "CDFG_ZextElem.hpp"

/**
   コンストラクタ
   @param[in] state 命令実行開始ステート
   @param[in] step 命令実行開始ステップ
 */
CDFG_ZextElem::CDFG_ZextElem
(const unsigned & state,
 const unsigned & step)
  : CDFG_Element(CDFG_Operator::eType::Zext,
                 1, // num input
                 state, step)
{} // CDFG_ZextElem

/**
   命令の入力元のコード(Verilog HDL)の出力
   @parma[in] at don't care
   @return 命令の入力部のコード(Verilog HDL)
 */
std::string
CDFG_ZextElem::input_from_str
(const unsigned & at)
{
  return this->_get_input_str(at);
} // input_from_str

/**
   命令の出力元のコード(Verilog HDL)の出力
   @return 命令の出力部のコード(Verilog HDL)
 */
std::string
CDFG_ZextElem::output_from_str
(void)
{
  return "("
    + this->get_input_at(0)->get_verilog_name()
    + "| 0)";
} // output_from_str

/**
   命令の出力先のコード(Verilog HDL)の取得
   @return 命令の出力先のコード(Verilog HDL)
 */
std::string
CDFG_ZextElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
} // output_to_str
