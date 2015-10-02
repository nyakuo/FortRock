#ifndef _CDFG_SCHEDULER_H
#define _CDFG_SCHEDULER_H

#include <memory>
#include <list>
#include <algorithm>

#include "CModule.hpp"
#include "CDFG_Element.hpp"

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

  unsigned _get_last_step
  (const std::list<std::shared_ptr<CDFG_Element> > & list);

  void _show_list // for debug
  (const std::list<std::shared_ptr<CDFG_Element> > & list);
};

#endif
