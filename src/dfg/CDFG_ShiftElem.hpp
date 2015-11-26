#ifndef _CDFG_SHIFTELEM_H
#define _CDFG_SHIFTELEM_H

#include <string>

#include "CDFG_Operator.hpp"
#include "CDFG_Element.hpp"

/**
   @class CDFG_ShiftElem
   @brief Shift命令を表すクラス
 */
class CDFG_ShiftElem : public CDFG_Element {
public:
  CDFG_ShiftElem(const CDFG_Operator::eType & type,
                 const unsigned & state,
                 const unsigned & step);

  // override
  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string input_to_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;

private:
  std::string _get_shift_str(void);
};

#endif
