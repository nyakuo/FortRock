#ifndef _CDFG_WIRE_H
#define _CDFG_WIRE_H

#include <string>
#include "CDFG_Node.hpp"

/**
   @class CDFG_Wire
   @brief Wire信号を表すクラス
 */
class CDFG_Wire : public CDFG_Node {
public:
  /**
     Wire信号の種類を表す定数
   */
  enum class eWireType : unsigned {
    WIRE = 0,  ///< 回路内部で使用するwire信号
      CLK,     ///< clock信号
      RES,     ///< reset信号
      REQ,     ///< request信号
      CE,      ///< clock enable信号
      IN_ORIG, ///< 入力信号
  };

  /**
     コンストラクタ
     @param[in] asm_name アセンブリ上での信号名
     @param[in] bit_width 信号のビット幅
     @param[in] is_signed 信号がsignedかどうか
     @param[in] type 信号の種類
   */
  CDFG_Wire(const std::string & asm_name,
            const unsigned & bit_width,
            const bool & is_signed,
            const eWireType & type)
    : _type(type),
      CDFG_Node(asm_name,
                bit_width,
                is_signed,
                CDFG_Node::eNode::WIRE,
                this->_get_prefix(type))
  {}

  // getter
  eWireType get_type(void) { return this->_type; }
  virtual bool is_input(void) override final;
  virtual bool is_output(void) override final;

private:
  eWireType _type; ///< 信号の種類

  std::string _get_prefix(const eWireType & type);
};

#endif
