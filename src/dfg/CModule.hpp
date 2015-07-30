#ifndef _CMODULE_H
#define _CMODULE_H

#include <string>
#include <list>
#include <memory>
#include <algorithm>

#include "CDFG_Element.hpp"
#include "CDFG_Node.hpp"
#include "CDFG_Label.hpp"
#include "CDFG_Parameter.hpp"
#include "CDFG_Operator.hpp"

/**
   @class CModule
   @brief モジュールの情報を表すデータクラス
*/
class CModule {
public:
  CModule(const std::string & module_name);

  // 資源の登録
  void add_element(const std::shared_ptr<CDFG_Element> element);
  void add_node(const std::shared_ptr<CDFG_Node> & node);
  void add_operator(std::shared_ptr<CDFG_Operator> & ope);

  // getter
  std::string & get_name(void);
  std::list<std::shared_ptr<CDFG_Node> > & get_node_list(void);
  std::list<std::shared_ptr<CDFG_Element> > & get_element_list(void);
  std::list<std::shared_ptr<CDFG_Operator> > & get_operator_list(void);
  std::shared_ptr<CDFG_Node> & get_node(const CDFG_Node::eNode & type);
  std::shared_ptr<CDFG_Node> & get_node(const CDFG_Label::eLabelType & type);
  std::shared_ptr<CDFG_Node> & get_node(const CDFG_Parameter::eParamType & type);
  std::shared_ptr<CDFG_Node> & get_node(const std::string & node_name);
  std::shared_ptr<CDFG_Node> & get_label_node(const unsigned & state);
  bool find_node(const std::string & asm_name);
  unsigned get_max_step(void);

private:
  std::string _module_name; //! モジュール名

  std::list<std::shared_ptr<CDFG_Element> > _dfg;            //! モジュール内のElementのリスト
  std::list<std::shared_ptr<CDFG_Node> > _node_list;         //! モジュール内のNodeのリスト
  std::list<std::shared_ptr<CDFG_Operator> > _operator_list; //! モジュール内の演算モジュールリスト
};

#endif
