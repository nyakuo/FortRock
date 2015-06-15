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
      REG   = 1 << 0,
      WIRE  = 1 << 1,
      IN    = 1 << 2,
      OUT   = 1 << 3,
      PARAM = 1 << 4,
      ARRAY = 1 << 5,
      CLK   = 1 << 6,
      RES   = 1 << 7,
      REQ   = 1 << 8,
      CE    = 1 << 9,
      FIN   = 1 << 10,
      STATE = 1 << 11,
      STEP  = 1 << 12,
      };

  CDFG_Node(const std::string & name,
            const unsigned & bit_width,
            const bool & is_signed,
            const CDFG_Node::eNode & type,
            const double & parameter = 0,
            const long & address = 0,
            const long & access_port = 0);
  ~CDFG_Node(){}

  /* ゲッター */
  std::string get_name(void) const;
  unsigned get_bit_width(void) const;
  bool get_is_signed(void) const;
  CDFG_Node::eNode get_type(void) const;
  double get_parameter(void) const;
  long get_address(void) const;
  long get_access_port(void) const;

private:
  std::string _name;      /** ノードの名前 */
  unsigned _bit_width;    /** ビット幅 */
  bool _is_signed;        /** signedかどうか */
  CDFG_Node::eNode _type; /** ノードの種類 */
  double _parameter;      /** parameter用定数 */
  long _address;          /** アドレス (only array) */
  long _access_port;       /** アクセスポート (only array) */
};

#endif
