#ifndef _CDFG_REGNODE_H
#define _CDFG_REGNODE_H

#include <string>
#include "CDFG_Node.hpp"

/**
   @class CDFG_Reg
   @brief Regを表すクラス
 */
class CDFG_Reg : public CDFG_Node {
public:
  /** 信号線の種類定数 */
  enum class eRegType : unsigned {
    Reg = 0,
      In_copy,
      Fin,
      Out,
      State,
      Prev_state,
      Step,
      };

  /**
     コンストラクタ
     @param[in] asm_name アセンブリ上での信号名
     @param[in] bit_width 信号線のビット幅
     @param[in] is_signed 信号線がsignedかどうか
     @param[in] type 信号線の種類
   */
  CDFG_Reg(const std::string & asm_name,
           const unsigned & bit_width,
           const bool & is_signed,
           const eRegType & type)
    : _type(type),
      CDFG_Node(asm_name,
                bit_width,
                is_signed,
                CDFG_Node::eType::Reg,
                this->_get_prefix(type))
  {}

  // getter
  eRegType get_type(void) { return this->_type; }
  virtual bool is_input(void) override final;
  virtual bool is_output(void) override final;

private:
  eRegType _type; ///< 信号線の種類

  std::string _get_prefix(const eRegType & type);
};

#endif
