#include "CDFG_Array.hpp"

/**
   コンストラクタ
   @param[in] name インスタンス名
   @param[in] word_length ワード長
   @param[in] write_ports 書き込みポート数 (同時書き込み数)
   @param[in] read_ports 読み込みポート数 (同時読み込み数)
   @param[in] length 配列の長さ
   @param[in] dimension 配列の次元数
*/
CDFG_Array::CDFG_Array
(const std::string & name,
 const unsigned & word_length,
 const unsigned & write_ports,
 const unsigned & read_ports,
 const unsigned & length,
 const unsigned & dimension)
  : _length(length),
    _dimension(dimension),
    CDFG_Mem(name,
             length * dimension,
             word_length,
             write_ports,
             read_ports,
             CDFG_Mem::eMemType::ARRAY) {}

/**
   コンストラクタ
   @param[in] name インスタンス名
   @param[in] write_ports 書き込みポート数 (同時書き込み数)
   @param[in] read_ports 読み込みポート数 (同時読み込み数)
   @param[in] length 配列の長さ
   @param[in] initializer 初期化子
   @note 1次元配列のみ対応
*/
CDFG_Array::CDFG_Array
(const std::string & name,
 const unsigned & word_length,
 const unsigned & write_ports,
 const unsigned & read_ports,
 const std::vector<int> & initializer)
  : _length(initializer.size()),
    _dimension(1),
    CDFG_Mem(name,
             initializer,
             word_length,
             write_ports,
             read_ports,
             CDFG_Mem::eMemType::ARRAY) {}

/**
   配列の初期化部の文字列を出力する
   @todo すべて0で初期化する場合などはforループで実装する
*/
std::string
CDFG_Array::init_string(void) {
  std::string ret_str("");

  for (auto i=0; i<this->_num_datas; ++i)
    ret_str.append(this->_verilog_name
                   + "["
                   + std::to_string(i)
                   + "] <= "
                   + std::to_string(this->_int_init[i])
                   + ";\n");

  ret_str.append("end\n");

  return ret_str;
}

/**
   メモリの指定したアドレスへのアクセス記述(Verilog)を出力
   @param[in] addr 参照するアドレス
   @return メモリの指定したアドレスへのアクセス記述(Verilog)
 */
std::string CDFG_Array::access_string
(const std::shared_ptr<CDFG_Addr> & addr)
{
  std::string ret_str(this->get_verilog_name());

  for (auto i=0; i<addr->get_addr_dim();
       ++i) {
    auto address = addr->get_address(i);
    if (address->get_type()
        == CDFG_Node::eNode::REG)
      ret_str.append("["
                     + address->get_verilog_name()
                     + "]");

    else if (address->get_type()
             == CDFG_Node::eNode::PARAM) {
      auto param
        = std::dynamic_pointer_cast
        <CDFG_Parameter>(address);
      ret_str.append("["
                     + std::to_string(param->get_parameter())
                     + "]");
    }
  } // for

  return ret_str;
}
