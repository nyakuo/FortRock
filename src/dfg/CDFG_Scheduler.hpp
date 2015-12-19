#ifndef _CDFG_SCHEDULER_H
#define _CDFG_SCHEDULER_H

#include <memory>
#include <list>
#include <algorithm>
#include <map>

#include "CDFG_Operator.hpp"
#include "CModule.hpp"
#include "CDFG_Element.hpp"
#include "CDFG_Addr.hpp"
#include "CDFG_Ram.hpp"

class CDFG_Ram;

/**
   @class CDFG_Scheduler
   @brief DFGのスケジューリングを行うクラス
 */
class CDFG_Scheduler {
public:
  CDFG_Scheduler
  (const std::shared_ptr<CModule> & module)
    : _module(module) {}
  ~CDFG_Scheduler(void) {}

  unsigned do_schedule(void);

private:
  std::shared_ptr<CModule> _module;

  int _min_step_data
  (const std::list<std::shared_ptr<CDFG_Element> > & list,
   const std::shared_ptr<CDFG_Element> & target_elem);

  int _min_step_operator
  (const std::list<std::shared_ptr<CDFG_Element> > & list,
   const std::shared_ptr<CDFG_Operator> & ope,
   const unsigned & data_depend_step);

  int _min_step_ram
  (const std::list<std::shared_ptr<CDFG_Element> > & list,
   const std::shared_ptr<CDFG_Element> & elem,
   const unsigned & data_depend_step);
  
  bool _can_use(const unsigned & step,
                const std::shared_ptr<CDFG_Operator> & ope,
                const std::list<std::shared_ptr<CDFG_Element> > & dfg);

  bool _can_use(const unsigned & step,
                const std::shared_ptr<CDFG_Ram> & ram,
                const std::shared_ptr<CDFG_Element> & elem,
                const std::list<std::shared_ptr<CDFG_Element> > & dfg);
  
  unsigned _get_last_step
  (const std::list<std::shared_ptr<CDFG_Element> > & list);

  std::shared_ptr<CDFG_Element> _get_last_elem
  (const std::list<std::shared_ptr<CDFG_Element> > & list);

  void _show_list // for debug
  (const std::list<std::shared_ptr<CDFG_Element> > & list);
};

#endif
