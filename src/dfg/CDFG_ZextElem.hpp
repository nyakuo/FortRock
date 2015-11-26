#ifndef _CDFG_ZEXTELEM_H
#define _CDFG_ZEXTELEM_H

#include <memory>
#include <string>

#include "CDFG_Element.hpp"
#include "CDFG_Node.hpp"
#include "CDFG_Operator.hpp"

/**
   @class CDFG_ZextElem
   @brief Zext命令を表すクラス
 */
class CDFG_ZextElem : public CDFG_Element {
public:
  CDFG_ZextElem(const unsigned & state,
                const unsigned & step);

  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;

private:
};

#endif
