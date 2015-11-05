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
(const std::shared_ptr<CDFG_Element> element)
{
  this->_dfg.emplace_back(element);
}

/**
   Nodeを追加
   @param[in] node モジュールに追加するNodeの参照
 */
void CModule::add_node
(const std::shared_ptr<CDFG_Node> & node)
{
  this->_node_list.emplace_back(node);
}

/**
   Operatorを追加
   @param[in] ope モジュールに追加するOperatorの参照
 */
void CModule::add_operator
(std::shared_ptr<CDFG_Operator> & ope)
{
  this->_operator_list.emplace_back(ope);
}

/**
   モジュールの名前を取得
   @return モジュール名
 */
std::string & CModule::get_name(void)
{
  return this->_module_name;
}

/**
   モジュールのElementのリスト(DFG)を取得
   @return モジュールのElementのリスト(DFG)
 */
std::list<std::shared_ptr<CDFG_Element> > &
CModule::get_element_list(void)
{
  this->_dfg.sort([](const std::shared_ptr<CDFG_Element> & obj1,
                     const std::shared_ptr<CDFG_Element> & obj2) -> bool
                  {
                    return obj1->get_state() < obj2->get_state()
                      && obj1->get_step() < obj2->get_step();
                  });
  return this->_dfg;
}

/**
   モジュールのNodeのリストを取得
   @return モジュールのNodeのリスト
 */
std::list<std::shared_ptr<CDFG_Node> > &
CModule::get_node_list(void)
{
  return this->_node_list;
}

/**
   モジュールの演算器のリストを取得
   @return モジュールの演算器のリスト
 */
std::list<std::shared_ptr<CDFG_Operator> > &
CModule::get_operator_list(void)
{
  return this->_operator_list;
}

/**
   モジュール内のノードの検索
   @param[in] type ノードの種類
   @note state, step, clkなどシステムに関する信号の取得に利用
   @return 検索結果のノード
 */
std::shared_ptr<CDFG_Node> &
CModule::get_node
(const CDFG_Node::eNode & type)
{
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [type](const std::shared_ptr<CDFG_Node> & obj) -> bool
                 {
                   return obj->get_type() == type;
                 });
  return *ite;
}

/**
   指定された種類のラベルを取得
   @param[in] type ラベルの種類
   @return ラベルノード
 */
std::shared_ptr<CDFG_Node> &
CModule::get_node
(const CDFG_Label::eLabelType & type)
{
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [type](const std::shared_ptr<CDFG_Node> & obj) -> bool
                 {
                   return obj->get_type() == CDFG_Node::eNode::LABEL
                     && std::dynamic_pointer_cast<CDFG_Label>
                     (obj)->get_type() == type;
                 });
  return *ite;
}

/**
   指定された種類の定数ノードを取得
   @param[in] type 定数ノードの種類
   @return 定数ノード
 */
std::shared_ptr<CDFG_Node> &
CModule::get_node
(const CDFG_Parameter::eParamType & type)
{
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [type](const std::shared_ptr<CDFG_Node> & obj) -> bool
                 {
                   return obj->get_type() == CDFG_Node::eNode::PARAM
                     && std::dynamic_pointer_cast<CDFG_Parameter>
                     (obj)->get_type() == type;
                 });
  return *ite;
}

/**
   指定された種類のregノードを取得
   @param[in] type regノードの種類
   @return regノード
 */
std::shared_ptr<CDFG_Node> &
CModule::get_node
(const CDFG_Reg::eRegType & type)
{
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [type](const std::shared_ptr<CDFG_Node> & obj) -> bool
                 {
                   return obj->get_type() == CDFG_Node::eNode::REG
                     && std::dynamic_pointer_cast<CDFG_Reg>
                     (obj)->get_type() == type;
                 });
  return *ite;
}

/**
   指定された種類のwireノードを取得
   @param[in] type wireノードの種類
   @return wireノード
 */
std::shared_ptr<CDFG_Node> &
CModule::get_node
(const CDFG_Wire::eWireType & type)
{
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [type](const std::shared_ptr<CDFG_Node> & obj) -> bool
                 {
                   return obj->get_type() == CDFG_Node::eNode::WIRE
                     && std::dynamic_pointer_cast<CDFG_Wire>
                     (obj)->get_type() == type;
                 });
  return *ite;
}

/**
   モジュール内のノードの検索
   @param[in] node_name ノードの名前
   @param[in] type ノードの種類
   @return 検索結果のノード
*/
std::shared_ptr<CDFG_Node> &
CModule::get_node
(const std::string & node_name,
 const CDFG_Node::eNode & type)
{
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [node_name, type]
                 (const std::shared_ptr<CDFG_Node> & obj) -> bool
                 {
                   return type == obj->get_type()
                     && obj->get_asm_name() == node_name;
                 });
  return *ite;
}

/**
   指定されたステップに対応するラベルNodeを取得する
   @param[in] state ステート番号
   @return ラベルNode
*/
std::shared_ptr<CDFG_Node> &
CModule::get_label_node
(const unsigned & state)
{
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [state](const std::shared_ptr<CDFG_Node> & obj) -> bool
                 {
                   return (obj->get_type() == CDFG_Node::eNode::LABEL)
                     && (std::dynamic_pointer_cast<CDFG_Label>
                         (obj)->get_state() == state);
                 });
  return *ite;
}

/**
   モジュール内のノードの検索
   @param[in] asm_name ノードLLVM IR上での名前
   @param[in] type ノードの種類
   @return 検索結果 T/F
 */
bool
CModule::find_node(const std::string & asm_name,
                   const CDFG_Node::eNode & type)
{
  auto ite =
    std::find_if(this->_node_list.begin(),
                 this->_node_list.end(),
                 [asm_name, type](const std::shared_ptr<CDFG_Node> & obj) -> bool
                 {
                   return obj->get_asm_name() == asm_name
                     && obj->get_type() == type;
                 });
  return ite != this->_node_list.end();
}

/**
   モジュールの最大ステップを取得
   @note step信号を定義する際にのビット幅を
         調べるためにFortRockが使用
   @return step信号に必要なビット幅
 */
unsigned
CModule::get_max_step(void)
{
  unsigned max_step = 0;

  for (auto & obj : this->_dfg)
    if (max_step < obj->get_step())
      max_step = obj->get_step();

  return max_step;
}
