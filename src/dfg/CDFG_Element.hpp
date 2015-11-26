#ifndef _CDFG_ELEMENT_H
#define _CDFG_ELEMENT_H

#include <memory>
#include <vector>
#include "CDFG_Node.hpp"
#include "CDFG_Wire.hpp"
#include "CDFG_Operator.hpp"

/**
   @class CDFG_Element
   @brief 入出力の接続を表すクラス

   モジュールや演算器に対して入出力がどのタイミングで
   与えられるかを表す
*/
class CDFG_Element : public CDFG_Node {
public:
  /** Elementの種類 */
  enum class eType : unsigned {
    Normal = 0, ///< 通常のDFG中の命令
      Input,    ///< 他の命令の入力
      Output,   ///< 他の命令の出力
  };

  CDFG_Element(const CDFG_Operator::eType & type,
               const unsigned & num_input,
               const unsigned & state,
               const unsigned & step,
               const unsigned & latency = 0);

  CDFG_Element(const std::shared_ptr<CDFG_Operator> & ope,
               const unsigned & state,
               const unsigned & step);

  virtual ~CDFG_Element(void) {}

  // Setter
  int set_operator(const std::shared_ptr<CDFG_Operator> & ope);
  int set_input(const std::shared_ptr<CDFG_Node> & input,
                const unsigned & number);
  void set_num_input(const unsigned & num_input);
  int set_output(const std::shared_ptr<CDFG_Node> & input,
                 const unsigned & number);
  void set_state(const unsigned & state);
  void set_step(const unsigned & step);
  void set_type(const eType & type);

  // Getter
  const std::shared_ptr<CDFG_Node> & get_input_at(const unsigned & at) const;
  const std::shared_ptr<CDFG_Node> & get_output_at(const unsigned & at) const;
  const unsigned get_num_input(void) const;
  const unsigned get_num_output(void) const;
  const std::shared_ptr<CDFG_Operator> & get_operator(void) const;
  const unsigned & get_state(void) const;
  const unsigned & get_step(void) const;

  // Override
  virtual bool is_input(void) override final
  { return (this->_type == eType::Input); }
  virtual bool is_output(void) override final
  { return (this->_type == eType::Output); }

  // Virtual
  virtual std::string input_from_str(const unsigned & at=0);
  virtual std::string input_to_str(const unsigned & at=0);
  virtual std::string output_from_str(void);
  virtual std::string output_to_str(void);
  virtual std::string other_str(const std::string & indent) { return "";}

protected:
  std::shared_ptr<CDFG_Operator> _ope; ///< 実行する演算器の参照

  std::string _get_input_str(const std::shared_ptr<CDFG_Node> & node);
  std::string _get_input_str(const unsigned & at);

private:
  std::vector<std::shared_ptr<CDFG_Node> > _input_list;  ///< 入力リスト
  std::vector<std::shared_ptr<CDFG_Node> > _output_list; ///< 出力リスト

  unsigned _state;  ///< 実行ステート (CFG)
  unsigned _step;   ///< 実行ステップ (clock)
  eType _type;      ///< Elementの種類 (通常，入力，出力)
};

#endif
