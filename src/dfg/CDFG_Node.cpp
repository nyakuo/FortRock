#include "CDFG_Node.hpp"

/**
   コンストラクタ
   @param[in] name NodeのLLVM IR上での名前
   @param[in] bit_width Nodeのビット幅
   @param[in] is_signed Nodeが is_signed かどうか
   @param[in] type Nodeの種類
   @param[in] parameter Nodeに設定する定数 (定数やアドレス)
   @note verilog_name の作成も行っている
 */
CDFG_Node::CDFG_Node(const std::string & asm_name,
                     const unsigned & bit_width,
                     const bool & is_signed,
                     const CDFG_Node::eNode & type,
                     const double & parameter)
  : _asm_name(asm_name),
    _bit_width(bit_width),
    _is_signed(is_signed),
    _type(type),
    _parameter(parameter)
{
  // 変数に使用できない文字の置換
  std::string safe_name = this->get_safe_name();

  // 変数名に接頭語をつける
  std::string prefix("");
  switch (type) {
  case eNode::IN:
  case eNode::REG: prefix = "r_";
    break;

  case eNode::WIRE: prefix = "w_";
    break;

  case eNode::CLK:
  case eNode::RES:
  case eNode::REQ:
  case eNode::CE:
  case eNode::IN_ORIG: prefix = "i_";
    break;

  case eNode::FIN:
  case eNode::OUT: prefix = "o_";
    break;

  case eNode::TRUE:
  case eNode::FALSE:
  case eNode::ZERO:
  case eNode::PARAM: prefix = "p_";
    break;

  case eNode::STATE:
  case eNode::PREV_STATE:
  case eNode::STEP: prefix = "r_sys_";
    break;

  case eNode::FINISH_LABEL:
  case eNode::LABEL: prefix = "l_";
    break;

  default:
    prefix = "undef_";
  } // switch

  if (type == eNode::IN_ORIG)
    this->_asm_name += "original";

  this->_verilog_name = prefix + safe_name;
} // CDFG_Node

/**
   Node名に含まれる'.'文字を'_'に変換する
   @param[in] name 変換する名前
   @return 変換後の名前
   @note CModuleGeneratorでPHI命令の出力に使用
 */
std::string
CDFG_Node::get_safe_name(void) {
  auto safe_name = this->_asm_name;

  int at;
  while((at = safe_name.find('.', 0)) != std::string::npos)
    safe_name.replace(at, 1, "_");

  while((at = safe_name.find('-', 0)) != std::string::npos)
    safe_name.replace(at, 1, "m");

  return safe_name;
}

/**
   NodeのLLVM IR上での信号名を取得
   @return NodeのLLVM IR上での信号名
 */
std::string &
CDFG_Node::get_asm_name(void){
  return this->_asm_name;
}

/**
   NodeのVerilog HDL上での信号名を取得
   @return NodeのVerilog HDL上での信号名
 */
std::string &
CDFG_Node::get_verilog_name(void){
  return this->_verilog_name;
}

/**
   Nodeのビット幅の取得
   @return Nodeのビット幅
 */
unsigned &
CDFG_Node::get_bit_width(void) {
  return this->_bit_width;
}

/**
   Nodeが is_signed かどうかを取得
   @return Nodeが is_signed かどうか
 */
bool &
CDFG_Node::get_is_signed(void) {
  return this->_is_signed;
}

/**
   Node が浮動小数点かどうかを取得
   @return Node が浮動小数点かどうか
 */
bool
CDFG_Node::get_is_float(void) {
  return ceil(this->_parameter)
    != floor(this->_parameter);
}

/**
   Nodeの種類を取得
   @return Nodeの種類
 */
CDFG_Node::eNode &
CDFG_Node::get_type(void) {
  return this->_type;
}

/**
   Nodeのパラメータ(定数)の取得
   @return Nodeのパラメータ(定数)
 */
double &
CDFG_Node::get_fparameter(void) {
  return this->_parameter;
}

/**
   パラメータの整数部を取得
   @return パラメータの整数部
   @note パラメータが整数である場合に使用
 */
long
CDFG_Node::get_parameter(void) {
  double a, b;
  b = modf(this->_parameter, &a);
  return (long)a;
}

/**
   パラメータの文字列をビット幅指定を含めて取得
   @return パラメータの文字列
   @note パラメータの形式(double, long)に応じて
         出力が変化
 */
std::string
CDFG_Node::get_param_str(void) {
  std::string ret ("");
  char buf[64];

  if (this->_parameter < 0) // 負の数
    ret = "-";

  // ビット幅
  ret += std::to_string(this->get_bit_width())
    + "'h";

  if (this->get_is_float()) { // 浮動小数点
    ret += std::to_string(fabs(this->get_fparameter()));
  }
  else { // 整数
    snprintf(buf, sizeof(buf), "%lx",
             std::abs(this->get_parameter()));
    ret += buf;
  }
  return ret;
}

/**
   icmp命令の比較条件の取得
   @return icmp命令の比較条件
 */
CDFG_Node::eCond &
CDFG_Node::get_condition(void) {
  return this->_condition;
}

/**
   Nodeの種類の再設定
   @param[in] type 再設定する種類
   @note fortrock.cpp(FortRockの本体)で利用するためだけに実装
 */
void CDFG_Node::set_type
(const CDFG_Node::eNode & type) {
  this->_type = type;
}

/**
   icmp命令の比較条件の設定
   @param[in] cond 比較条件
 */
void CDFG_Node::set_condition
(const CDFG_Node::eCond & cond) {
  this->_condition = cond;
}
