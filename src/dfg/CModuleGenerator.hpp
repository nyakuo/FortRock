#ifndef _CMODULE_GENERATOR_H
#define _CMODULE_GENERATOR_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <array>
#include <algorithm>

#include "CDFG_Element.hpp"
#include "CDFG_Node.hpp"
#include "CDFG_Parameter.hpp"
#include "CDFG_Label.hpp"
#include "CDFG_IcmpElem.hpp"
#include "CDFG_Operator.hpp"
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
  bool find_node(const std::string & node_name);
  bool find_node(std::shared_ptr<CDFG_Node> node);
  std::shared_ptr<CDFG_Node> get_node(const std::string & node_name);
  std::shared_ptr<CDFG_Node> get_node(const CDFG_Node::eNode & type);
  std::shared_ptr<CDFG_Node> get_node(const CDFG_Label::eLabelType & type);
  std::shared_ptr<CDFG_Node> get_node(const CDFG_Parameter::eParamType & type);
  std::shared_ptr<CDFG_Operator> get_operator(const CDFG_Operator::eType & type);
  unsigned get_max_step(void);

private:
  COutput _cout;
  std::shared_ptr<CModule> _module;

  void _generate_test_data(void);
  void _generate_header(void);
  void _generate_define(void);
  void _generate_assign(void);
  void _generate_function(void);
  void _generate_calculator(void);
  void _generate_always(void);
  void _generate_footer(void);
};

#endif
