#include "CDFG_Node.h"

CDFG_Node::CDFG_Node(const std::string & name,
                     const unsigned & bit_width,
                     const bool & is_signed,
                     const CDFG_Node::eNode & type)
  : _name(name),
    _bit_width(bit_width),
    _is_signed(is_signed),
    _type(type)
{}
