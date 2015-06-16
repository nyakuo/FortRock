#include "CModule.hpp"

/**
   コンストラクタ
   @brief モジュールの名前を設定する
   @param[in] module_name モジュールの名前
 */
CModule::CModule
(const std::string & module_name)
  : _module_name(module_name)
{}

/**
   ElementをDFGに追加
   @param[in] element DFGに追加するElementの参照
 */
void CModule::add_element
(std::shared_ptr<CDFG_Element> & element) {
  this->_dfg.emplace_back(element);
}

/**
   Nodeを追加
   @param[in] node モジュールに追加するNodeの参照
 */
void CModule::add_node
(std::shared_ptr<CDFG_Node> & node) {
  this->_node_list.emplace_back(node);
}

/**
   Operatorを追加
   @param[in] ope モジュールに追加するOperatorの参照
 */
void CModule::add_operator
(std::shared_ptr<CDFG_Operator> & ope) {
  this->_operator_list.emplace_back(ope);
}

/**
   モジュールの名前を取得
   @return モジュール名
 */
std::string & CModule::get_name(void) {
  return this->_module_name;
}

/**
   モジュールのElementのリスト(DFG)を取得
   @return モジュールのElementのリスト(DFG)
 */
std::list<std::shared_ptr<CDFG_Element> > &
CModule::get_element_list(void) {
  return this->_dfg;
}

/**
   モジュールのNodeのリストを取得
   @return モジュールのNodeのリスト
 */
std::list<std::shared_ptr<CDFG_Node> > &
CModule::get_node_list(void) {
  return this->_node_list;
}

/**
   モジュールの演算器のリストを取得
   @return モジュールの演算器のリスト
 */
std::list<std::shared_ptr<CDFG_Operator> > &
CModule::get_operator_list(void) {
  return this->_operator_list;
}

/**
   モジュールのステートのNodeの取得
   @node this->_node_list を検索して
         見つからなければ新規作成し追加する
 */
// std::shared_ptr<CDFG_Node>
// CModule::get_state_node(void) {
// }

/**
   モジュールのステップのNodeの取得
   @node this->_node_list を検索して
         見つからなければ新規作成し追加する
 */
// std::shared_ptr<CDFG_Node>
// CModule::get_step_node(void) {
// }
