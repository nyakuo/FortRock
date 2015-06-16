#ifndef _CMODULE_H
#define _CMODULE_H

#include <string>
#include <list>
#include <memory>

#include "CDFG_Element.hpp"
#include "CDFG_Node.hpp"
#include "CDFG_Operator.hpp"

/**
   @class CModule
   @breif モジュールの情報を表すデータクラス
*/
class CModule {
public:
  CModule(const std::string & module_name);

  void add_element(std::shared_ptr<CDFG_Element> & element);
  void add_node(std::shared_ptr<CDFG_Node> & node);
  void add_operator(std::shared_ptr<CDFG_Operator> & ope);

  // getter
  std::string & get_name(void);
  std::list<std::shared_ptr<CDFG_Node> > & get_node_list(void);
  std::list<std::shared_ptr<CDFG_Element> > & get_element_list(void);
  std::list<std::shared_ptr<CDFG_Operator> > & get_operator_list(void);

//  std::shared_ptr<CDFG_Node> get_state_node(void);
//  std::shared_ptr<CDFG_Node> get_step_node(void);

private:
  std::string _module_name; /** モジュール名 */

  std::list<std::shared_ptr<CDFG_Element> > _dfg;            /** モジュール内のElementのリスト*/
  std::list<std::shared_ptr<CDFG_Node> > _node_list;         /** モジュール内のNodeのリスト */
  std::list<std::shared_ptr<CDFG_Operator> > _operator_list; /** モジュール内の演算モジュールリスト */

  void _make_operator_node(std::shared_ptr<CDFG_Operator> & ope);
};

#endif
