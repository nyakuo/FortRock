#include "CDFG_Port.hpp"

CDFG_Port::CDFG_Port(const std::string & name,
                     const unsigned & bit_width,
                     const bool & is_signed,
                     const eType & type)
  : _name(name),
    _bit_width(bit_width),
    _is_signed(is_signed),
    _type(type)
{}

CDFG_Port::~CDFG_Port() {}


/**
   ポートのビット幅の取得
   @return ポートのビット幅
 */
unsigned
CDFG_Port::get_bit_width(void) const{
  return this->_bit_width;
}

/**
   ポートがsignedかどうかを取得
   @return ポートがsignedかどうか
 */
bool
CDFG_Port::get_is_signed(void) const {
  return this->_is_signed;
}

/**
   ポートの種類を取得
   @return 種類
 */
CDFG_Port::eType
CDFG_Port::get_type(void) const {
  return this->_type;
}

/**
   ポートの名前を取得
   @note ポートの名前と接続されている信号名は同一ではない
   @return ポートの名前
 */
std::string
CDFG_Port::get_name(void) const {
  return this->_name;
}

/**
   接続されているノードの参照を取得
   @return 接続されているノードの参照
 */
std::shared_ptr<CDFG_Node>
CDFG_Port::get_node(void) const {
  return this->_node;
}
