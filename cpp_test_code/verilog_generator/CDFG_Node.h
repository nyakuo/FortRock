// -*- C++ -*-
#ifndef __CDFG_NODE_H
#define __CDFG_NODE_H

#include <iostream>

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
      FUNC,
      };

  CDFG_Node(const std::string & name,
            const unsigned & bit_width,
            const bool & is_signed,
            const CDFG_Node::eNode & type);

  inline CDFG_Node::eNode get_type(void) const { return this->__type; }
  inline std::string get_name(void) const { return this->__name; }
  inline unsigned get_bit_width(void) const { return this->__bit_width; }
  inline bool get_is_signed(void) const { return this->__is_signed; }

private:
  CDFG_Node::eNode __type;
  std::string __name;
  unsigned __bit_width;
  bool __is_signed;

};

#endif
