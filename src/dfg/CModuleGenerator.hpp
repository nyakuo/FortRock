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
  void add_node(std::shared_ptr<CDFG_Node> & node);
  void add_operator(std::shared_ptr<CDFG_Operator> & ope);
  void add_element(std::shared_ptr<CDFG_Element> & elem);
  bool find_node(const std::string & node_name);
  bool find_node(std::shared_ptr<CDFG_Node> & node);
  std::shared_ptr<CDFG_Node> get_node(const std::string & node_name);

private:
  COutput _cout;
  std::shared_ptr<CModule> _module;

  void _generate_test_data(void);
  void _generate_header(void);
  void _generate_define(void);
  void _generate_assign(void);
  void _generate_calculator(void);
  void _generate_always(void);
  void _generate_footer(void);
};

#endif
