#ifndef _CDFG_ICMPELEM_H
#define _CDFG_ICMPELEM_H

#include <string>

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
     @param[in] true_node Trueの定数ノードの参照
     @param[in] false_node Falseの定数ノードの参照
     @param[in] step 比較を実行するステップ
     @param[in] state 比較を実行するステート
   */
  CDFG_IcmpElem(const CDFG_IcmpElem::eCond & cond,
                const std::shared_ptr<CDFG_Node> & true_node,
                const std::shared_ptr<CDFG_Node> & false_node,
                const unsigned & state,
                const unsigned & step)
    : _cond(cond),
      _true_node(true_node),
      _false_node(false_node),
      CDFG_Element(CDFG_Operator::eType::Icmp,
                   2, // num input
                   state,
                   step)
  {}
  ~CDFG_IcmpElem(void) {}

  // override
  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;

private:
  const eCond _cond; ///< 比較条件
  const std::shared_ptr<CDFG_Node> _true_node;  ///< True定数ノード
  const std::shared_ptr<CDFG_Node> _false_node; ///< False定数ノード

  std::string _get_condition_code(void);
};

#endif
