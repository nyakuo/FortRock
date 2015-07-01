#include "CInstancingOperator.hpp"

/**
   コンストラクタ
 */
CInstancingOperator::CInstancingOperator(void) {}

/**
   デストラクタ
 */
CInstancingOperator::~CInstancingOperator(void) {}

/**
   回路全体で使用する演算器のインスタンス化
   @param[in] module_gen 回路全体の演算器を管理するクラスのインスタンス
   @param[in] filename 演算器の設定ファイル名
 */
void CInstancingOperator::instancing_operators
(std::shared_ptr<CModuleGenerator> & module_gen,
 const std::string & filename) {
  // xmlファイルのオープン
  auto doc = xmlParseFile(filename.c_str());
  try {
    if (doc == NULL)
      throw std::string ("ERROR: can not open xml file");
  }
  catch (std::string e) {
    std::cerr << e << std::endl;
    return;
  }

  auto cur = xmlDocGetRootElement(doc);
  try {
    if (cur == NULL)
      throw std::string ("ERROR: empty document");
  }
  catch (std::string e) {
    xmlFreeDoc(doc);
    std::cerr << e << std::endl;
    return;
  }

  // IPCoreの項目までパース
  cur = cur->xmlChildrenNode;
  while (xmlStrcmp(cur->name, (const xmlChar*)"IPCore"))
    cur = cur->next;


  // 演算器の情報をパースする
   this->_parse_operator_info(doc, cur, module_gen);
}

/**
   演算器の情報をパースし，演算器をインスタンス化する
 */
void CInstancingOperator::_parse_operator_info
(xmlDocPtr & doc, xmlNodePtr & cur,
 std::shared_ptr<CModuleGenerator> & module_gen) {
  cur = cur->xmlChildrenNode;

  bool use_ip_core;
  unsigned latency;
  std::string mod_name;
  unsigned num_module;
  std::string clock_port_name;
  std::string left_operator_port_name;
  std::string right_operator_port_name;
  std::string out_port_name;
  unsigned left_port_bit_width;
  unsigned right_port_bit_width;
  unsigned out_port_bit_width;
  std::string clock_enable_port_name;

  CDFG_Operator::eType type;

  // 演算器のパース
  while (cur != NULL) {
    //! @todo addInt以外への対応
    if (xmlStrcmp(cur->name, (const xmlChar*)"addInt") == 0)
      type = CDFG_Operator::eType::ADD;
    else if (xmlStrcmp(cur->name, (const xmlChar*)"mul") == 0)
      type = CDFG_Operator::eType::MUL;
    else if (xmlStrcmp(cur->name, (const xmlChar*)"sRem") == 0)
      type = CDFG_Operator::eType::SREM;
    else if (xmlStrcmp(cur->name, (const xmlChar*)"sDiv") == 0)
      type = CDFG_Operator::eType::SDIV;
    else {
      cur = cur->next;
      continue;
    }

    // 演算器の情報のパース
    auto mod_info = cur->xmlChildrenNode;
    while (mod_info != NULL) {
      if (xmlStrcmp(mod_info->name, (const xmlChar*)"text") != 0) {
        auto key = xmlNodeListGetString(doc, mod_info->xmlChildrenNode, 1);
        std::stringstream ss;
        ss << key;
        auto str = ss.str();
        str.erase(std::remove(str.begin(),
                              str.end(),
                              ' '),
                  str.end());

        if (xmlStrcmp(mod_info->name, (const xmlChar*)"useIPCore") == 0) {
          if (str.compare("true") == 0)
            use_ip_core = true;
          else
            use_ip_core = false;
        }

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"latency") == 0)
          latency = std::stoul(str);

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"moduleName") == 0)
          mod_name = str;

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"numModule") == 0)
          num_module = std::stoul(str);

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"clockPortName") == 0)
          clock_port_name = str;

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"leftOperandPortName") == 0)
          left_operator_port_name = str;

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"rightOperandPortName") == 0)
          right_operator_port_name = str;

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"outPortName") == 0)
          out_port_name = str;

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"clockEnablePortName") == 0)
          clock_enable_port_name = str;

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"leftPortBitWidth") == 0)
          left_port_bit_width = std::stoul(str);

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"rightPortBitWidth") == 0)
          right_port_bit_width = std::stoul(str);

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"outPortBitWidth") == 0)
          out_port_bit_width = std::stoul(str);

        xmlFree(key);
      } // if : mod_info
      mod_info = mod_info->next;
    } // while : mod_info

    // module のインスタンス化
    for (auto i=0; i<num_module; ++i) {
      auto ope_instance_name = mod_name + '_' + std::to_string(i);
      auto ope = std::make_shared<CDFG_Operator>
        (ope_instance_name,
         mod_name,
         latency,
         type);

      // 入出力ポートの割り当て
      auto left_node = std::make_shared<CDFG_Node>
        (ope_instance_name + '_' + left_operator_port_name,
         left_port_bit_width,
         false, //! @todo is signed対応
         CDFG_Node::eNode::REG);

      auto right_node = std::make_shared<CDFG_Node>
        (ope_instance_name + '_' + right_operator_port_name,
         right_port_bit_width,
         false, //! @todo is signed対応
         CDFG_Node::eNode::REG);

      auto out_node = std::make_shared<CDFG_Node>
        (ope_instance_name + '_' + out_port_name,
         out_port_bit_width,
         false, //! @todo is signed対応
         CDFG_Node::eNode::WIRE);

      ope->add_input_port(left_operator_port_name,
                          left_node);
      ope->add_input_port(right_operator_port_name,
                          right_node);
      ope->add_output_port(out_port_name,
                           out_node);

      // clockの接続
      auto clk_node = module_gen->get_node(CDFG_Node::eNode::CLK);
      ope->add_output_port(clock_port_name,
                           clk_node);

      module_gen->add_node(left_node);
      module_gen->add_node(right_node);
      module_gen->add_node(out_node);

      module_gen->add_operator(ope);
      cur = cur->next;
    } // while : cur
  }
  //! @todo FUNC (独自関数)への対応
}
