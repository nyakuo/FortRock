#include "CDFG_Ram.hpp"

// static
const std::string CDFG_Ram::_ram_module_name = "ram";
unsigned CDFG_Ram::_num_instance = 0;
unsigned CDFG_Ram::_num_r_port = 2;
unsigned CDFG_Ram::_num_w_port = 2;

/**
   コンストラクタ
   @brief インスタンス数の増加
   @param[in] name インスタンス名
   @param[in] num_datas データ数
   @param[in] word_length ワード長
   @param[in] address_width アドレス幅
   @param[in] num_w_port 書き込みポート数
   @param[in] num_r_port 読み込みポート数
   @param[in] data_type データタイプ
   @param[in] is_initialized 初期化されるかどうか
   @param[in] latency アクセスにかかるレイテンシ
 */
CDFG_Ram::CDFG_Ram
(const std::string & name,
 const unsigned & num_datas,
 const unsigned & word_length,
 const unsigned & address_width,
 const eDataType & data_type,
 const bool & is_initialized,
 const unsigned & latency)
  : _address_width(address_width),
    CDFG_Mem(name,
             num_datas,
             word_length,
             _num_w_port,
             _num_r_port,
             CDFG_Mem::eMemType::Ram,
             data_type,
             is_initialized,
             latency)
{
  // インスタンス数の更新
  ++CDFG_Ram::_num_instance;

  // アドレスポートのインスタンス化
  for (auto i=0; i<_num_w_port; ++i)
    {
      auto a_port =
        std::make_shared<CDFG_Reg>
        (name + "_addr_" + std::to_string(i),
         word_length,
         false,
         CDFG_Reg::eRegType::Reg);

      this->_address_ports.emplace_back(a_port);
    }

  // r/wポートのインスタンス化
  for (auto i=0; i<_num_w_port; ++i)
    {
      auto rw_port =
        std::make_shared<CDFG_Reg>
        (name + "_rw_" + std::to_string(i),
         1,
         false,
         CDFG_Reg::eRegType::Reg);

      this->_rw_ports.emplace_back(rw_port);
    }
} // CDFG_Ram

/**
   メモリの初期化部を出力する
   @param[in] indent 現在のインデント
   @todo 実装
*/
std::string
CDFG_Ram::init_string
(const std::string & indent)
{
  std::string ret_str("");
  return "";

  if (this->_is_initialized)
    if (this->_data_type
        == CDFG_Mem::eDataType::Integer)
      ret_str.append(indent);

  return ret_str;
} // init_string

/**
   RAMモジュールの宣言文の出力
   @return RAMモジュールの宣言文
*/
std::string
CDFG_Ram::define_string
(void) {
  std::string ret_str("");

  // Ram モジュールのインスタンス化
  ret_str.append
    (_ram_module_name
     + " "
     + this->_verilog_name
     + "(");

  for (auto i=0; i<this->get_num_w_port(); ++i )
    ret_str.append
      ("  .write" + std::to_string(i)
       + "("
       + this->get_write_port(i)->get_verilog_name()
       + "),\n");

  for (auto i=0; i<this->get_num_r_port(); ++i )
    ret_str.append
      ("  .read" + std::to_string(i)
       + "("
       + this->get_read_port(i)->get_verilog_name()
       + "),\n");

  for (auto i=0; i<this->get_num_addr_port(); ++i)
    ret_str.append
      ("  .addr" + std::to_string(i)
       + "("
       + this->get_address_port(i)->get_verilog_name()
       + "),\n");

  for (auto i=0; i<this->get_num_rw_port(); ++i)
    ret_str.append
      ("  .rw" + std::to_string(i)
       + "("
       + this->get_rw_port(i)->get_verilog_name()
       + "),\n");

  ret_str.append("  .clk(i_w_clk));\n\n");

  // ポートのインスタンス化
  {
    for (auto & w_port : this->_write_ports)
      ret_str.append
        ("  reg ["
         + std::to_string(w_port->get_bit_width() - 1)
         + ":0] "
         + w_port->get_verilog_name()
         + ";\n");

    for (auto & r_port : this->_read_ports)
      ret_str.append
        ("  wire ["
         + std::to_string(r_port->get_bit_width() - 1)
         + ":0] "
         + r_port->get_verilog_name()
         + ";\n");

    for (auto & addr : this->_address_ports)
      ret_str.append
        ("  reg ["
         + std::to_string(addr->get_bit_width() - 1)
         + ":0] "
         + addr->get_verilog_name()
         + ";\n");

    for (auto & rw : this->_rw_ports)
      ret_str.append
        ("  reg ["
         + std::to_string(rw->get_bit_width() - 1)
         + ":0] "
         + rw->get_verilog_name()
         + ";\n");
  }

  return ret_str;
} // define_string

/**
   ポートにアクセスする
   @param[in] addr アクセスするポートへの参照
   @note get_address() は最後尾が最新のアドレス (一次元の場合)
   @todo 多次元配列に対応
 */
std::string
CDFG_Ram::access_string
(const std::shared_ptr<CDFG_Addr> & addr)
{
  auto dim = addr->get_addr_dim();
  return addr->get_address(dim-1)->get_verilog_name();
} // access_string

/**
   入出力ポートの数を設定
   @param[in] num_r_port 読み出しポート数
   @param[in] num_w_port 書き込みポート数
 */
void CDFG_Ram::set_num_port
(const unsigned & num_r_port,
 const unsigned & num_w_port)
{
  _num_r_port = num_r_port;
  _num_w_port = num_w_port;
} // set_num_port

/**
   アドレスポートの取得
   @param[in] at アドレスポートの位置
 */
std::shared_ptr<CDFG_Reg>
CDFG_Ram::get_address_port
(const unsigned & at)
{
  return this->_address_ports.at(at);
} // get_addres_port

/**
   r/wポートの取得
   @param[in] at r/wポートの位置
 */
std::shared_ptr<CDFG_Reg>
CDFG_Ram::get_rw_port
(const unsigned & at)
{
  return this->_rw_ports.at(at);
} // get_rw_port

/**
   アドレスポートの数を取得
   @return アドレスポートの数
 */
unsigned
CDFG_Ram::get_num_addr_port
(void) {
  return this->_address_ports.size();
} // get_num_addr_port

/**
   r/wポートの数を取得する
   @return r/wポートの数
 */
unsigned
CDFG_Ram::get_num_rw_port
(void) {
  return this->_rw_ports.size();
} // get_num_rw_port
