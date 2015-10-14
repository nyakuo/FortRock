#ifndef _COPERATOR_GENERATOR_H
#define _COPERATOR_GENERATOR_H

#include <string>
#include <memory>
#include <list>
#include "CNode_data.hpp"
#include "../dfg/CDFG_Operator.hpp"
#include "../dfg/CDFG_Node.hpp"
#include "../dfg/CDFG_Reg.hpp"
#include "../dfg/CDFG_Wire.hpp"
#include "../dfg/CModuleGenerator.hpp"

/**
   @class COperatorGenerator
   @brief 演算器をxmlからインスタンス化するためのクラス
 */
class COperatorGenerator {
public:
  COperatorGenerator(void);
  ~COperatorGenerator(void);

  void generate_operator(std::shared_ptr<CModuleGenerator> & module_gen);
  void add_port_info(const std::string & name,
                     const unsigned & bit_width,
                     const CNode_data::ePortType & type,
                     const bool & is_signed);

  // setter
  void set_type(const CDFG_Operator::eType & type);
  void set_latency(const unsigned & latency);
  void set_name(const std::string & name);
  void set_instance_name(const std::string & name);
  void set_num_operator(const unsigned & num);

private:
  std::list<std::shared_ptr<CNode_data> > _inode_info; //! 入力ノードのリスト
  std::list<std::shared_ptr<CNode_data> > _onode_info; //! 出力ノードのリスト
  CDFG_Operator::eType  _type;          //! 演算器の種類
  unsigned              _latency;       //! 演算器のレイテンシ
  std::string           _name;          //! 演算器のmodule名
  std::string           _instance_name; //! 演算器のインスタンス名
  unsigned              _num_operator;  //! インスタンス化する演算器の数
};

#endif
