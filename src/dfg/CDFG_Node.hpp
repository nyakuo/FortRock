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
  enum class eType : unsigned {
    Reg = 0,
      Wire,
      Param,
      Label,
      Addr,
      Mem,
      Elem, ///< CDFG_Element
      };

  CDFG_Node(const std::string & asm_name,
            const unsigned & bit_width,
            const bool & is_signed,
            const CDFG_Node::eType & type,
            const std::string & prefix);

  CDFG_Node(const CDFG_Node::eType & type)
    : _type(type) {}

  virtual ~CDFG_Node(void) {}

  // getter
  std::string get_safe_name(void);
  std::string & get_asm_name(void);
  std::string & get_verilog_name(void);
  unsigned & get_bit_width(void);
  bool & get_is_signed(void);
  CDFG_Node::eType & get_type(void);
  virtual bool is_input(void) = 0;
  virtual bool is_output(void) = 0;

  // setter
  void set_type(const CDFG_Node::eType & type);
  virtual std::string to_string(void)
  { return this->get_verilog_name(); }

protected:
  std::string _asm_name;     ///< ノードの名前
  std::string _verilog_name; ///< Verilog HDL上での名前
  unsigned _bit_width;       ///< ビット幅
  bool _is_signed;           ///< signedかどうか
  eType _type;               ///< ノードの種類
};

#endif
