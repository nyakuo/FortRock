#ifndef _CDFG_RETELEM_H
#define _CDFG_RETELEM_H

#include <memory>

#include "CDFG_Element.hpp"
#include "CDFG_RetElem.hpp"
#include "CDFG_Operator.hpp"
#include "CDFG_Label.hpp"

/**
   @class CDFG_RetElem
   @brief ret命令を表すクラス
 */
class CDFG_RetElem : public CDFG_Element {
public:
  CDFG_RetElem(const std::shared_ptr<CDFG_Node> & finish_state,
               const std::shared_ptr<CDFG_Node> & state_node,
               const std::shared_ptr<CDFG_Node> & step_node,
               const std::shared_ptr<CDFG_Node> & prev_state,
               const std::shared_ptr<CDFG_Node> & fin_signal,
               const std::shared_ptr<CDFG_Node> & true_signal,
               const unsigned & state,
               const unsigned & step);

  std::string finish_state_str(const std::string & indent);
  unsigned get_finish_state(void);

  // Override
  virtual std::string other_str(const std::string & indent) override final;

  //private:
  const std::shared_ptr<CDFG_Label> _finish_state; ///< 終了ステートラベルの参照
  const std::shared_ptr<CDFG_Node> _state_node;    ///< stateノードの参照
  const std::shared_ptr<CDFG_Node> _prev_state;    ///< prev stateノードの参照
  const std::shared_ptr<CDFG_Node> _step_node;     ///< stepノードの参照
  const std::shared_ptr<CDFG_Node> _fin_signal;    ///< 終了信号の参照
  const std::shared_ptr<CDFG_Node> _true_signal;   ///< True信号の参照
};

#endif
