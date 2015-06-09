#ifndef _CDFG_OPERATOR_H
#define _CDFG_OPERATOR_H

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "CDFG_Node.hpp"

/**
   @class CDFG_Operator
   @brief 演算器を定義するクラス
*/
class CDFG_Operator {
public:
  enum class eType : unsigned {
    ADD = 0,
      SUB,
      ADD_SUB,
      MUL,
      DIV,
      FUNC
      };

  CDFG_Operator(const std::string & name,
                const std::string & mod_name,
                const unsigned & latency,
                const eType & type);
  ~CDFG_Operator(void);

  /** ゲッター */
  std::string get_name(void) const;
  std::string get_mod_name(void) const;
  unsigned get_latency(void) const;
  eType get_type(void) const;
  unsigned get_num_input(void) const;
  unsigned get_num_output(void) const;
  std::string get_input_signal_at(const unsigned & at) const;
  std::string get_output_signal_at(const unsigned & at) const;
  std::shared_ptr<CDFG_Node> get_input_node_at(const unsigned & at) const;
  std::shared_ptr<CDFG_Node> get_output_node_at(const unsigned & at) const;

  /* セッター */
  void add_input_port(const std::string & port_name,
                      std::shared_ptr<CDFG_Node> & node);
  void add_output_port(const std::string & port_name,
                       std::shared_ptr<CDFG_Node> & node);
  void set_latency(const unsigned & latency);
  void set_type(const eType & type);

private:
  std::string _name;     /** インスタンス名 */
  std::string _mod_name; /** モジュールの名前 */
  unsigned _latency;     /** レイテンシ */
  eType _type;           /** モジュールの種類 */

  std::vector<std::pair<std::string, std::shared_ptr<CDFG_Node> > > _input_list; /** 入力ポート定義リスト */
  std::vector<std::pair<std::string, std::shared_ptr<CDFG_Node> > > _output_list; /** 出力ポート定義リスト */
};

#endif
