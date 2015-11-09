#include "CDFG_Wire.hpp"

/**
   入力信号かどうかを取得
   @return WIRE以外基本的にtrue
 */
bool
CDFG_Wire::is_input(void) {

  switch (this->_type) {
  case eWireType::Clk:
  case eWireType::Res:
  case eWireType::Req:
  case eWireType::Ce:
  case eWireType::In_orig:
    return true;

  default: return false;
  }
}

/**
   出力信号かどうかを取得
   @return 常にfalse
 */
bool
CDFG_Wire::is_output(void) {
  return false;
}

/**
   信号名の接頭辞を取得
   @return 信号の接頭辞
 */
std::string
CDFG_Wire::_get_prefix
(const CDFG_Wire::eWireType & type)
{
  switch (type)
    {
    case eWireType::Wire:
      return "w_";

    case eWireType::Clk:
    case eWireType::Res:
    case eWireType::Req:
    case eWireType::Ce:
    case eWireType::In_orig:
      return "i_w_";
    }
}
