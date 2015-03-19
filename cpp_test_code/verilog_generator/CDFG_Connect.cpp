#include "CDFG_Connect.h"

CDFG_Connect::CDFG_Connect(const unsigned & exec_state,
                           const CDFG_Element * connect_to)
  : exec_state(exec_state),
    connect_to(connect_to) {}
