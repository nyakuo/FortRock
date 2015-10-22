#ifndef _CDFG_MEM_H
#define _CDFG_MEM_H

#include <string>
#include <list>
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
      RAM,     ///< RAM
      OTHER,   ///< その他
      };

  enum class eDataType : unsigned {
    INTEGER = 0, ///< 整数型
      FLOAT,     ///< 浮動小数点型
  };

  CDFG_Mem(const std::string & name,
           const unsigned & num_datas,
           const unsigned & word_length,
           const eMemType & mem_type,
           const eDataType & data_type,
           const bool & is_initialized);

  virtual ~CDFG_Mem(void) {}

  // インタフェース依存の部分の出力関数 (Verilog HDL)
  virtual std::string init_string
  (const std::string & indent) = 0;   ///< 初期化部の出力
  virtual std::string define_string(void) = 0; ///< 宣言部の出力
  virtual std::string access_string
  (const std::shared_ptr<CDFG_Addr> & addr) = 0; ///< アクセス部(制御部も含む)の出力

  // getter
  eMemType get_mem_type(void);
  unsigned get_num_w_port(void);
  unsigned get_num_r_port(void);

  // overrides
  virtual bool is_input(void) override final { return false; }
  virtual bool is_output(void) override final { return false; }

protected:
  const unsigned _num_datas;   ///< メモリ上のデータ数
  const unsigned _word_length; ///< ワード長
  std::list<std::shared_ptr<CDFG_Node> > _write_ports; ///< 書き込みポート
  std::list<std::shared_ptr<CDFG_Node> > _read_ports;  ///< 読み込みポート
  const eMemType _mem_type;    ///< メモリの種類
  const bool _is_initialized;  ///< 初期化されるかどうか
  const eDataType _data_type;  ///< 格納されるデータ型
};

#endif
