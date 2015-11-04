#ifndef _CMODULE_GENERATOR_H
#define _CMODULE_GENERATOR_H

#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <array>
#include <algorithm>

#include "CDFG_Element.hpp"
#include "CDFG_IcmpElem.hpp"
#include "CDFG_FcmpElem.hpp"
#include "CDFG_TruncElem.hpp"
#include "CDFG_Node.hpp"
#include "CDFG_Parameter.hpp"
#include "CDFG_Label.hpp"
#include "CDFG_Wire.hpp"
#include "CDFG_Reg.hpp"
#include "CDFG_Array.hpp"
#include "CDFG_Ram.hpp"
#include "CDFG_Mem.hpp"
#include "CDFG_Addr.hpp"
#include "CDFG_Operator.hpp"
#include "CDFG_Scheduler.hpp"
#include "CStateMachineGen.hpp"
#include "COutput.hpp"
#include "CModule.hpp"


/**
   @class CModuleGenerator
   @brief モジュールを出力するクラス
 */
class CModuleGenerator {
public:
  int generate(void);

  CModuleGenerator(const std::string & filename,
                   const std::string & module_name);

  // FortRock本体のためのアクセサ
  // setter
  void add_node(const std::shared_ptr<CDFG_Node> & node);
  void add_operator(std::shared_ptr<CDFG_Operator> & ope);
  void add_element(const std::shared_ptr<CDFG_Element> elem);

  // getter
  bool find_node(const std::string & node_name,
                 const CDFG_Node::eNode & type);
  bool find_node(std::shared_ptr<CDFG_Node> node);
  std::shared_ptr<CDFG_Node>
  get_node(const std::string & node_name,
           const CDFG_Node::eNode & type);
  std::shared_ptr<CDFG_Node> get_node(const CDFG_Node::eNode & type);
  std::shared_ptr<CDFG_Node> get_node(const CDFG_Label::eLabelType & type);
  std::shared_ptr<CDFG_Node> get_node(const CDFG_Parameter::eParamType & type);
  std::shared_ptr<CDFG_Node> get_node(const CDFG_Reg::eRegType & type);
  std::shared_ptr<CDFG_Node> get_node(const CDFG_Wire::eWireType & type);
  std::shared_ptr<CDFG_Operator> get_operator(const CDFG_Operator::eType & type);
  unsigned get_max_step(void);

  // for debug
  std::list<std::shared_ptr<CDFG_Node> > &
  get_node_list(void) { return this->_module->get_node_list(); }

private:
  COutput _cout;
  std::shared_ptr<CModule> _module;

  void _generate_test_data(void);
  void _generate_header(void);
  void _generate_define(void);
  void _generate_define_array(void);
  void _generate_assign(void);
  void _generate_function(void);
  void _generate_calculator(void);
  void _generate_always(void);
  void _generate_footer(void);
};

#endif
