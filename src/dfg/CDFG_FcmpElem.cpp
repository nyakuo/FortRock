#include "CDFG_FcmpElem.hpp"

/**
   比較演算のコード (Verilog)
   @return 比較演算のコード (Verilog)
*/
const std::string
CDFG_FcmpElem::_get_condition_code(void)
{
  auto result = this->get_operator()
    ->get_output_node_at(0)->get_verilog_name();

  switch (this->_cond) {
  case eCond::Ueq:
    return result + "[3] & " + result + "[0]";

  case eCond::Une:
    return result + "[3] & " + result + "[0]";

  case eCond::Ult:
    return result + "[3] & " + result + "[1]";

  case eCond::Ugt:
    return result + "[3] & " + result + "[2]";

  case eCond::Uge:
    return result + "[3] & " + result + "[2] | " + result + "[0]";

  case eCond::Ule:
    return result + "[3] & " + result + "[1] | " + result + "[0]";

  case eCond::Oeq:
    return "~" + result + "[3] & " + result + "[0]";

  case eCond::One:
    return "~" + result + "[3] & ~" + result + "[0]";

  case eCond::Olt:
    return "~" + result + "[3] & " + result + "[1]";

  case eCond::Ogt:
    return "~" + result + "[3] & " + result + "[2]";

  case eCond::Oge:
    return "~" + result + "[3] & " + result + "[2] | " + result + "[0]";

  case eCond::Ole:
    return "~" + result + "[3] & " + result + "[1] | " + result + "[0]";

  default: return "";
  }
} // _get_condition_code


/**
   演算の入力元のコード(Verilog HDL)の取得
   @param[in] at 取得する入力の位置 (0 or 1)
   @return 演算の入力元のコード(Verilog HDL)
 */
std::string
CDFG_FcmpElem::input_from_str
(const unsigned & at)
{
  return this->_get_input_str(at);
} // input_from_str

/**
   入力先のコード(Verilog HDL)の取得
   @param[in] at 取得する演算器の入力の位置 (0 or 1)
   @return 入力先のコード(Verilog HDL)
 */
std::string
CDFG_FcmpElem::input_to_str
(const unsigned & at)
{
  return this->get_operator()
    ->get_input_node_at(at)
    ->get_verilog_name();
} // input_to_str

/**
   演算の出力部の文字列(Verilog HDL)の取得
   @return 演算の出力部の文字列(Verilog HDL)
 */
std::string
CDFG_FcmpElem::output_from_str
(void)
{
  switch (this->_cond) {
  case eCond::True:
    return this->_true_node->get_verilog_name();
    break;

  case eCond::False:
    return this->_false_node->get_verilog_name();
    break;

  default:
    return "(" + this->_get_condition_code() + ")";
    break;
  }
} // output_from_str

/**
   命令の出力部の取得
   @return 命令の出力部 (Verilog HDL)
 */
std::string
CDFG_FcmpElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
} // output_to_str
