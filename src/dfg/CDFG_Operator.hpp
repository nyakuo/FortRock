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
    Add = 0,
      Fadd,
      Sub,
      Fsub,
      Add_sub,
      Div,
      Func,
      Mul,
      Fmul,
      Sdiv,
      Fdiv,
      Srem,   // ↑演算器使用のレイテンシあり
      Ret,    // ↓演算器不使用のレイテンシなし (代入など)
      Br,
      Load,
      Store,
      Icmp,
      Fcmp,
      Phi,
      Select,
      Switch,
      Lshiftl,
      Lshiftr,
      Ashiftr,
      And,
      Or,
      Xor,
      Copy, ///< 引数のコピー (非LLVMの命令)
      Trunc,
      Getelementptr,
      Zext,
      };

  ///< @todo 演算器の入力ポートの名前などを外部の入力ファイルを用いて指定する機能

  CDFG_Operator(const std::string & name,
                const std::string & mod_name,
                const unsigned    & latency,
                const eType       & type);
  ~CDFG_Operator(void);

  // getter
  std::string get_name(void)       const;
  std::string get_mod_name(void)   const;
  unsigned    get_latency(void)    const;
  eType       get_type(void)       const;
  unsigned    get_num_input(void)  const;
  unsigned    get_num_output(void) const;
  std::string get_input_signal_at(const unsigned & at) const;
  std::string get_output_signal_at(const unsigned & at) const;
  std::shared_ptr<CDFG_Node> get_input_node_at(const unsigned & at) const;
  std::shared_ptr<CDFG_Node> get_output_node_at(const unsigned & at) const;

  // setter
  void add_input_port(const std::string & port_name,
                      const std::shared_ptr<CDFG_Node> & node);
  void add_output_port(const std::string & port_name,
                       const std::shared_ptr<CDFG_Node> & node);
  void set_latency(const unsigned & latency);
  void set_type(const eType & type);
  void set_num_input(const unsigned & num);

private:
  std::string _name;     ///< インスタンス名
  std::string _mod_name; ///< モジュールの名前
  unsigned    _latency;  ///< レイテンシ
  eType       _type;     ///< モジュールの種類

  std::vector<std::pair<std::string,
                        std::shared_ptr<CDFG_Node>
                        >
              > _input_list;  ///< 入力ポート定義リスト

  std::vector<std::pair<std::string,
                        std::shared_ptr<CDFG_Node>
                        >
              > _output_list; ///< 出力ポート定義リスト
};

#endif
