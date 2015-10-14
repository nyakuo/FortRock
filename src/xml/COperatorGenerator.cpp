#include "COperatorGenerator.hpp"

COperatorGenerator::COperatorGenerator(void){}
COperatorGenerator::~COperatorGenerator(void){}

/**
   演算器のインスタンス化 (moduleへ登録)
   @param[in] module_gen モジュールへの参照
 */
void COperatorGenerator::generate_operator
(std::shared_ptr<CModuleGenerator> & module_gen ) {
  for (auto i=0; i<this->_num_operator; ++i) {
    this->set_instance_name(this->_name + std::to_string(i));
    auto ope = std::make_shared<CDFG_Operator>
      (this->_instance_name,
       this->_name,
       this->_latency,
       this->_type);

    // 入力ポートのインスタンス化
    for (auto & inode : this->_inode_info) {
      std::shared_ptr<CDFG_Node> node;
      // CKL, CE
      if (inode->get_type() == CNode_data::ePortType::CLK ||
          inode->get_type() == CNode_data::ePortType::CE) {
        node = module_gen->get_node((inode->get_type()
                                     == CNode_data::ePortType::CLK) ?
                                    CDFG_Wire::eWireType::CLK
                                    : CDFG_Wire::eWireType::CE);
        ope->add_input_port(inode->get_name(),
                            node);
      }
      // 引数ポート
      else if (inode->get_type() == CNode_data::ePortType::IN) {
        node = std::make_shared<CDFG_Reg>
          (this->_instance_name + '_' + inode->get_name(),
           inode->get_bit_width(),
           inode->get_is_signed(),
           CDFG_Reg::eRegType::REG);

        ope->add_input_port(inode->get_name(),
                            node);
        module_gen->add_node(node);
      }
    }

    // 出力ポートのインスタンス化
    for (auto & onode : this->_onode_info) {
      auto node = std::make_shared<CDFG_Wire>
        (this->_instance_name + '_' + onode->get_name(),
         onode->get_bit_width(),
         onode->get_is_signed(),
         CDFG_Wire::eWireType::WIRE);
      ope->add_output_port(onode->get_name(),
                           node);
      module_gen->add_node(node);
    }

    // 演算器のインスタンス化
    module_gen->add_operator(ope);
  }
}

/**
   入出力ポートの情報を追加
   @param[in] name ポート名
   @param[in] bit_width ポートのビット幅
   @param[in] type 信号の種類
   @param[in] is_signed ポートがsignedか
 */
void COperatorGenerator::add_port_info
(const std::string & name,
 const unsigned & bit_width,
 const CNode_data::ePortType & type,
 const bool & is_signed)
{
  auto node_data = std::make_shared<CNode_data>
    (name, bit_width, type, is_signed);

  if (type == CNode_data::ePortType::IN
      || type == CNode_data::ePortType::CLK
      || type == CNode_data::ePortType::CE)
    this->_inode_info.push_back(node_data);
  else if (type == CNode_data::ePortType::OUT)
    this->_onode_info.push_back(node_data);
}

/**
   演算器の種類を設定
   @param[in] type 演算器の種類
 */
void COperatorGenerator::set_type
(const CDFG_Operator::eType & type) {
  this->_type = type;
}

/**
   演算器のレイテンシを設定
   @param[in] latency 演算器のレイテンシ
 */
void COperatorGenerator::set_latency
(const unsigned & latency) {
  this->_latency = latency;
}

/**
   演算器のモジュール名を設定
   @param[in] name 演算器のモジュール名
 */
void COperatorGenerator::set_name
(const std::string & name) {
  this->_name = name;
}

/**
   演算器のインスタンス名を設定
   @param[in] name 演算器のインスタンス名
 */
void COperatorGenerator::set_instance_name
(const std::string & name) {
  this->_instance_name = name;
}

/**
   インスタンス化する演算器の数を設定
   @param[in] num インスタンス化する演算器の数
 */
void COperatorGenerator::set_num_operator
(const unsigned & num) {
  this->_num_operator = num;
}
