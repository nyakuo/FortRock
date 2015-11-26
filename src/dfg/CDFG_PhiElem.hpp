#ifndef _CDFG_PHIELEM_H
#define _CDFG_PHIELEM_H

#include <vector>
#include <memory>

#include "CDFG_Node.hpp"
#include "CDFG_Element.hpp"
#include "CDFG_Operator.hpp"

/**
   @class CDFG_PhiElem
   @brief Phi命令を表すクラス
 */
class CDFG_PhiElem : public CDFG_Element {
public:
  CDFG_PhiElem(const unsigned & num_input,
               const std::shared_ptr<CDFG_Node> prev_state,
               const unsigned & state,
               const unsigned & step);

  // Setter
  void set_label(const std::shared_ptr<CDFG_Node> & node,
                 const unsigned & at);

  // Getter
  const std::shared_ptr<CDFG_Node> & get_prev_state(void) const;
  const std::string get_label_str(const unsigned & at) const;

  // Override
  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string input_to_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;

private:
  std::vector<std::shared_ptr<CDFG_Node> > _labels;
  std::shared_ptr<CDFG_Node> _prev_state;
};

#endif
