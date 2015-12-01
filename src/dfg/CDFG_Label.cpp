#include "CDFG_Label.hpp"

/**
   ラベルの値をverilogの文法で返す
   @return ラベルの値
   例) 8'h12ab
 */
std::string
CDFG_Label::to_string
(void)
{
  std::string ret ("");
  char buf[64];

  // ビット幅
  ret += std::to_string(this->get_bit_width())
    + "'h";

  snprintf(buf, sizeof(buf), "%x",
           this->_label_state);

  return ret + buf;
}
