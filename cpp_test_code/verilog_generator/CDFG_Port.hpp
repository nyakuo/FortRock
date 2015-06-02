#ifndef _CDFG_PORT_H
#define _CDFG_PORT_H

#include <string>
#include <memory>
#include "CDFG_Node.hpp"

/**
   @class CDFG_Port
   @brief 演算器のポートの定義を行うクラス

   CDFG_Operatorクラスの入出力ポートを定義する情報クラス
   参照されて使われる
*/
class CDFG_Port {
public:
  /** 入出力のポートの種類 */
  enum class eType : unsigned {
    CLK = 0,                    // clock
      CE,                       // clock enable
      IN,                       // input
      OUT,                      // output
      OTH                       // other
  };

  CDFG_Port(const std::string & name,
            const unsigned & bit_width,
            const bool & is_signed,
            const eType & type);
  CDFG_Port(const std::string & name,
            std::shared_ptr<CDFG_Node> & node)
    : _name(name),
      _node(node) {} // test code
  ~CDFG_Port();

  /* ゲッター */
  unsigned get_bit_width(void) const;
  bool get_is_signed(void) const;
  eType get_type(void) const;
  std::string get_name(void) const;
  std::shared_ptr<CDFG_Node> get_node(void) const;

private:
  unsigned _bit_width;              /** 入出力ポートのビット幅 */
  bool _is_signed;                  /** 入出力ポートの扱い */
  eType _type;                      /** 入出力ポートの種類 */
  std::string _name;                /** 入出力ポートの名前 */
  std::shared_ptr<CDFG_Node> _node; /** ポートに接続される入出力ポート */
};

#endif
