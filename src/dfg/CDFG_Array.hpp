#ifndef _CDFG_ARRAY_H
#define _CDFG_ARRAY_H

#include <vector>
#include <string>
#include <memory>
#include "CDFG_Addr.hpp"
#include "CDFG_Mem.hpp"
#include "CDFG_Parameter.hpp"

/**
   @class CDFG_Array
   @brief メモリに対する配列アクセスを可能にするクラス
 */
class CDFG_Array : public CDFG_Mem {
public:
  /**
     コンストラクタ
     @param[in] name インスタンス名
     @param[in] word_length ワード長
     @param[in] write_ports 書き込みポート数 (同時書き込み数)
     @param[in] read_ports 読み込みポート数 (同時読み込み数)
     @param[in] length 配列の長さ
     @param[in] dimension 配列の次元数
  */
  CDFG_Array(const std::string & name,
             const unsigned & word_length,
             const unsigned & write_ports,
             const unsigned & read_ports,
             const unsigned & length,
             const unsigned & dimension);

  /**
     コンストラクタ
     @param[in] name インスタンス名
     @param[in] write_ports 書き込みポート数 (同時書き込み数)
     @param[in] read_ports 読み込みポート数 (同時読み込み数)
     @param[in] length 配列の長さ
     @param[in] initializer 初期化子
  */
  CDFG_Array(const std::string & name,
             const unsigned & word_length,
             const unsigned & write_ports,
             const unsigned & read_ports,
             const std::vector<int> & initializer);

  ~CDFG_Array(void) {}

  virtual std::string init_string(void) override final;
  virtual std::string define_string(void) override final {return "";}
  virtual std::string access_string
  (const std::shared_ptr<CDFG_Addr> & addr) override final;

private:
  const unsigned _dimension; ///< 配列の次元数
  const unsigned _length;    ///< 配列の長さ
};

#endif
