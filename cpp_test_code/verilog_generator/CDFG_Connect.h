// -*- C++ -*-
#ifndef __CDEFG_CONNECT
#define __CDEFG_CONNECT

#include <iostream>
using namespace std;

class CDFG_Element;

class CDFG_Connect {
public:
  const unsigned exec_state;
  const CDFG_Element * connect_to;

  CDFG_Connect(const unsigned & exec_state,
               const CDFG_Element * connect_to);

};

#endif
