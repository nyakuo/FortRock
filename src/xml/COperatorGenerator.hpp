#ifndef _COPERATOR_GENERATOR_H
#define _COPERATOR_GENERATOR_H

#include <string>
#include <memory>
#include <list>
#include "../dfg/CDFG_Operator.hpp"
#include "../dfg/CDFG_Node.hpp"
#include "../dfg/CModuleGenerator.hpp"

/**
   @class COperatorGenerator
   @brief 演算器をxmlからインスタンス化するためのクラス
 */
class COperatorGenerator {

  /**
      @class CNode_data
      @brief 入出力ポートのデータを保持する
  */
  class CNode_data {
  public:
    CNode_data(const std::string & port_name,
               const unsigned & bit_width,
               const CDFG_Node::eNode & type,
               const bool & is_signed)
      : _port_name(port_name),
        _bit_width(bit_width),
        _type(type),
        _is_signed(is_signed) {}

    std::string & get_name(void) { return this->_port_name; }
    unsigned & get_bit_width(void) { return this->_bit_width; }
    CDFG_Node::eNode & get_type(void) { return this->_type; }
    bool & get_is_signed(void) { return this->_is_signed; }

  private:
    std::string _port_name; //! 入出力ポート名
    unsigned _bit_width;    //! 入出力のビット幅
    CDFG_Node::eNode _type; //! 入出力の種類 (REG, WIRE)
    bool _is_signed;        //! 入出力がsignedかどうか
  };

public:
  COperatorGenerator(void);
  ~COperatorGenerator(void);

  void generate_operator(std::shared_ptr<CModuleGenerator> & module_gen);
  void add_port_info(const std::string & name,
                     const unsigned & bit_width,
                     const CDFG_Node::eNode & type,
                     const bool & is_signed);
  void set_type(const CDFG_Operator::eType & type);
  void set_latency(const unsigned & latency);
  void set_name(const std::string & name);
  void set_instance_name(const std::string & name);
  void set_num_operator(const unsigned & num);

private:
  std::list<std::shared_ptr<CNode_data> > _inode_info; //! 入力ノードのリスト
  std::list<std::shared_ptr<CNode_data> > _onode_info; //! 出力ノードのリスト
  CDFG_Operator::eType _type;  //! 演算器の種類
  unsigned _latency;           //! 演算器のレイテンシ
  std::string _name;           //! 演算器のmodule名
  std::string _instance_name;  //! 演算器のインスタンス名
  unsigned _num_operator;      //! インスタンス化する演算器の数
};

#endif
