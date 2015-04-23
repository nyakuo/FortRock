#ifndef _CDFG_NODE_H
#define _CDFG_NODE_H

#include <string>

class CDFG_Node {
public:
  enum class eNode : unsigned {
    REG = 0,
      WIRE,
      IN,
      OUT,
      PARAM,
      ADD,
      MINUS,
      MUL,
      DIV,
      FUNC
      };

  CDFG_Node(const std::string & name,
            const unsigned & bit_width,
            const bool & is_signed,
            const CDFG_Node::eNode & type);

  inline CDFG_Node::eNode get_type(void) const { return this->_type; }
  inline std::string get_name(void) const { return this->_name; }
  inline unsigned get_bit_width(void) const { return this->_bit_width; }
  inline bool get_is_signed(void) const { return this->_is_signed; }

private:
  CDFG_Node::eNode _type;
  std::string _name;
  bool _is_signed;
  unsigned _bit_width;
  // todo: parameter用定数が足りない
  // todo: 演算器用入出力ポートの定義ができない
};

#endif
