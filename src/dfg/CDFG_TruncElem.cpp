#include "CDFG_TruncElem.hpp"

/**
   コンストラクタ
   @param[in] bit_width 変換後のビット幅
   @param[in] state 命令を実行するステート
   @param[in] step 命令を実行するステップ
*/
CDFG_TruncElem::CDFG_TruncElem
(const unsigned & bit_width,
 const unsigned & state,
 const unsigned & step)
  : _bit_width(bit_width),
    CDFG_Element(CDFG_Operator::eType::Trunc,
                 1, // num input
                 state,
                 step)
{}

/**
   入力元のコード(Verilog HDL)の取得
   @param[in] at don't care
   @return 入力部のコード(Verilog HDL)
 */
std::string
CDFG_TruncElem::input_from_str
(const unsigned & at)
{
  return this->_get_input_str(0);
}

/**
   入力先のコード(Verilog HDL)の取得
   @param[in] at don't care
   @return 入力先のコード(Verilog HDL)
 */
std::string
CDFG_TruncElem::input_to_str
(const unsigned & at)
{
  return this->output_to_str();
}

/**
   出力元のコード(Verilog HDL)の取得
   @return 出力元のコード(Verilog HDL)
 */
std::string
CDFG_TruncElem::output_from_str
(void)
{
  return this->input_from_str(0)
    + "["
    + std::to_string(this->get_dest_bit_width()-1)
    + ":0]";
}

/**
   出力先のコード(Verilog HDL)の取得
   @return 出力先のコード(Verilog HDL)
 */
std::string
CDFG_TruncElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
}
