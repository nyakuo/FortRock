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
    Eq  = 1 << 0, ///< ==
    Ne  = 1 << 1, ///< !=
    Ugt = 1 << 2, ///< (unsigned) > (unsigned)
    Uge = 1 << 3, ///< (unsigned) >= (unsigned)
    Ult = 1 << 4, ///< (unsigned) < (unsigned)
    Ule = 1 << 5, ///< (unsigned) <= (unsigned)
    Sgt = 1 << 6, ///< (signed) > (signed)
    Sge = 1 << 7, ///< (signed) >= (signed)
    Slt = 1 << 8, ///< (signed) < (signed)
    Sle = 1 << 9, ///< (signed) <= (signed)
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
      CDFG_Element(CDFG_Operator::eType::Icmp,
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
