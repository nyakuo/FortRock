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
   モジュール内のノードの検索
   @param[in] type ノードの種類
   @note state, step, clkなどシステムに関する信号の取得に利用
   @return 検索結果のノード
 */
std::shared_ptr<CDFG_Node> &
CModule::get_node(CDFG_Node::eNode type) {
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [type](std::shared_ptr<CDFG_Node> obj) -> bool {
                   return obj->get_type() == type;
                 }
                 );
  return *ite;
}

/**
   モジュール内のノードの検索
   @param[in] node_name ノードの名前
   @return 検索結果のノード
 */
std::shared_ptr<CDFG_Node> &
CModule::get_node(const std::string & node_name) {
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [node_name](std::shared_ptr<CDFG_Node> obj) -> bool {
                   return obj->get_asm_name() == node_name;
                 }
                 );
  return *ite;
}

/**
   モジュール内のノードの検索
   @param[in] asm_name ノードLLVM IR上での名前
   @return 検索結果 T/F
 */
bool
CModule::find_node(const std::string & asm_name) {
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [asm_name](std::shared_ptr<CDFG_Node> obj) -> bool {
                   return obj->get_asm_name() == asm_name;
                 }
                 );
  return ite != this->_node_list.end();
}
