#ifndef _CDFG_LOGICALELEM_H
#define _CDFG_LOGICALELEM_H

#include <string>

#include "CDFG_Element.hpp"

/**
   @class CDFG_LogicalElem
   @brief 論理演算を表すクラス
 */
class CDFG_LogicalElem : public CDFG_Element {
public:
  CDFG_LogicalElem(const CDFG_Operator::eType & type,
                   const unsigned & state,
                   const unsigned & step);

  // override
  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string input_to_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;

private:
  std::string _get_logical_str(void);
};

#endif
