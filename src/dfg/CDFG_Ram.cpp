#include "CDFG_Ram.hpp"
#include <iostream> // for debug
// static
const std::string CDFG_Ram::_RAM_MODULE_NAME = "ram";
unsigned CDFG_Ram::_NUM_INSTANCE = 0;
unsigned CDFG_Ram::_NUM_R_PORT = 2;
unsigned CDFG_Ram::_NUM_W_PORT = 2;

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
             _NUM_W_PORT,
             _NUM_R_PORT,
             CDFG_Mem::eMemType::RAM,
             data_type,
             is_initialized,
             latency)
{
  // インスタンス数の更新
  ++CDFG_Ram::_NUM_INSTANCE;

  // アドレスポートのインスタンス化
  for (auto i=0; i<_NUM_W_PORT; ++i) {
    auto a_port =
      std::make_shared<CDFG_Reg>
      (name + "_addr_" + std::to_string(i),
       word_length,
       false,
       CDFG_Reg::eRegType::REG);

    this->_address_ports.emplace_back(a_port);
  }

  // r/wポートのインスタンス化
  for (auto i=0; i<_NUM_W_PORT; ++i) {
    auto rw_port =
      std::make_shared<CDFG_Reg>
      (name + "_rw_" + std::to_string(i),
       1,
       false,
       CDFG_Reg::eRegType::REG);

    this->_rw_ports.emplace_back(rw_port);
  }

#if 0
  // xmlファイルを用いて入出力ポートのインスタンス化
  auto doc = xmlParseFile(FortRock::_OPERATOR_CONFIG_FILENAME.c_str());
  try {
    if (doc == NULL)
      throw std::string ("ERROR: can not open config file");
  }
  catch(std::string e) {
    std::cerr << e << std::endl;
    return;
  }

  auto cur = xmlDocGetRootElement(doc);
  try {
    if (cur == NULL)
      throw std::string ("ERROR: config file is empty");
  }
  catch (std::string e) {
    xmlFreeDoc(doc);
    std::cerr << e << std::endl;
    return;
  }

  // RAMの情報をパース
  unsigned num_read_port, num_write_port;
  {
    // IPコアの項目までパース
    cur = cur->xmlChildrenNode;
    while (xmlStrcmp(cur->name, (const xmlChar*)"ip_core"))
      cur = cur->next;

    while (cur != NULL) {
      // RAM以外
      if (xmlStrcmp(cur->name, (const xmlChar*)"ram") != 0) {
        cur = cur->next;
        continue;
      }
      auto mod_info = cur->xmlChildrenNode;
      while (mod_info != NULL) {
        auto key = xmlNodeListGetString(doc, mod_info->xmlChildrenNode, 1);
        std::stringstream ss;
        ss << key;
        auto str = ss.str();
        str.erase(std::remove(str.begin(),
                              str.end(),
                              ' '),
                  str.end());

        if (xmlStrcmp(mod_info->name, (const xmlChar*)"module_name") == 0) {
          this->_asm_name = str;
          this->_verilog_name = str;
        }

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"num_read_port") == 0)
          num_read_port = std::stoul(str);

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"num_write_port") == 0)
          num_write_port = std::stoul(str);

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"word_length") == 0)
          this->_word_length = std::stoul(str);

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"address_width") == 0)
          this->_address_width = std::stoul(str);

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"num_datas") == 0)
          this->_num_datas = std::stoul(str);

        else {
          mod_info = mod_info->next;
          continue;
        }
      } // while : mod_info
    } // while : cur
  } // RAMの情報をパース
#endif
} // CDFG_Ram


/**
   メモリの初期化部を出力する
   @param[in] indent 現在のインデント
   @todo 浮動小数点対応
*/
std::string
CDFG_Ram::init_string
(const std::string & indent) {
  std::string ret_str("");

  if (this->_is_initialized) {
    if (this->_data_type
        == CDFG_Mem::eDataType::INTEGER) {
      ret_str.append(indent); // todo
    } // if : this->_data_type
  } // if : this->_initialized

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

  ret_str.append
    (_RAM_MODULE_NAME
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

  ret_str.append(".clk(clk));\n");

  return ret_str;
} // define_string

std::string
CDFG_Ram::access_string
(const std::shared_ptr<CDFG_Addr> & addr)
{
  std::string ret_str("access string");
  return ret_str;
} // access_string

/**
   入出力ポートの数を設定
   @param[in] num_r_port 読み出しポート数
   @param[in] num_w_port 書き込みポート数
 */
void CDFG_Ram::set_num_port
(const unsigned & num_r_port,
 const unsigned & num_w_port) {
  _NUM_R_PORT = num_r_port;
  _NUM_W_PORT = num_w_port;
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
