// -*- C++ -*-
#ifndef _CDFG_ELEMENT_H
#define _CDFG_ELEMENT_H

#include <iostream>
#include <memory>
#include <vector>
#include "CDFG_Node.h"

class CDFG_Element {
private:
  std::vector< std::shared_ptr< CDFG_Node > > _input_list;
  std::vector< std::shared_ptr< CDFG_Node > > _output_list;
  std::shared_ptr< CDFG_Node > _ope;
  unsigned _state;

public:
  CDFG_Element(void);
  int set_ope(std::shared_ptr< CDFG_Node > & ope);
  void set_input_size(const unsigned & size);
  void set_output_size(const unsigned & size);
  int set_input(std::shared_ptr< CDFG_Node > & input, const unsigned & number);
  int set_output(std::shared_ptr< CDFG_Node > & input, const unsigned & number);
  void set_state(const unsigned & state);
};

#endif
