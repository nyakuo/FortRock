
#ifndef _CDFG_LABEL_H
#define _CDFG_LABEL_H

#include "CDFG_Node.hpp"

/**
   @class CDFG_Label
   @brief ラベルを表すクラス
 */
class CDFG_Label : public CDFG_Node {
public:
  enum eLabelType {
    FINISH = 0,
      LABEL,
  };

  CDFG_Label(const std::string & asm_name,
             const unsigned & bit_width,
             const eLabelType & type,
             const unsigned & state)
    : _label_type(type),
      _label_state(state),
      CDFG_Node(asm_name,
                bit_width,
                false, // is signed
                CDFG_Node::eNode::LABEL) {}

  ~CDFG_Label(void) {}

  eLabelType get_type(void) { return this->_label_type; }
  unsigned get_state(void) { return this->_label_state; }

private:
  const eLabelType _label_type; //! ラベルの種類
  const unsigned _label_state;  //! ラベルのステート番号
};

#endif
