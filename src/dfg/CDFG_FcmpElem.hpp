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
     @param[in] true_node Trueの定数ノードの参照
     @param[in] false_node Falseの定数ノードの参照
     @param[in] step 比較を実行するステップ
     @param[in] state 比較を実行するステート
   */
  CDFG_FcmpElem(const CDFG_FcmpElem::eCond & cond,
                const std::shared_ptr<CDFG_Node> & true_node,
                const std::shared_ptr<CDFG_Node> & false_node,
                const std::shared_ptr<CDFG_Operator> & ope,
                const unsigned & state,
                const unsigned & step)
  : _cond(cond),
    _true_node(true_node),
    _false_node(false_node),
      CDFG_Element(ope, state, step)
  {}

  ~CDFG_FcmpElem(void) {}

  // override
  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string input_to_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;

private:
  const eCond _cond; ///< 比較条件
  const std::shared_ptr<CDFG_Node> _true_node;  ///< True定数ノード
  const std::shared_ptr<CDFG_Node> _false_node; ///< False定数ノード

  const eCond & get_condition(void) { return this->_cond; }
  const std::string _get_condition_code(void);
};

#endif
