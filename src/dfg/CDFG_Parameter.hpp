#ifndef _CDFG_PARAMETER_H
#define _CDFG_PARAMETER_H

#include <string>

#include "CDFG_Node.hpp"

/**
   @class CDFG_Parameter
   @brief 定数ノードを表すクラス
 */
class CDFG_Parameter : public CDFG_Node {
public:
  /** 定数の種類を表す定数 */
  enum eParamType {
    INTEGER = 1 << 0,
    FLOAT   = 1 << 1,
    TRUE    = 1 << 2,
    FALSE   = 1 << 3,
    BOOL    = (TRUE | FALSE),
    ZERO    = 1 << 4,
  };

  CDFG_Parameter(const std::string & asm_name,
                 const unsigned & bit_width,
                 const bool & is_signed,
                 const eParamType & param_type,
                 const double & parameter)
    : _param_type(param_type),
      CDFG_Node(asm_name,
                bit_width,
                is_signed,
                CDFG_Node::eNode::PARAM,
                "p_")
  { this->set_parameter(parameter); }
  ~CDFG_Parameter(void) {}

  // setter
  void set_parameter(const double & parameter);

  // getter
  long get_parameter(void);
  double get_fparameter(void);
  eParamType get_type(void) { return this->_param_type; }
  virtual std::string to_string(void) override final;
  virtual bool is_input(void) override final { return false; }
  virtual bool is_output(void) override final { return false; }

private:
  long        _parameter;  //! 整数の定数値
  double      _fparameter; //! 浮動小数点の定数値
  eParamType  _param_type; //! 定数の種類
};

#endif
