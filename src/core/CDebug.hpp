#ifndef _C_DEBUG_H
#define _C_DEBUG_H

#include <iostream>
#include <memory>
#include <list>
#include "../dfg/CModule.hpp"
#include "../dfg/CDFG_Node.hpp"

/**
   @class CDebug
   @brief デバッグ情報の表示クラス
   @attention デバッグ用途以外に使用しない
 */
class CDebug {
public:
  CDebug(void) {}
  ~CDebug(void) {}

  static void output_node_info
  (const std::list<std::shared_ptr<CDFG_Node> > & dfg);
};

#endif
