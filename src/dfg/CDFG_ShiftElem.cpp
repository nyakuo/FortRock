#include "CDFG_ShiftElem.hpp"

/**
   コンストラクタ
   @param[in] type シフト演算の種類
   @param[in] state 命令の実行開始ステート
   @param[in] step 命令の実行開始ステップ
 */
CDFG_ShiftElem::CDFG_ShiftElem
(const CDFG_Operator::eType & type,
 const unsigned & state,
 const unsigned & step)
  : CDFG_Element(type,
                 2, // num input
                 state, step)
{}

/**
   シフト命令のコード(Verilog HDL)の取得
   @return シフト命令のコード(Verilog HDL)
 */
std::string
CDFG_ShiftElem::_get_shift_str
(void)
{
  switch (this->get_operator()->get_type())
    {
    case CDFG_Operator::eType::Lshiftl:
      return  " << ";

    case CDFG_Operator::eType::Lshiftr:
      return " >> ";

    case CDFG_Operator::eType::Ashiftr:
      return " >>> ";

    default:
      return"";
    }
} // _get_shift_str

/**
   入力元のコード(Verilog HDL)の取得
   @param[in] at 取得する入力の位置 (0 or 1)
   @return 入力元のコード(Verilog HDL)
 */
std::string
CDFG_ShiftElem::input_from_str
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
CDFG_ShiftElem::input_to_str
(const unsigned & at)
{
  return this->output_to_str();
} // input_to_str

/**
   出力元のコード(Verilog HDL)の取得
   @return 出力元のコード(Verilog HDL)
 */
std::string
CDFG_ShiftElem::output_from_str
(void)
{
  return "(" + this->input_from_str(0)
    + this->_get_shift_str()
    + this->input_from_str(1) + ")";
} // output_from_str

/**
   出力先のコード(Verilog HDL)の取得
   @return 出力先のコード(Verilog HDL)
 */
std::string
CDFG_ShiftElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
} // output_to_str
