#include "CDFG_Parameter.hpp"

// static値の初期化
const unsigned CDFG_Parameter::Single_fp_bit_width = 32;
const unsigned CDFG_Parameter::Double_fp_bit_width = 64;
const bool CDFG_Parameter::Is_positive_logic = true;

/**
   整数型のコンストラクタ
   @param[in] asm_name LLVM IRでの変数名
   @param[in] bit_width ビット幅
   @param[in] is_signed signedかどうか
   @param[in] param_type 定数の種類
   @param[in] parameter 定数
*/
CDFG_Parameter::CDFG_Parameter
(const std::string & asm_name,
 const unsigned    & bit_width,
 const long        & parameter)
  : _param_type(eParamType::Integer),
    CDFG_Node(asm_name,
              bit_width,
              true, // is signed
              CDFG_Node::eNode::Param,
              "p_")
{
  this->_data.param = parameter;
}

/**
   倍精度浮動小数点型のコンストラクタ
   @param[in] asm_name LLVM IRでの変数名
   @param[in] parameter 定数
*/
CDFG_Parameter::CDFG_Parameter
(const std::string & asm_name,
 const double      & parameter)
  : _param_type(eParamType::Double),
    CDFG_Node(asm_name,
              CDFG_Parameter::Double_fp_bit_width,
              true, // 浮動小数点は常にsigned
              CDFG_Node::eNode::Param,
              "pf_")
{
  this->_data.lfparam = parameter;
}

/**
   単精度浮動小数点型のコンストラクタ
   @param[in] asm_name LLVM IRでの変数名
   @param[in] parameter 定数
*/
CDFG_Parameter::CDFG_Parameter
(const std::string & asm_name,
 const float       & parameter)
  : _param_type(eParamType::Float),
    CDFG_Node(asm_name,
              CDFG_Parameter::Single_fp_bit_width,
              true, // 浮動小数点は常にsigned
              CDFG_Node::eNode::Param,
              "pf_")
{
  this->_data.fparam = parameter;
}

/**
   bool型のコンストラクタ
   @param[in] asm_name LLVM IRでの変数名
   @param[in] parameter 定数
*/
CDFG_Parameter::CDFG_Parameter
(const std::string & asm_name,
 const bool        & parameter)
  : _param_type(eParamType::Bool),
    CDFG_Node(asm_name,
              1, // bit width
              false,
              CDFG_Node::eNode::Param,
              "pb_")
{
  this->_data.param = (long)parameter;
} // CDFG_Parameter

/**
   定数型(true, false, zero)のコンストラクタ
   @param[in] asm_name LLVM IRでの変数名
   @param[in] param_type 定数の種類
*/
CDFG_Parameter::CDFG_Parameter
(const std::string & asm_name,
 const eParamType  & param_type)
  : _param_type(param_type),
    CDFG_Node(asm_name,
              1, // bit width
              true, // signed
              CDFG_Node::eNode::Param,
              "pb_")
{
  switch (param_type) {
  case eParamType::True:
    if (CDFG_Parameter::Is_positive_logic)
      this->_data.param = 1L;
    else
      this->_data.param = 0L;
    break;

  case eParamType::False:
    if (CDFG_Parameter::Is_positive_logic)
      this->_data.param = 0L;
    else
      this->_data.param = 1L;
    break;

  case eParamType::Zero:
    this->_data.param = 0L;
    break;

  default:
    std::cerr << "ERROR("
              << __FILE__
              << "): コンストラクタの呼び出しが無効です"
              << std::endl;
  } // switch
} // CDFG_Parameter

/**
   ノードに設定された定数を整数型で取得する
   @return ノードに設定された定数 (整数型)
*/
long
CDFG_Parameter::get_parameter
(void) const
{
  return this->_data.param;
}

/**
   ノードに設定された定数を倍精度浮動小数点型で取得する
   @return ノードに設定された定数 (倍精度浮動小数点型)
*/
double
CDFG_Parameter::get_lfparameter
(void) const
{
  return this->_data.lfparam;
}

/**
   ノードに設定された定数を浮動小数点型で取得する
   @return ノードに設定された定数 (浮動小数点型)
*/
float
CDFG_Parameter::get_fparameter
(void) const
{
  return this->_data.fparam;
}

/**
   パラメータの文字列をビット幅指定を含めて取得
   @return パラメータの文字列
   @note パラメータの形式(double, long)に応じて
   出力が変化
*/
std::string
CDFG_Parameter::to_string
(void)
{
  std::string ret ("");
  char buf[64];

  // 負の数
  if (this->_param_type == eParamType::Integer
       && this->_data.param < 0)
    ret = "-";

  // ビット幅
  ret += std::to_string(this->_bit_width)
    + "'h";

  switch (this->_param_type) {
  case eParamType::Integer:
  case eParamType::True:
  case eParamType::False:
  case eParamType::Bool:
  case eParamType::Zero:
    snprintf(buf, sizeof(buf), "%lx"
             , std::abs(this->_data.param));
    ret += buf;
    break;

  case eParamType::Float:
  case eParamType::Double:
    for (int i=this->_bit_width / 8 - 1 * ((this->_bit_width % 8) == 0);
         i >= 0;
         --i)
      {
        snprintf(buf, sizeof(buf)
                 , "%02x", this->_data.c[i]);
        ret += buf;
      }
    break;

  default:;
  }

  return ret;
} // to_string
