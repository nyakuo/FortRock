#ifndef _CDFG_TRUNCELEM_H
#define _CDFG_TRUNCELEM_H

#include "CDFG_Element.hpp"

/**
   @class CDFG_TruncElem
   @brief trunc命令を表す Elementクラス
 */
class CDFG_TruncElem : public CDFG_Element {
public:
  /**
     コンストラクタ
     @param[in] bit_width 変換後のビット幅
     @param[in] state 命令を実行するステート
     @param[in] step 命令を実行するステップ
   */
  CDFG_TruncElem(const unsigned & bit_width,
                 const unsigned & state,
                 const unsigned & step)
    : _bit_width(bit_width),
      CDFG_Element(CDFG_Operator::eType::TRUNC,
                   1, // num input
                   state,
                   step)
  {}
  ~CDFG_TruncElem(void) {}

  /**
     変換後のビット幅を取得
     @return 変換後のビット幅
  */
  const unsigned & get_dest_bit_width(void)
  { return this->_bit_width; }

private:
  const unsigned _bit_width; //! 変更後のビット幅
};

#endif
