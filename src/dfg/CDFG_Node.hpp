#ifndef _CDFG_NODE_H
#define _CDFG_NODE_H

#include <string>
#include <cmath>

/**
   @class CDFG_Node
   @brief 演算器やモジュール(関数)を表すクラス
 */
class CDFG_Node {
public:
  /** ノードの種類 */
  enum class eNode : unsigned {
      REG          = 1 << 0,
      WIRE         = 1 << 1,
      IN           = 1 << 2,
      OUT          = 1 << 3,
      PARAM        = 1 << 4,
      ARRAY        = 1 << 5,
      CLK          = 1 << 6,
      RES          = 1 << 7,
      REQ          = 1 << 8,
      CE           = 1 << 9,
      FIN          = 1 << 10,
      STATE        = 1 << 11,
      PREV_STATE   = 1 << 12,
      FINISH_LABEL = 1 << 13,
      STEP         = 1 << 14,
      LABEL        = 1 << 15,
      TRUE         = 1 << 16,
      FALSE        = 1 << 17,
      ZERO         = 1 << 18,
      IN_ORIG      = 1 << 19,
      OTHER        = 1 << 30,
      };

  /** ICMP命令の比較条件 */
  enum class eCond : unsigned {
    EQ  = 1 << 0, //! ==
    NE  = 1 << 1, //! !=
    UGT = 1 << 2, //! (unsigned) > (unsigned)
    UGE = 1 << 3, //! (unsigned) >= (unsigned)
    ULT = 1 << 4, //! (unsigned) < (unsigned)
    ULE = 1 << 5, //! (unsigned) <= (unsigned)
    SGT = 1 << 6, //! (signed) > (signed)
    SGE = 1 << 7, //! (signed) >= (signed)
    SLT = 1 << 8, //! (signed) < (signed)
    SLE = 1 << 9, //! (signed) <= (signed)
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
  std::string get_safe_name(void);
  std::string & get_asm_name(void);
  std::string & get_verilog_name(void);
  unsigned & get_bit_width(void);
  bool & get_is_signed(void);
  bool get_is_float(void);
  CDFG_Node::eNode & get_type(void);
  double & get_fparameter(void);
  long get_parameter(void);
  std::string get_param_str(void);
  long & get_address(void);
  long & get_access_port(void);
  eCond & get_condition(void);

  // setter
  void set_type(const CDFG_Node::eNode & type);
  void set_condition(const CDFG_Node::eCond & cond);

private:
  std::string _asm_name;     //! ノードの名前
  std::string _verilog_name; //! Verilog HDL上での名前
  unsigned _bit_width;       //! ビット幅
  bool _is_signed;           //! signedかどうか
  bool _is_float;            //! 浮動小数点かどうか
  CDFG_Node::eNode _type;    //! ノードの種類
  double _parameter;         //! parameter用定数
  long _address;             //! アドレス (only array)
  long _access_port;         //! アクセスポート (only array)
  eCond _condition;          //! icmp命令の比較条件
};

#endif
