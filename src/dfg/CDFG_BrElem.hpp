#ifndef _CDFG_BRELEM_H
#define _CDFG_BRELEM_H

#include <string>
#include <memory>

#include "CDFG_Element.hpp"
#include "CDFG_Node.hpp"
#include "CDFG_Operator.hpp"

/**
   @class CDFG_BrElem
   @brief Br̿���ɽ�����饹
 */
class CDFG_BrElem : public CDFG_Element {
public:
  CDFG_BrElem(const std::shared_ptr<CDFG_Node> & prev_state,
              const std::shared_ptr<CDFG_Node> & step_node,
              const unsigned & state,
              const unsigned & step);

  void set_condition(const std::shared_ptr<CDFG_Node> & tf);

  // Getter
  const std::shared_ptr<CDFG_Node> & get_condition_node(void) const;
  bool is_conditional(void) const;

  // Override
  virtual std::string other_str(const std::string & indent) override final;

private:
  std::shared_ptr<CDFG_Node> _tf_node; ///< ���Ρ��� (True or False)
  const std::shared_ptr<CDFG_Node> _prev_state; ///< prev state�Ρ���
  const std::shared_ptr<CDFG_Node> _step_node;  ///< step�Ρ���
};

#endif
