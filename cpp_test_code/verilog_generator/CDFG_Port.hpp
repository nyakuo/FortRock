#ifndef _CDFG_PORT_H
#define _CDFG_PORT_H

/**
   @class CDFG_Port
   @brief 演算器のポートの定義を行うクラス

   CDFG_Operatorクラスの入出力ポートを定義する情報クラス
   参照されて使われる
*/
class CDFG_Port {
public:
  CDFG_Port(const unsigned & bit_width,
            const bool & is_signed);

  inline unsigned get_bit_width(void) const { return this->_bit_width; }
  inline bool get_is_signed(void) const { return this->_is_signed; }

private:
  unsigned _bit_width;
  bool _is_signed;
};

#endif
