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

  CModuleGenerator(const std::string & filename);

private:
  COutput _cout;
  std::shared_ptr<CModule> _module;

  // std::list<std::shared_ptr<CDFG_Element> > _dfg;            /** モジュール内のElementのリスト */
  // std::list<std::shared_ptr<CDFG_Node> > _node_list;         /** モジュール内のNodeのリスト */
  // std::list<std::shared_ptr<CDFG_Operator> > _operator_list; /** モジュール内の演算モジュールリスト */
  void _generate_header(void);
  void _generate_define(void);
  void _generate_assign(void);
  void _generate_calculator(void);
  void _generate_always(void);
  void _generate_footer(void);
};


#endif
