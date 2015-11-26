#include "CDFG_IcmpElem.hpp"

/**
   比較演算のコード (Verilog)
   @return 比較演算のコード (Verilog)
 */
std::string
CDFG_IcmpElem::_get_condition_code
(void)
{
  switch (this->_cond) {
  case CDFG_IcmpElem::eCond::Eq:
    return " == ";

  case CDFG_IcmpElem::eCond::Ne:
    return " != ";

  case CDFG_IcmpElem::eCond::Ugt:
  case CDFG_IcmpElem::eCond::Sgt:
    return " > ";

  case CDFG_IcmpElem::eCond::Uge:
  case CDFG_IcmpElem::eCond::Sge:
    return " >= ";

  case CDFG_IcmpElem::eCond::Ule:
  case CDFG_IcmpElem::eCond::Sle:
    return " <= ";

  case CDFG_IcmpElem::eCond::Ult:
  case CDFG_IcmpElem::eCond::Slt:
    return  " < ";

  default:;
  }
} // _get_condition_code

/**
   演算の入力のコード(Verilog HDL)の取得
   @param[in] at 取得する入力の位置 (0 or 1)
   @return 演算の入力のコード(Verilog HDL)
 */
std::string
CDFG_IcmpElem::input_from_str
(const unsigned & at)
{
  return this->_get_input_str(at);
} // input_from_str

/**
   演算の入力を出力 (Verilog HDL)
   @return 演算の入力 (Verilog HDL)
 */
std::string
CDFG_IcmpElem::output_from_str
(void)
{
  return "("
    + this->input_from_str(0)
    + " " + this->_get_condition_code() + " "
    + this->input_from_str(1)
    + ")";
} // output_from_str

/**
   演算の出力のコード(Verilog HDL)の取得
   @return 演算の出力のコード(Verilog HDL)
 */
std::string
CDFG_IcmpElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
} // output_to_str
