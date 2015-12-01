#ifndef _CDFG_ARRAY_H
#define _CDFG_ARRAY_H

#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include "CDFG_Addr.hpp"
#include "CDFG_Mem.hpp"
#include "CDFG_Parameter.hpp"

/**
   @class CDFG_Array
   @brief メモリに対する配列アクセスを可能にするクラス
   @todo 浮動小数点対応
 */
class CDFG_Array : public CDFG_Mem {
public:
  /**
     コンストラクタ
     @param[in] name インスタンス名
     @param[in] type 格納されるデータ型
     @param[in] word_length ワード長
     @param[in] write_ports 書き込みポート数 (同時書き込み数)
     @param[in] read_ports 読み込みポート数 (同時読み込み数)
     @param[in] length 配列の長さ
  */
  CDFG_Array(const std::string & name,
             const CDFG_Mem::eDataType & type,
             const unsigned & word_length,
             const unsigned & write_ports,
             const unsigned & read_ports,
             const std::vector<unsigned> & length);

  /**
     コンストラクタ
     @param[in] name インスタンス名
     @param[in] type 格納されるデータ型
     @param[in] write_ports 書き込みポート数 (同時書き込み数)
     @param[in] read_ports 読み込みポート数 (同時読み込み数)
     @param[in] length 配列の長さ
     @param[in] initializer 初期化子
     @todo 浮動小数点対応
  */
  CDFG_Array(const std::string & name,
             const eDataType & type,
             const unsigned & word_length,
             const unsigned & write_ports,
             const unsigned & read_ports,
             const std::vector<unsigned> & length,
             const std::vector<std::vector<int> > & initializer);

  ~CDFG_Array(void) {}

  // override
  virtual std::string init_string
  (const std::string & indent) override final;
  virtual std::string define_string(void) override final;
  virtual std::string access_string
  (const std::shared_ptr<CDFG_Addr> & addr) override final;

  // getter
  /**
     配列の次元数の取得
     @return 配列の次元数
   */
  unsigned get_dimension(void) { return this->_length.size(); }

private:
  const std::vector<unsigned>    _length;   ///< 配列の各次元での長さ
  std::vector<std::vector<int> > _int_data; ///< 整数型データ格納場所
};

#endif
