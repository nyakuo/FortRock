#ifndef _CDFG_LABEL_H
#define _CDFG_LABEL_H

#include <string>
#include "CDFG_Node.hpp"

/**
   @class CDFG_Label
   @brief ラベルを表すクラス
 */
class CDFG_Label : public CDFG_Node {
public:
  enum eLabelType {
    Finish = 0,  ///< 終了ステート
    Label,       ///< その他のステート
  };

  CDFG_Label(const std::string & asm_name,
             const unsigned & bit_width,
             const eLabelType & type,
             const unsigned & state)
    : _label_type(type),
      _label_state(state),
      CDFG_Node(asm_name,
                bit_width,
                false, // == is signed
                CDFG_Node::eNode::Label,
                "l_") {}

  ~CDFG_Label(void) {}

  // getter
  eLabelType get_type(void) { return this->_label_type; }
  unsigned get_state(void) { return this->_label_state; }
  virtual bool is_input(void) override final { return false; }
  virtual bool is_output(void) override final { return false; }
  virtual std::string to_string(void) override final;

private:
  const eLabelType _label_type;  ///< ラベルの種類
  const unsigned   _label_state; ///< ラベルのステート番号
};

#endif
