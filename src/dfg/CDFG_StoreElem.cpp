#include "CDFG_StoreElem.hpp"

/**
   コンストラクタ
   @param[in] state 命令実行ステート
   @param[in] step 命令実行ステップ
   @param[in] latency
*/
CDFG_StoreElem::CDFG_StoreElem
(const unsigned & state,
 const unsigned & step,
 const unsigned & latency)
  : CDFG_Element(CDFG_Operator::eType::Store,
                 1, // 入力数
                 state,
                 step,
                 latency)
{} // CDFG_StoreElem

/**
   アクセスに使用するポートの設定
   @param[in] port_num アクセスに使用するポート
 */
void
CDFG_StoreElem::set_use_port
(const unsigned & port_num) {
  this->_port_num = port_num;
} // set_use_port
