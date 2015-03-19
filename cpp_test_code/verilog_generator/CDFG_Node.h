// -*- C++ -*-
#ifndef __CDFG_NODE_H
#define __CDFG_NODE_H

#include <iostream>
#include <list>
#include "CDFG_Connect.h"
using namespace std;

class CDFG_Element {
public:
  string name;
  unsigned bit_width;
  bool is_signed;
  list<CDFG_Connect> connection_list;

  CDFG_Element(const string & name,
               const unsigned & bit_width,
               const bool & is_signed);

  void set_connect(const CDFG_Connect & connect);
};

#endif
