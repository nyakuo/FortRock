#ifndef _CNODE_DATA_H
#define _CNODE_DATA_H

/**
   @class CNode_data
   @brief 入出力ポートのデータを保持する
*/
class CNode_data {
public:
  /** ポートの種類定数 */
  enum class ePortType : unsigned {
    IN = 0, //! 入力
      OUT,  //! 出力
      CLK,  //! clock
      CE,   //! clock enable
      };

  /**
     コンストラクタ
     @param[in] port_name ポート名
     @param[in] bit_width ポートのビット幅
     @param[in] type ポートの種類
     @param[in] is_signed ポートがsignedか
   */
  CNode_data(const std::string & port_name,
             const unsigned & bit_width,
             const ePortType & type,
             const bool & is_signed)
    : _port_name(port_name),
      _bit_width(bit_width),
      _type(type),
      _is_signed(is_signed)
  {}

  // getter
  std::string & get_name(void)      { return this->_port_name; }
  unsigned    & get_bit_width(void) { return this->_bit_width; }
  ePortType   & get_type(void)      { return this->_type; }
  bool        & get_is_signed(void) { return this->_is_signed; }

private:
  std::string       _port_name; //! 入出力ポート名
  unsigned          _bit_width; //! 入出力のビット幅
  ePortType         _type;      //! 入出力の種類 (REG, WIRE)
  bool              _is_signed; //! 入出力がsignedかどうか
};

#endif
