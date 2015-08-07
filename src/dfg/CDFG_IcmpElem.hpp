#ifndef _CDFG_ICMPELEM_H
#define _CDFG_ICMPELEM_H

#include "CDFG_Element.hpp"

/**
   @class CDFG_IcmpElem
   @brief icmp命令を表す Elementクラス
 */
class CDFG_IcmpElem : public CDFG_Element {
public:
  /** 比較条件を表す定数 */
  enum class eCond : unsigned {
    EQ  = 1 << 0, ///< ==
    NE  = 1 << 1, ///< !=
    UGT = 1 << 2, ///< (unsigned) > (unsigned)
    UGE = 1 << 3, ///< (unsigned) >= (unsigned)
    ULT = 1 << 4, ///< (unsigned) < (unsigned)
    ULE = 1 << 5, ///< (unsigned) <= (unsigned)
    SGT = 1 << 6, ///< (signed) > (signed)
    SGE = 1 << 7, ///< (signed) >= (signed)
    SLT = 1 << 8, ///< (signed) < (signed)
    SLE = 1 << 9, ///< (signed) <= (signed)
    };

  /**
     コンストラクタ
     @param[in] cond 比較条件
     @param[in] step 比較を実行するステップ
     @param[in] state 比較を実行するステート
   */
  CDFG_IcmpElem(const CDFG_IcmpElem::eCond & cond,
                const unsigned & state,
                const unsigned & step)
    : _cond(cond),
      CDFG_Element(CDFG_Operator::eType::ICMP,
                   2, // num input
                   state,
                   step)
  {}
  ~CDFG_IcmpElem(void) {}

  const eCond & get_condition(void) { return this->_cond; }

private:
  const eCond _cond; ///< 比較条件
};

#endif
