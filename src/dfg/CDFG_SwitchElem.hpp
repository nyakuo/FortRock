#ifndef _CDFG_SWITCHELEM_H
#define _CDFG_SWITCHELEM_H

#include <string>
#include <memory>
#include <vector>

#include "CDFG_Element.hpp"
#include "CDFG_Operator.hpp"
#include "CDFG_Node.hpp"

/**
   @class CDFG_SwitchElem
   @brief Switch命令を表すクラス
 */
class CDFG_SwitchElem : public CDFG_Element {
public:
  CDFG_SwitchElem(const unsigned & num_input,
                  const std::shared_ptr<CDFG_Node> & state_node,
                  const std::shared_ptr<CDFG_Node> & prev_state,
                  const std::shared_ptr<CDFG_Node> & step_node,
                  const std::shared_ptr<CDFG_Node> & default_label,
                  const std::shared_ptr<CDFG_Node> & cond_node,
                  const unsigned & state,
                  const unsigned & step);

  // Setter
  void set_condition_at(const std::shared_ptr<CDFG_Node> & cond,
                        const unsigned & at);

  // Getter
  std::string get_condition_code(void);
  std::string get_condition_at(const unsigned & at);
  std::string get_default_label(void);

  // Override
  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string input_to_str(const unsigned & at=0) override final;
  virtual std::string output_to_str(void) override final;
  virtual std::string other_str(const std::string & indent) override final;

private:
  const std::shared_ptr<CDFG_Node> _condition_node; ///< 条件ノード
  const std::shared_ptr<CDFG_Node> _default_label;  ///< defaultラベル
  const std::shared_ptr<CDFG_Node> _state_node;     ///< ステートノード
  const std::shared_ptr<CDFG_Node> _prev_state;     ///< ステートノード
  const std::shared_ptr<CDFG_Node> _step_node;      ///< ステップノード
  std::vector<std::shared_ptr<CDFG_Node> > _conditions; ///< 遷移条件のリスト
  std::vector<std::shared_ptr<CDFG_Node> > _to; ///< 遷移先のリスト
};

#endif
