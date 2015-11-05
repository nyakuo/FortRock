#ifndef _CDFG_STOREELEM_H
#define _CDFG_STOREELEM_H

#include "CDFG_Element.hpp"

/**
   @class CDFG_StoreElem
   @brief Store 命令を表すクラス
 */
class CDFG_StoreElem : public CDFG_Element {
public:
  CDFG_StoreElem(const unsigned & state,
                 const unsigned & step,
                 const unsigned & latency);

  // setter
  void set_use_port(const unsigned & port_num);

private:
  unsigned _port_num; ///< 使用するアクセスポート
};

#endif
