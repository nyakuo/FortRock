#ifndef _COPERATOR_MANAGER_H
#define _COPERATOR_MANAGER_H

#include <memory>
#include <map>
#include <vector>

#include "../dfg/CDFG_Operator.hpp"

/**
   @class COperatorManger
   @brief 回路内の演算器を管理するクラス
 */
class COperatorManager {
public:
  COperatorManager(void);

  void add_operator(std::shared_ptr<CDFG_Operator> & ope);

  // getter
  unsigned get_num_module(const CDFG_Operator::eType & type);
  std::shared_ptr<CDFG_Operator> get_module_at(const unsigned & at,
                                               const CDFG_Operator::eType & type);
  bool useIPCore(const CDFG_Operator::eType & type);

private:
  std::map<CDFG_Operator::eType,
           std::vector<std::shared_ptr<CDFG_Operator> > > _operators;
  std::map<CDFG_Operator::eType, bool> _use_ipcore;
};

#endif
