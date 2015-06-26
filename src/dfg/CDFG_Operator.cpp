#include "CDFG_Operator.hpp"

CDFG_Operator::CDFG_Operator(const std::string & name,
                             const std::string & mod_name,
                             const unsigned & latency,
                             const CDFG_Operator::eType & type)
  : _name(name),
    _mod_name(mod_name),
    _latency(latency),
    _type(type)
 {}

CDFG_Operator::~CDFG_Operator(void) {}

/**
   moduleのインスタンス名の取得
   @return moduleのインスタンス名
 */
std::string
CDFG_Operator::get_name(void) const {
  return this->_name;
}

/**
   modulemのモジュール名の取得
   @return modulemのモジュール名
 */
std::string
CDFG_Operator::get_mod_name(void) const {
  return this->_mod_name;
}

/**
   moduleのレイテンシの取得
   @return moduleのレイテンシ
*/
unsigned
CDFG_Operator::get_latency(void) const {
  return this->_latency;
}

/**
   moduleの種類の取得
   @return moduleの種類
*/
CDFG_Operator::eType
CDFG_Operator::get_type(void) const {
  return this->_type;
}

/**
   moduleの入力の数を取得
   @return 0 Portが定義されていない, それ以外 moduleの入力の数
*/
unsigned
CDFG_Operator::get_num_input(void) const {
  if (this->_input_list.empty())
    return 0;

  return this->_input_list.size();
}

/**
   moduleの出力の数を取得
   @return 0 Portが定義されていない, それ以外 moduleの出力の数
*/
unsigned
CDFG_Operator::get_num_output(void) const {
  if (this->_output_list.empty())
    return 0;

  return this->_output_list.size();
}

/**
   入力信号の接続名の取得
   @param[in] at 取得する入力信号の位置
   @return 入力信号の接続名
 */
std::string
CDFG_Operator::get_input_signal_at
(const unsigned & at) const {
  return this->_input_list.at(at).first;
}

/**
   出力信号の接続名の取得
   @param[in] at 取得する出力信号の位置
   @return 出力信号の接続名
 */
std::string
CDFG_Operator::get_output_signal_at
(const unsigned & at) const {
  return this->_output_list.at(at).first;
}

/**
   入力信号の参照の取得
   @param[in] at 取得する入力信号の位置
   @return 入力の参照
*/
std::shared_ptr<CDFG_Node>
CDFG_Operator::get_input_node_at
(const unsigned & at) const {
  return this->_input_list.at(at).second;
}

/**
   出力ポートの参照の取得
   @param[in] at 取得する入力ポートの位置
   @return 入力ポートの参照
*/
std::shared_ptr<CDFG_Node>
CDFG_Operator::get_output_node_at
(const unsigned & at) const {
  return this->_output_list.at(at).second;
}

/**
   入力ポートの追加
   @param[in] port_name 追加する入力信号の名前
   @param[in] node 追加する入力信号の参照
*/
void CDFG_Operator::add_input_port
(const std::string & port_name,
 std::shared_ptr<CDFG_Node> & node) {
  this->_input_list.push_back
    (std::pair<std::string, std::shared_ptr<CDFG_Node> >
     (port_name, node));
}

/**
   出力ポートの追加
   @param[in] port_name 追加する出漁k信号の名前
   @param[in] node 追加する出力信号の参照
*/
void CDFG_Operator::add_output_port
(const std::string & port_name,
 std::shared_ptr<CDFG_Node> & node) {
  this->_output_list.push_back
    (std::pair<std::string, std::shared_ptr<CDFG_Node> >
     (port_name, node));
}

/**
   レイテンシのセット
   @param[in] latency moduleのレイテンシ
*/
void CDFG_Operator::set_latency
(const unsigned & latency) {
  this->_latency = latency;
}

/**
   moduleの種類のセット
   @param[in] type moduleの種類
 */
void CDFG_Operator::set_type
(const CDFG_Operator::eType & type) {
  this->_type = type;
}

/**
   演算器の入力の数を変更する
   @param[in] num 変更後の演算器の入力の数
   @note phony演算器(演算器を使用しない演算)で使用
 */
void CDFG_Operator::set_num_input
(const unsigned & num) {
  this->_input_list.resize(num);
}
