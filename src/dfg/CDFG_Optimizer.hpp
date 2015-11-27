#ifndef _CDFG_OPTIMIZER_H
#define _CDFG_OPTIMIZER_H

#include <memory>
#include <list>

#include "CModule.hpp"
#include "CDFG_Operator.hpp"
#include "CDFG_Node.hpp"
#include "CDFG_Element.hpp"

/**
   @class CDFG_Optimizer
   @brief DFGに対して最適化を実施するクラス
*/
class CDFG_Optimizer {
public:
  CDFG_Optimizer
  (const std::shared_ptr<CModule> & module)
    : _module(module) {}
 ~CDFG_Optimizer(void) {}

  unsigned do_optimize(void);

private:
  std::shared_ptr<CModule> _module;

  std::shared_ptr<CDFG_Node> _is_phi_output
  (const std::shared_ptr<CDFG_Node> & node,
   const std::list<std::shared_ptr<CDFG_Element> > & phi_list);

  bool _used_in_phi
  (const std::shared_ptr<CDFG_Node> & node,
   const std::list<std::shared_ptr<CDFG_Element> > & phi_list);

  bool _used_in_another_state
  (const unsigned & state,
   const std::shared_ptr<CDFG_Node> & node,
   const std::list<std::list<std::shared_ptr<CDFG_Element> > > & dfg);

  bool _is_multiple_use
  (const std::shared_ptr<CDFG_Element> & elem,
   const std::list<std::list<std::shared_ptr<CDFG_Element> > > & dfg);

  unsigned _replace_reg
  (const std::shared_ptr<CDFG_Element> & rm_elem,
   std::list<std::shared_ptr<CDFG_Element> > & state_dfg);
};

#endif
