#include "CDFG_Element.h"

CDFG_Element::CDFG_Element(void) {
  this->_ope = NULL;
}

int CDFG_Element::set_ope(std::shared_ptr< CDFG_Node > & ope) {
  int ret = 0;
  // overwrite
  if (this->_ope) {
    this->_ope.reset();
    ret = 1;
  }

  this->_ope = std::move(ope);

  return ret;
}

void CDFG_Element::set_input_size(const unsigned & size) {
  this->_input_list.reserve(size);
}

void CDFG_Element::set_output_size(const unsigned & size) {
  this->_output_list.reserve(size);
}

int CDFG_Element::set_input(std::shared_ptr< CDFG_Node > & input, const unsigned & number) {
  if (number > this->_input_list.size())
    return -1;

  this->_input_list[number] = move(input);
  return 0;
}

int CDFG_Element::set_output(std::shared_ptr< CDFG_Node > & output, const unsigned & number) {
  if (number > this->_output_list.size())
    return -1;

  this->_output_list[number] = move(output);
  return 0;
}


void CDFG_Element::set_state(const unsigned & state) {
  this->_state = state;
}
