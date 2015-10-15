#include "CDFG_Array.hpp"

/**
   コンストラクタ
   @param[in] name インスタンス名
   @param[in] type 格納されるデータ型
   @param[in] word_length ワード長
   @param[in] write_ports 書き込みポート数 (同時書き込み数)
   @param[in] read_ports 読み込みポート数 (同時読み込み数)
   @param[in] length 配列の長さ
*/
CDFG_Array::CDFG_Array
(const std::string & name,
 const CDFG_Mem::eDataType & type,
 const unsigned & word_length,
 const unsigned & write_ports,
 const unsigned & read_ports,
 const std::vector<unsigned> & length)
  : _length(length),
    CDFG_Mem(name,
             std::accumulate(length.begin(),
                             length.end(), 0),
             word_length,
             write_ports,
             read_ports,
             CDFG_Mem::eMemType::ARRAY,
             type,
             false) {}

/**
   コンストラクタ
   @param[in] name インスタンス名
   @param[in] type 格納されるデータ型
   @param[in] write_ports 書き込みポート数 (同時書き込み数)
   @param[in] read_ports 読み込みポート数 (同時読み込み数)
   @param[in] length 配列の長さ
   @param[in] initializer 初期化子
*/
CDFG_Array::CDFG_Array
(const std::string & name,
 const CDFG_Mem::eDataType & type,
 const unsigned & word_length,
 const unsigned & write_ports,
 const unsigned & read_ports,
 const std::vector<unsigned> & length,
 const std::vector<std::vector<int> > & initializer)
  : _length(length),
    CDFG_Mem(name,
             std::accumulate(length.begin(),
                             length.end(), 0),
             word_length,
             write_ports,
             read_ports,
             CDFG_Mem::eMemType::ARRAY,
             type,
             true)
{
  // メモリの初期化
  //! @todo 多次元配列への対応
  this->_int_data.resize(initializer.size());

  for (auto & vec : initializer) {
    this->_int_data.reserve(vec.size());
    for (auto & val : vec)
      this->_int_data[0].push_back(val);
  } // for : vec
}

/**
   配列の初期化部の文字列を出力する
   @todo すべて0で初期化する場合などはforループで実装する
*/
std::string
CDFG_Array::init_string
(const std::string & indent) {
  std::string ret_str("");

  if (this->_is_initialized) {
    if (this->_data_type
        == CDFG_Array::eDataType::INTEGER) {
      for (auto i=0; i<this->_length[0]; ++i)
        ret_str.append(indent
                       + this->_verilog_name
                       + "["
                       + std::to_string(i)
                       + "] <= "
                       + std::to_string(this->_int_data[0][i])
                       + ";\n");
    } // if : this->_data_type
  } // if : this->_is_initialized
  return ret_str;
}

/**
   配列の宣言文(Verilog)の出力
   @return 配列の宣言文(Verilog)
   @todo 一次元配列のみ対応
 */
std::string
CDFG_Array::define_string
(void) {
  std::string ret_str ("");

  for (auto dim=0;
       dim < 1;
       ++dim) {
    ret_str.append("reg ["
                   + std::to_string(this->_word_length - 1)
                   + ":0] "
                   + this->_verilog_name
                   + "[0:"
                   + std::to_string(this->_length[dim] - 1)
                   + "];\n");
  } // for : dim

  return ret_str;
}

/**
   メモリの指定したアドレスへのアクセス記述(Verilog)を出力
   @param[in] addr 参照するアドレス
   @return メモリの指定したアドレスへのアクセス記述(Verilog)
 */
std::string
CDFG_Array::access_string
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
