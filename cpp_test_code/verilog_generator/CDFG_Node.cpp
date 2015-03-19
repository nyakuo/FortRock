#include "CDFG_Element.h"

CDFG_Element::CDFG_Element(const string & name,
                           const unsigned & bit_width,
                           const bool & is_signed)
  : name(name),
    bit_width(bit_width),
    is_signed(is_signed)
{}
