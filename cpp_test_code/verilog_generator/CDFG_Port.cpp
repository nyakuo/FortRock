#include "CDFG_Port.hpp"

CDFG_Port::CDFG_Port(const unsigned & bit_width,
                     const bool & is_signed)
  : _bit_width(bit_width),
    _is_signed(is_signed)
{}
