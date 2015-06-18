#ifndef _CDFG_NODE_H
#define _CDFG_NODE_H

#include <string>

/**
   @class CDFG_Node
   @brief 演算器やモジュール(関数)を表すクラス
 */
class CDFG_Node {
public:
  enum class eNode : unsigned {
      REG        = 1 << 0,
      WIRE       = 1 << 1,
      IN         = 1 << 2,
      OUT        = 1 << 3,
      PARAM      = 1 << 4,
      ARRAY      = 1 << 5,
      CLK        = 1 << 6,
      RES        = 1 << 7,
      REQ        = 1 << 8,
      CE         = 1 << 9,
      FIN        = 1 << 10,
      STATE      = 1 << 11,
      PREV_STATE = 1 << 12,
      STEP       = 1 << 13,
      LABEL      = 1 << 14,
      };

  CDFG_Node(const std::string & asm_name,
            const unsigned & bit_width,
            const bool & is_signed,
            const CDFG_Node::eNode & type,
            const double & parameter = 0,
            const long & address = 0,
            const long & access_port = 0);
  ~CDFG_Node(){}

  /* ゲッター */
  std::string & get_asm_name(void);
  std::string & get_verilog_name(void);
  unsigned & get_bit_width(void);
  bool & get_is_signed(void);
  CDFG_Node::eNode & get_type(void);
  double & get_parameter(void);
  long & get_address(void);
  long & get_access_port(void);

  // setter
  void set_type(CDFG_Node::eNode type);

private:
  std::string _asm_name;     //! ノードの名前
  std::string _verilog_name; //! Verilog HDL上での名前
  unsigned _bit_width;       //! ビット幅
  bool _is_signed;           //! signedかどうか
  CDFG_Node::eNode _type;    //! ノードの種類
  double _parameter;         //! parameter用定数
  long _address;             //! アドレス (only array)
  long _access_port;         //! アクセスポート (only array)
};

#endif
