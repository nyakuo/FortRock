#include "CDFG_Element.h"

CDFG_Element::CDFG_Element(void) {
  this->__ope = NULL;
}

int CDFG_Element::set_ope(std::shared_ptr< CDFG_Node > & ope) {
  int ret = 0;
  // overwrite
  if (this->__ope) {
    this->__ope.reset();
    ret = 1;
  }

  this->__ope = std::move(ope);

  return ret;
}

void CDFG_Element::set_input_size(const unsigned & size) {
  this->__input_list.reserve(size);
}

void CDFG_Element::set_output_size(const unsigned & size) {
  this->__output_list.reserve(size);
}

int CDFG_Element::set_input(std::shared_ptr< CDFG_Node > & input, const unsigned & number) {
  if (number > this->__input_list.size())
    return -1;

  this->__input_list[number] = move(input);
  return 0;
}

int CDFG_Element::set_output(std::shared_ptr< CDFG_Node > & output, const unsigned & number) {
  if (number > this->__output_list.size())
    return -1;

  this->__output_list[number] = move(output);
  return 0;
}


void CDFG_Element::set_state(const unsigned & state) {
  this->__state = state;
}
