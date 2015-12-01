#ifndef _CDFG_PARAMETER_H
#define _CDFG_PARAMETER_H

#include <iostream> // for error message
#include <string>
#include <sstream>

#include "CDFG_Node.hpp"

/**
   @class CDFG_Parameter
   @brief 定数ノードを表すクラス
 */
class CDFG_Parameter : public CDFG_Node {
public:
  //! 単精度浮動小数点のビット幅
  static const unsigned Single_fp_bit_width;

  //! 倍精度浮動小数点のビット幅
  static const unsigned Double_fp_bit_width;

  //! 回路が正論理かどうか
  //! @brief TRUE/FALSE の0/1に影響
  static const bool Is_positive_logic;

  /** 定数の種類を表す定数 */
  enum eParamType {
    Integer = 1 << 0,
    Float   = 1 << 1,
    Double  = 1 << 2,
    True    = 1 << 3,
    False   = 1 << 4,
    Bool    = (True | False),
    Zero    = 1 << 5,
  };

  //! 整数型のコンストラクタ
  CDFG_Parameter(const std::string & asm_name,
                 const unsigned    & bit_width,
                 const long        & parameter);

  //! 倍精度浮動小数点型のコンストラクタ
  CDFG_Parameter(const std::string & asm_name,
                 const double      & parameter);

  //! 単精度浮動小数点型のコンストラクタ
  CDFG_Parameter(const std::string & asm_name,
                 const float       & parameter);

  //! bool型のコンストラクタ
  CDFG_Parameter(const std::string & asm_name,
                 const bool        & parameter);

  //! 定数型(true, false, zero)のコンストラクタ
  CDFG_Parameter(const std::string & asm_name,
                 const eParamType  & param_type);

  // Getter
  long get_parameter(void) const;
  double get_lfparameter(void) const;
  float get_fparameter(void) const;
  eParamType get_type(void) const { return this->_param_type; }

  // Override
  virtual std::string to_string(void) override final;
  virtual bool is_input(void) override final { return false; }
  virtual bool is_output(void) override final { return false; }

private:
  //! 定数値
  union data {
    long   param;   ///< 整数の定数値
    double lfparam; ///< 倍精度浮動小数点の定数値
    float  fparam;  ///< 浮動小数点の定数値
    char   c[8];    ///< 1バイト毎にアクセスするため
  };

  union data _data;       ///< 定数値
  const eParamType _param_type; ///< 定数の種類
};
#endif
