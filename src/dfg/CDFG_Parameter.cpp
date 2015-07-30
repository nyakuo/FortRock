#include "CDFG_Parameter.hpp"

/**
   ノードに定数を設定する
   @param[in] parameter ノードに設定する定数
 */
void
CDFG_Parameter::set_parameter
(const double & parameter) {
  this->_fparameter
    = this->_parameter = parameter;
}

/**
   ノードに設定された定数を整数型で取得する
   @return ノードに設定された定数 (整数型)
 */
long
CDFG_Parameter::get_parameter(void) {
  return this->_parameter;
}

/**
   ノードに設定された定数を浮動小数点型で取得する
   @return ノードに設定された定数 (浮動小数点型)
*/
double
CDFG_Parameter::get_fparameter(void) {
  return this->_fparameter;
}

/**
   パラメータの文字列をビット幅指定を含めて取得
   @return パラメータの文字列
   @note パラメータの形式(double, long)に応じて
         出力が変化
 */
std::string
CDFG_Parameter::to_string(void){
  std::string ret ("");
  char buf[64];

  if (this->_parameter < 0) // 負の数
    ret = "-";

  // ビット幅
  ret += std::to_string(this->get_bit_width())
    + "'h";

  if (this->_param_type == eParamType::FLOAT) // 浮動小数点
    ret += std::to_string(fabs(this->get_fparameter()));
  else { // 整数
    snprintf(buf, sizeof(buf), "%lx",
             std::abs(this->_parameter));
    ret += buf;
  }

  return ret;
}
