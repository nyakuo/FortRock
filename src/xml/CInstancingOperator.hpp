#ifndef _CINSTANCING_OPERATOR_H
#define _CINSTANCING_OPERATOR_H

#include <string>
#include <sstream>
#include <algorithm>
#include <memory>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

#include "COperatorGenerator.hpp"
#include "CNode_data.hpp"
#include "../dfg/CModuleGenerator.hpp"
#include "../dfg/CDFG_Operator.hpp"
#include "../dfg/CDFG_Node.hpp"
#include "../dfg/CDFG_Ram.hpp"

class CModuleGenerator;

/**
   @class CInstancingOperator
   @breif XMlをパースするクラス
          演算器の設定や各種設定の入力などに使う
*/
class CInstancingOperator {
public:
  CInstancingOperator(void);
  ~CInstancingOperator(void);

  void instancing_operators
  (std::shared_ptr<CModuleGenerator> & module_gen,
   const std::string & filename);

private:
  void _parse_operator_info
  (const xmlDocPtr & doc,
   const xmlNodePtr & node,
   std::shared_ptr<CModuleGenerator> & module_gen);

  void _parse_ram_info
  (const xmlDocPtr & doc,
   const xmlNodePtr & node);
};

#endif
