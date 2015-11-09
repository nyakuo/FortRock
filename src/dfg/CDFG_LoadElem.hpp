#ifndef _CDFG_LOADELEM_H
#define _CDFG_LOADELEM_H

#include "CDFG_Element.hpp"
#include "CDFG_Addr.hpp"

/**
   @class CDFG_LoadElem
   @brief Load命令を表すElementクラス
 */
class CDFG_LoadElem : public CDFG_Element {
public:
  /**
     コンストラクタ
     @param[in] is_gepope getelementptr命令を含むか否か
     @param[in] state 命令の実行ステート
     @param[in] step 命令の実行ステップ
     @param[in] addr 読み込み元のアドレス
     @param[in] latency BRAMの読み込みレイテンシ
   */
  CDFG_LoadElem(const bool & is_gepope,
                const unsigned & state,
                const unsigned & step,
                const unsigned & latency)
    : _is_gepope(is_gepope),
      CDFG_Element(CDFG_Operator::eType::Load,
                   1, // 入力数
                   state,
                   step,
                   latency) {}
  ~CDFG_LoadElem(void) {}

  /**
     getelementptr命令を含むか否かを取得
     @return getelementptr命令を含むか否か
   */
  bool is_gepope(void) { return this->_is_gepope; }

private:
  const bool _is_gepope; ///< getelementptr命令を含むか否か
};

#endif
