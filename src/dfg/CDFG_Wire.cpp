#include "CDFG_Wire.hpp"
#include <iostream>
/**
   入力信号かどうかを取得
   @return WIRE以外基本的にtrue
 */
bool
CDFG_Wire::is_input(void) {

  switch (this->_type) {
  case eWireType::CLK:
  case eWireType::RES:
  case eWireType::REQ:
  case eWireType::CE:
  case eWireType::IN_ORIG:
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
CDFG_Wire::_get_prefix(void)
{
  switch(this->_type)
    {
    case eWireType::WIRE:
      return "w_";

    case eWireType::CLK:
    case eWireType::RES:
    case eWireType::REQ:
    case eWireType::CE:
    case eWireType::IN_ORIG:
      return "i_w_";
    }
}
