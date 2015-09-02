#ifndef _CDFG_ELEMENT_H
#define _CDFG_ELEMENT_H

#include <iostream>
#include <memory>
#include <vector>
#include "CDFG_Node.hpp"
#include "CDFG_Operator.hpp"

/**
   @class CDFG_Element
   @brief 入出力の接続を表すクラス

   モジュールや演算器に対して入出力がどのタイミングで
   与えられるかを表す
*/
class CDFG_Element {
public:
  CDFG_Element(const CDFG_Operator::eType & type,
               const unsigned & num_input,
               const unsigned & state,
               const unsigned & step);

  CDFG_Element(const std::shared_ptr<CDFG_Operator> & ope,
               const unsigned & state,
               const unsigned & step);

  virtual ~CDFG_Element(void) {}

  // Setter
  int set_operator(const std::shared_ptr<CDFG_Operator> & ope);
  int set_input(std::shared_ptr<CDFG_Node> input,
                const unsigned & number);
  int set_output(std::shared_ptr<CDFG_Node> input,
                 const unsigned & number);
  void set_state(const unsigned & state);
  void set_step(const unsigned & step);

  // Getter
  std::shared_ptr<CDFG_Node> & get_input_at(const unsigned & at);
  std::shared_ptr<CDFG_Node> & get_output_at(const unsigned & at);
  unsigned get_num_input(void);
  unsigned get_num_output(void);
  std::shared_ptr<CDFG_Operator> & get_operator(void);
  unsigned get_state(void);
  unsigned get_step(void);

private:
  std::vector<std::shared_ptr<CDFG_Node> > _input_list;  ///< 入力リスト
  std::vector<std::shared_ptr<CDFG_Node> > _output_list; ///< 出力リスト
  std::shared_ptr<CDFG_Operator> _ope;                   ///< 実行する演算器の参照
  unsigned _state;                                       ///< 実行ステート (CFG)
  unsigned _step;                                        ///< 実行ステップ (clock)
};

#endif
