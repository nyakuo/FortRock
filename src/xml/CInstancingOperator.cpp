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
      throw std::string ("ERROR: can not open config file");
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

  // IPコアの項目までパース
  cur = cur->xmlChildrenNode;
  while (xmlStrcmp(cur->name, (const xmlChar*)"ip_core"))
    cur = cur->next;

  // 演算器の情報をパース
  this->_parse_operator_info(doc, cur, module_gen);

  // ramの項目までパース
  while (xmlStrcmp(cur->name, (const xmlChar*)"ram"))
    cur = cur->next;

  this->_parse_ram_info(doc, cur);
}

/**
   演算器の情報をパースし，演算器をインスタンス化する
   @param[in] doc ドキュメントツリーの参照
   @param[in] node 現在のノードの参照
   @param[in] module_gen モジュール生成クラスの参照
*/
void
CInstancingOperator::_parse_operator_info
(const xmlDocPtr & doc,
 const xmlNodePtr & node,
 std::shared_ptr<CModuleGenerator> & module_gen)
{
  auto cur = node->xmlChildrenNode;
  unsigned num_module;
  CDFG_Operator::eType type;

  // 演算器のパース
  while (cur != NULL) {
    auto ope_gen = std::make_shared<COperatorGenerator>();

    //! @todo その他の種類への対応
    if (xmlStrcmp(cur->name, (const xmlChar*)"add_int") == 0)
      ope_gen->set_type(CDFG_Operator::eType::Add);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"fadd") == 0)
      ope_gen->set_type(CDFG_Operator::eType::Fadd);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"sub_int") == 0)
      ope_gen->set_type(CDFG_Operator::eType::Sub);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"fsub") == 0)
      ope_gen->set_type(CDFG_Operator::eType::Fsub);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"mul") == 0)
      ope_gen->set_type(CDFG_Operator::eType::Mul);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"fmul") == 0)
      ope_gen->set_type(CDFG_Operator::eType::Fmul);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"s_rem") == 0)
      ope_gen->set_type(CDFG_Operator::eType::Srem);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"s_div") == 0)
      ope_gen->set_type(CDFG_Operator::eType::Sdiv);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"fdiv") == 0)
      ope_gen->set_type(CDFG_Operator::eType::Fdiv);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"fcmp") == 0)
      ope_gen->set_type(CDFG_Operator::eType::Fcmp);

    else {
      cur = cur->next;
      continue;
    }

    // 演算器の情報のパース
    auto mod_info = cur->xmlChildrenNode;
    while (mod_info != NULL) {
      if (xmlStrcmp(mod_info->name, (const xmlChar*)"text") != 0) {
        auto key
          = xmlNodeListGetString(doc,
                                 mod_info->xmlChildrenNode,
                                 1);
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
            ope_gen->set_num_operator(0);
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
          CNode_data::ePortType type;

          for (auto & info = mod_info->xmlChildrenNode;
               info != NULL;
               info = info->next) {
            if (xmlStrcmp(info->name,
                          (const xmlChar*)"info") == 0) {
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
                    type = CNode_data::ePortType::In;
                  else if (str.compare("output") == 0)
                    type = CNode_data::ePortType::Out;
                  else if (str.compare("clock") == 0)
                    type = CNode_data::ePortType::Clk;
                  else if (str.compare("clock_enable") == 0)
                    type = CNode_data::ePortType::Ce;
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

/**
   RAMの情報をパースし，RAMの入出力ポート数(共通)を設定する
   @param[in] doc ドキュメントツリーの参照
   @param[in] node 現在のノードの参照
*/
void
CInstancingOperator::_parse_ram_info
(const xmlDocPtr & doc,
 const xmlNodePtr & node)
{
  auto cur = node->xmlChildrenNode;

  std::string name("");
  auto num_r_port = 0;
  auto num_w_port = 0;

  while(cur != NULL) {
    auto key
      = xmlNodeListGetString(doc,
                             cur->xmlChildrenNode,
                             1);
    std::stringstream ss;
    ss << key;
    auto str = ss.str();
    str.erase(std::remove(str.begin(),
                          str.end(),
                          ' '),
              str.end());

    if (xmlStrcmp(cur->name, (const xmlChar*)"module_name") == 0)
      name = str;

    else if (xmlStrcmp(cur->name, (const xmlChar*)"num_read_port") == 0)
      num_r_port = std::stoul(str);

    else if (xmlStrcmp(cur->name, (const xmlChar*)"num_write_port") == 0)
      num_w_port = std::stoul(str);

    cur = cur->next;
  }

  // RAMに使用する入出力ポート数の設定 (共通)
  CDFG_Ram::set_num_port(num_r_port,
                         num_w_port);
}
