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
      OEQ,  ///< orderd and equal
      OGT,  ///< orderd and greater than
      OGE,  ///< orderd and greater than or equal
      OLT,  ///< orderd and less than
      OLE,  ///< orderd and less than or equal
      ONE,  ///< orderd and not equal
      ORD,  ///< orderd
      UNO,  ///< unorderd
      UEQ,  ///< unorderd and equal
      UGT,  ///< unorderd and greater than
      UGE,  ///< unorderd and greater than or equal
      ULT,  ///< unorderd and less than
      ULE,  ///< unorderd and less than or equal
      UNE,  ///< unorderd and not equal
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
