#include "CLabel.hpp"

/**
   コンストラクタ
   @param[in] name ラベル名
   @param[in] state ラベルに対応するステート番号
 */
CLabel::CLabel(const std::string & name,
               const unsigned & state)
  : _name(name),
    _state(state)
{}

/**
   ラベルの名前を返す
   @return ラベル名
 */
std::string & CLabel::get_name(void) {
  return this->_name;
}

/**
   ラベルに対応するステート番号を変えす
   @return ステート番号
 */
unsigned & CLabel::get_state(void) {
  return this->_state;
}
