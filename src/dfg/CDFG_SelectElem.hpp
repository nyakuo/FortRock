#ifndef _CDFG_SELECTELEM_H
#define _CDFG_SELECTELEM_H

#include <memory>
#include <string>

#include "CDFG_Element.hpp"
#include "CDFG_Node.hpp"
#include "CDFG_Operator.hpp"

/**
   @class CDFG_SelectElem
   @brief Select̿���ɽ�����饹
 */
class CDFG_SelectElem : public CDFG_Element {
public:
  CDFG_SelectElem(const std::shared_ptr<CDFG_Node> & tf_node,
                  const unsigned & state,
                  const unsigned & step);

  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;

private:
  const std::shared_ptr<CDFG_Node> _tf_node; ///< ���Ρ��� (True or False)
};

#endif
