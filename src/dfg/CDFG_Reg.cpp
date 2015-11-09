#include "CDFG_Reg.hpp"

/**
   入力信号かどうかを取得
   @return 常にfalse
 */
bool
CDFG_Reg::is_input(void)
{
  return false;
} // is_input

/**
   出力信号かどうかを取得
   @return FIN, OUTの場合のみ true
 */
bool
CDFG_Reg::is_output(void)
{
  switch (this->_type) {
  case eRegType::Fin:
  case eRegType::Out:
    return true;

  default:
    return false;
  }
} // is_output

/**
   信号名の接頭辞を取得
   @return 信号名の接頭辞
 */
std::string
CDFG_Reg::_get_prefix
(const CDFG_Reg::eRegType & type)
{
  switch (type) {
  case eRegType::In_copy:
  case eRegType::Reg:
    return "r_";

  case eRegType::Fin:
  case eRegType::Out:
    return "o_r_";

  case eRegType::State:
  case eRegType::Prev_state:
  case eRegType::Step:
    return "r_sys_";
  }
} // _get_prefix
