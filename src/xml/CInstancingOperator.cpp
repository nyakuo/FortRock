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
  while (xmlStrcmp(cur->name, (const xmlChar*)"ip_core"))
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
  unsigned num_module;
  CDFG_Operator::eType type;

  // 演算器のパース
  while (cur != NULL) {
    auto ope_gen = std::make_shared<COperatorGenerator>();

    //! @todo その他の種類への対応
    if (xmlStrcmp(cur->name, (const xmlChar*)"add_int") == 0)
      ope_gen->set_type(CDFG_Operator::eType::ADD);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"mul") == 0)
      ope_gen->set_type(CDFG_Operator::eType::MUL);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"s_rem") == 0)
      ope_gen->set_type(CDFG_Operator::eType::SREM);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"s_div") == 0)
      ope_gen->set_type(CDFG_Operator::eType::SDIV);

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

        if (xmlStrcmp(mod_info->name, (const xmlChar*)"use_ip_core") == 0) {
          if (str.compare("false") == 0) { // false
            num_module = 0; // 以降の処理をスキップ
            break;
          }
        }

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"latency") == 0)
          ope_gen->set_latency(std::stoul(str));

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"module_name") == 0)
          ope_gen->set_name(str);

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"instance_name") == 0)
          ope_gen->set_instance_name(str);

        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"num_module") == 0)
          ope_gen->set_num_operator(std::stoul(str));

        // 入出力定義
        else if (xmlStrcmp(mod_info->name, (const xmlChar*)"io") == 0) {
          CDFG_Node::eNode type;

          if (xmlStrcmp(mod_info->name, (const xmlChar*)"input") == 0)
            type = CDFG_Node::eNode::REG;
          else if (xmlStrcmp(mod_info->name, (const xmlChar*)"output") == 0)
            type = CDFG_Node::eNode::WIRE;

          for (auto & info = mod_info->xmlChildrenNode;
               info != NULL;
               info = info->next) {
            if (xmlStrcmp(info->name, (const xmlChar*)"info") == 0) {
              std::string port_name;
              unsigned bit_width;
              bool is_signed;

              for (auto text = info->xmlChildrenNode;
                   text != NULL;
                   text = text->next) {
                auto key = xmlNodeListGetString(doc, text->xmlChildrenNode, 1);
                std::stringstream ss;
                ss << key;
                auto str = ss.str();
                str.erase(std::remove(str.begin(),
                                      str.end(),
                                      ' '),
                          str.end());

                if (xmlStrcmp(text->name, (const xmlChar*)"port_name") == 0)
                  port_name = str;

                else if (xmlStrcmp(text->name, (const xmlChar*)"bit_width") == 0)
                  bit_width = std::stoul(str);

                else if (xmlStrcmp(text->name, (const xmlChar*)"is_signed") == 0) {
                  if (str.compare("true") == 0)
                    is_signed = true;
                  else if (str.compare("false") == 0)
                    is_signed = false;
                }

                else if (xmlStrcmp(text->name, (const xmlChar*)"type") == 0) {
                  if (str.compare("input") == 0)
                    type = CDFG_Node::eNode::REG;
                  else if (str.compare("output") == 0)
                    type = CDFG_Node::eNode::WIRE;
                  else if (str.compare("clock") == 0)
                    type = CDFG_Node::eNode::CLK;
                  else if (str.compare("clock_enable") == 0)
                    type = CDFG_Node::eNode::CE;
                }
              } // for : text
              ope_gen->add_port_info(port_name,
                                     bit_width,
                                     type,
                                     is_signed);
            } // if : info
          } //for : info
        } // if : mod_info

      } // if : text
      // 次の演算器の情報へ
      mod_info = mod_info->next;
    } // while : mod_info
    // 演算器のインスタンス化
    ope_gen->generate_operator(module_gen);

    // 次の演算器へ
    cur = cur->next;
  } // while : cur
  //! @todo FUNC (独自関数)への対応
}
