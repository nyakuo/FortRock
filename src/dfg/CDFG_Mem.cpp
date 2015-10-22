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
 const eMemType & mem_type,
 const eDataType & data_type,
 const bool & is_initialized)
  : _num_datas(num_datas),
    _word_length(word_length),
    _mem_type(mem_type),
    _is_initialized(is_initialized),
    _data_type(data_type),
    CDFG_Node(name,
              word_length,
              false, // is_signed
              CDFG_Node::eNode::MEM,
              "m_")
{}


/**
   メモリの種類の取得
   @return メモリの種類
 */
CDFG_Mem::eMemType
CDFG_Mem::get_mem_type(void)
{ return this->_mem_type; }

/**
   書き込みポート数の取得
   @return 書き込みポート数
 */
unsigned
CDFG_Mem::get_num_w_port(void)
{ return this->_write_ports.size(); }

/**
   読み込みポート数の取得
   @return 読み込みポート数
 */
unsigned
CDFG_Mem::get_num_r_port(void)
{ return this->_read_ports.size(); }
