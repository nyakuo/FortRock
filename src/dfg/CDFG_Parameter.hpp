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
                CDFG_Node::eNode::PARAM)
  { this->set_parameter(parameter); }
  ~CDFG_Parameter(void) {}

  // setter
  void set_parameter(const double & parameter);

  // getter
  long get_parameter(void);
  double get_fparameter(void);
  eParamType get_type(void) { return this->_param_type; }

  virtual std::string to_string(void) override final;

private:
  long _parameter;
  double _fparameter;
  eParamType _param_type;
};

#endif
