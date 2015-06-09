#include "CDFG_Node.hpp"

CDFG_Node::CDFG_Node(const std::string & name,
                     const unsigned & bit_width,
                     const bool & is_signed,
                     const CDFG_Node::eNode & type,
                     const double & parameter,
                     const long & address,
                     const long & access_port)
  : _name(name),
    _bit_width(bit_width),
    _is_signed(is_signed),
    _type(type),
    _parameter(parameter),
    _address(address),
  _access_port(access_port)
{}

/* ゲッター */
std::string CDFG_Node::get_name(void) const { return this->_name; }
unsigned CDFG_Node::get_bit_width(void) const { return this->_bit_width; }
bool CDFG_Node::get_is_signed(void) const { return this->_is_signed; }
CDFG_Node::eNode CDFG_Node::get_type(void) const { return this->_type; }
double CDFG_Node::get_parameter(void) const { return this->_parameter; }
long CDFG_Node::get_address(void) const { return this->_address; }
long CDFG_Node::get_access_port(void) const { return this->_access_port; }
