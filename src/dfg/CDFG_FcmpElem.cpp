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
  case eCond::UEQ:
    return result + "[3] & " + result + "[0]";
  case eCond::UNE:
    return result + "[3] & " + result + "[0]";
  case eCond::ULT:
    return result + "[3] & " + result + "[1]";
  case eCond::UGT:
    return result + "[3] & " + result + "[2]";
  case eCond::UGE:
    return result + "[3] & " + result + "[2] | " + result + "[0]";
  case eCond::ULE:
    return result + "[3] & " + result + "[1] | " + result + "[0]";
  case eCond::OEQ:
    return "~" + result + "[3] & " + result + "[0]";
  case eCond::ONE:
    return "~" + result + "[3] & ~" + result + "[0]";
  case eCond::OLT:
    return "~" + result + "[3] & " + result + "[1]";
  case eCond::OGT:
    return "~" + result + "[3] & " + result + "[2]";
  case eCond::OGE:
    return "~" + result + "[3] & " + result + "[2] | " + result + "[0]";
  case eCond::OLE:
    return "~" + result + "[3] & " + result + "[1] | " + result + "[0]";

  default: return "";
  }
} // get_condition_code
