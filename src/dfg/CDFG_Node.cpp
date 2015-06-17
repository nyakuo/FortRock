#include "CDFG_Node.hpp"

/**
   コンストラクタ
   @param[in] name Nodeの名前
   @param[in] bit_width Nodeのビット幅
   @param[in] is_signed Nodeが is_signed かどうか
   @param[in] type Nodeの種類
   @param[in] parameter Nodeに設定する定数 (PARAMの場合)
   @param[in] address メモリのアドレス (未使用)
   @param[in] access_port メモリのアクセスポート (未使用)
 */
CDFG_Node::CDFG_Node(const std::string & name,
                     const unsigned & bit_width,
                     const bool & is_signed,
                     const CDFG_Node::eNode & type,
                     const double & parameter,
                     const long & address,
                     const long & access_port)
  : _name(name),
    _bit_width(bit_width),
    _is_signed(is_signed),
    _type(type),
    _parameter(parameter),
    _address(address),
  _access_port(access_port)
{}

/**
   Nodeの信号名を取得
   @return Nodeの信号名
 */
std::string &
CDFG_Node::get_name(void){
  return this->_name;
}

/**
   Nodeのビット幅の取得
   @return    Nodeのビット幅
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
CDFG_Node::get_parameter(void) {
  return this->_parameter;
}

/**
   Nodeのメモリアドレスの取得 (未使用)
 */
long &
CDFG_Node::get_address(void) {
  return this->_address;
}

/**
   Nodeのメモリのアクセスポートの取得 (未使用)
 */
long &
CDFG_Node::get_access_port(void) {
  return this->_access_port;
}

/**
   Nodeの種類の再設定
   @param[in] type 再設定する種類
   @note fortrock.cpp(FortRockの本体)で利用するためだけに実装
 */
void CDFG_Node::set_type(CDFG_Node::eNode type) {
  this->_type = type;
}
