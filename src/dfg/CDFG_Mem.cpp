#include "CDFG_Mem.hpp"

/**
   コンストラクタ
   @brief データを格納する動的配列を確保
   @param[in] name インスタンス名
   @param[in] num_datas メモリ上のデータ数
   @param[in] word_length ワード長
   @param[in] num_w_port 書き込みポート数
   @param[in] num_r_port 読み込みポート数
   @param[in] mem_type メモリタイプ
   @param[in] data_type データタイプ
   @param[in] is_initialized 初期化されるかどうか
   @param[in] latency アクセスにかかるレイテンシ
*/
CDFG_Mem::CDFG_Mem
(const std::string & name,
 const unsigned & num_datas,
 const unsigned & word_length,
 const unsigned & num_w_port,
 const unsigned & num_r_port,
 const eMemType & mem_type,
 const eDataType & data_type,
 const bool & is_initialized,
 const unsigned & latency)
  : _num_datas(num_datas),
    _word_length(word_length),
    _mem_type(mem_type),
    _data_type(data_type),
    _is_initialized(is_initialized),
    _latency(latency),
    CDFG_Node(name,
              word_length,
              true, // is_signed
              CDFG_Node::eNode::MEM,
              "m_")
{
  // 書き込みポートのインスタンス化
  for (auto i=0; i<num_w_port; ++i) {
    auto w_port =
      std::make_shared<CDFG_Reg>
      (name + "_" + std::to_string(i),
       word_length,
       true,
       CDFG_Reg::eRegType::REG);

    this->_write_ports.emplace_back(w_port);
  }

  // 読み出しポートのインスタンス化
  for (auto i=0; i<num_w_port; ++i) {
    auto r_port =
      std::make_shared<CDFG_Wire>
      (name + "_" + std::to_string(i),
       word_length,
       true,
       CDFG_Wire::eWireType::WIRE);

    this->_read_ports.emplace_back(r_port);
  }
} // CDFG_Mem

/**
   メモリの種類の取得
   @return メモリの種類
 */
const CDFG_Mem::eMemType &
CDFG_Mem::get_mem_type(void)
{ return this->_mem_type; } // get_mem_type

/**
   書き込みポート数の取得
   @return 書き込みポート数
 */
unsigned
CDFG_Mem::get_num_w_port(void)
{ return this->_write_ports.size(); } // get_num_w_port

/**
   読み込みポート数の取得
   @return 読み込みポート数
 */
unsigned
CDFG_Mem::get_num_r_port(void)
{ return this->_read_ports.size(); } // get_num_r_port

/**
   読み出しポートの取得
   @param[in] port_num 取得するポートのvector内での位置
   @return 読み出しポートの参照
 */
std::shared_ptr<CDFG_Node>
CDFG_Mem::get_read_port
(const unsigned & port_num) {
  return this->_read_ports.at(port_num);
} // get_read_port

/**
   書き込みポートの取得
   @param[in] port_num 取得するポートのvector内での位置
   @return 書き込みポートの参照
 */
std::shared_ptr<CDFG_Node>
CDFG_Mem::get_write_port
(const unsigned & port_num) {
  return this->_write_ports.at(port_num);
} // get_write_port

/**
   メモリのアクセスレイテンシの取得
   @return メモリのアクセスレイテンシ
 */
const unsigned &
CDFG_Mem::get_latency(void) {
  return this->_latency;
} // get_latency
