#ifndef _CDFG_TRUNCELEM_H
#define _CDFG_TRUNCELEM_H

#include "CDFG_Element.hpp"

/**
   @class CDFG_TruncElem
   @brief trunc命令を表す Elementクラス
 */
class CDFG_TruncElem : public CDFG_Element {
public:
  CDFG_TruncElem(const unsigned & bit_width,
                 const unsigned & state,
                 const unsigned & step);
  ~CDFG_TruncElem(void) {}

  /**
     変換後のビット幅を取得
     @return 変換後のビット幅
  */
  const unsigned & get_dest_bit_width(void)
  { return this->_bit_width; }

  // override
  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string input_to_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;

private:
  const unsigned _bit_width; ///< 変更後のビット幅
};

#endif
