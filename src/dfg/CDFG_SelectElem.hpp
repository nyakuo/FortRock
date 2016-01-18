#ifndef _CDFG_SELECTELEM_H
#define _CDFG_SELECTELEM_H

#include <memory>
#include <string>

#include "CDFG_Element.hpp"
#include "CDFG_Node.hpp"
#include "CDFG_Operator.hpp"

/**
   @class CDFG_SelectElem
   @brief Select命令を表すクラス
 */
class CDFG_SelectElem : public CDFG_Element {
public:
  CDFG_SelectElem(const unsigned & state,
                  const unsigned & step);

  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;

};

#endif
