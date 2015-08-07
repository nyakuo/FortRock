#ifndef _CDFG_MEM_H
#define _CDFG_MEM_H

#include <string>
#include <vector>
#include <memory>
#include "CDFG_Node.hpp"
#include "CDFG_Addr.hpp"

class CDFG_Addr; // 循環参照によるクラスの重複定義対応

/**
   @class CDFG_Mem
   @brief メモリを表すクラス
   配列やスタックなどのインタフェースモジュールが
   実際にアクセスするデータを表すクラス
 */
class CDFG_Mem : public CDFG_Node {
public:
  /** メモリの種類を表す定数 */
  enum class eMemType : unsigned {
    ARRAY = 0, ///< 配列
      OTHER,   ///< その他
  };

  CDFG_Mem(const std::string & name,
           const unsigned & num_datas,
           const unsigned & word_length,
           const unsigned & write_ports,
           const unsigned & read_ports,
           const eMemType & mem_type);

  CDFG_Mem(const std::string & name,
           const std::vector<int> & init_array,
           const unsigned & word_length,
           const unsigned & wrire_ports,
           const unsigned & read_ports,
           const eMemType & mem_type);

  ~CDFG_Mem(void) {}

  // インタフェース依存の部分の出力関数 (Verilog HDL)
  virtual std::string init_string(void) = 0;   ///< 初期化部の出力
  virtual std::string define_string(void) = 0; ///< 宣言部の出力
  virtual std::string access_string(const std::shared_ptr<CDFG_Addr> & addr) = 0; ///< アクセス部(制御部も含む)の出力

  // getter
  eMemType get_mem_type(void) { return this->_mem_type; }

  // overrides
  virtual bool is_input(void) override final { return false; }
  virtual bool is_output(void) override final { return false; }

protected:
  const unsigned _num_datas;   ///< メモリ上のデータ数
  const unsigned _word_length; ///< ワード長
  const unsigned _write_ports; ///< 書き込みポート数 (同時書き込み数)
  const unsigned _read_ports;  ///< 読み込みポート数 (同時読み込み数)
  std::vector<int> _int_array; ///< 整数型のデータ格納場所
  std::vector<int> _int_init;  ///< 整数型の初期化子
  const eMemType _mem_type;    ///< メモリの種類
};

#endif
