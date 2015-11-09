#include "CDFG_FcmpElem.hpp"

/**
   比較結果の論理演算のコード (Verilog)
   @return 比較結果の論理演算のコード (Verilog)
*/
const std::string
CDFG_FcmpElem::get_condition_code(void)
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
} // get_condition_code
