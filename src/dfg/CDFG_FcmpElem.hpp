#ifndef _CDFG_FCMPELEM_H
#define _CDFG_FCMPELEM_H

#include <memory>
#include "CDFG_Element.hpp"
#include "CDFG_Operator.hpp"

/**
   @class CDFG_FcmpElem
   @brief fcmp命令を表す Elementクラス
 */
class CDFG_FcmpElem : public CDFG_Element {
public:

  /** 比較条件 */
  enum class eCond : unsigned char {
    False,  ///< always false
      True, ///< always true
      Oeq,  ///< orderd and equal
      Ogt,  ///< orderd and greater than
      Oge,  ///< orderd and greater than or equal
      Olt,  ///< orderd and less than
      Ole,  ///< orderd and less than or equal
      One,  ///< orderd and not equal
      Ord,  ///< orderd
      Uno,  ///< unorderd
      Ueq,  ///< unorderd and equal
      Ugt,  ///< unorderd and greater than
      Uge,  ///< unorderd and greater than or equal
      Ult,  ///< unorderd and less than
      Ule,  ///< unorderd and less than or equal
      Une,  ///< unorderd and not equal
  };

  /**
     コンストラクタ
     @param[in] cond 比較条件
     @param[in] step 比較を実行するステップ
     @param[in] state 比較を実行するステート
   */
  CDFG_FcmpElem(const CDFG_FcmpElem::eCond & cond,
                const std::shared_ptr<CDFG_Operator> & ope,
                const unsigned & state,
                const unsigned & step)
  : _cond(cond),
      CDFG_Element(ope, state, step)
  {}

  ~CDFG_FcmpElem(void) {}

  const eCond & get_condition(void) { return this->_cond; }
  const std::string get_condition_code(void);

private:
  const eCond _cond; ///< 比較条件
};

#endif
