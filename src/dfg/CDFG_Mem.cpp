#include "CDFG_Mem.hpp"

/**
   コンストラクタ
   @brief データを格納する動的配列を確保
   @param[in] name インスタンス名
   @param[in] num_datas メモリ上のデータ数
   @param[in] word_length ワード長
   @param[in] write_ports 書き込みポート数 (同時書き込み数)
   @param[in] read_ports 読み込みポート数 (同時読み込み数)
*/
CDFG_Mem::CDFG_Mem
(const std::string & name,
 const unsigned & num_datas,
 const unsigned & word_length,
 const unsigned & write_ports,
 const unsigned & read_ports,
 const eMemType & mem_type)
  : _num_datas(num_datas),
    _word_length(word_length),
    _write_ports(write_ports),
    _read_ports(read_ports),
    _mem_type(mem_type),
    CDFG_Node(name,
              word_length,
              false, // is_signed
              CDFG_Node::eNode::MEM,
              "m_")
{
  this->_int_array.reserve(num_datas);
}

/**
   コンストラクタ (初期化子を用いた)
   @param[in] name インスタンス名
   @param[in] init_array 初期化子
   @param[in] word_length ワード長
   @param[in] write_ports 書き込みポート数 (同時書き込み数)
   @param[in] read_ports 読み込みポート数 (同時読み込み数)
 */
CDFG_Mem::CDFG_Mem
(const std::string & name,
 const std::vector<int> & init_array,
 const unsigned & word_length,
 const unsigned & write_ports,
 const unsigned & read_ports,
 const eMemType & mem_type)
  : _num_datas(init_array.size()),
    _word_length(word_length),
    _write_ports(write_ports),
    _read_ports(read_ports),
    _mem_type(mem_type),
    CDFG_Node(name,
              word_length,
              false, // is_signed
              CDFG_Node::eNode::MEM,
              "m_")
{
  this->_int_array.reserve(init_array.size());
  this->_int_init = init_array;
}
