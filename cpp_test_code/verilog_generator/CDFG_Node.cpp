#include "CDFG_Node.h"

CDFG_Node::CDFG_Node(const std::string & name,
                     const unsigned & bit_width,
                     const bool & is_signed,
                     const CDFG_Node::eNode & type)
  : __name(name),
    __bit_width(bit_width),
    __is_signed(is_signed),
    __type(type)
{}
