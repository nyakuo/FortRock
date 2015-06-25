#include "CDFG_Element.hpp"

/**
   デフォルトコンストラクタ
   @param[in] type 演算の種類
   @param[in] state 演算の実行ステート
   @param[in] step 演算の実行ステップ
   @note load命令やphi命令など演算器を必要としない
         命令のDFG化にに使用
 */
CDFG_Element::CDFG_Element
(const CDFG_Operator::eType & type,
 const unsigned & num_input,
 const unsigned & state,
 const unsigned & step) {
  auto phony_ope = std::make_shared<CDFG_Operator>
    ("phony",
     "phony_mod",
     0, /* latency */
     type);
  this->_input_list.resize(num_input);
  this->_output_list.resize(1); //! @todo 演算器の出力を1つに限定
  this->_ope = phony_ope;
  this->_state = state;
  this->_step = step;
}

/**
   コンストラクタ
   @param[in] ope 演算に使用する演算器の参照
 */
CDFG_Element::CDFG_Element(std::shared_ptr<CDFG_Operator> & ope) {
  this->set_operator(ope);
}

/**
   オペレータのセット
   @param[in] ope セットするオペレータ
   @return 1 オペレータを上書き, 0 オペレータを新規にセット
 */
int CDFG_Element::set_operator(std::shared_ptr<CDFG_Operator> & ope) {
  int ret = 0;

  // overwrite
  if (this->_ope) {
    this->_ope.reset();
    ret = 1;
  }
  this->_ope = ope;

  // 入出力数の設定
  this->_input_list.resize(ope->get_num_input());
  this->_output_list.resize(ope->get_num_output());

  return ret;
}

/**
   入力のセット
   @param[in] input セットする入力ノード
   @param[in] number セットする位置
   @return 0 セットする位置が有効, -1 セットする位置が無効
 */
int CDFG_Element::set_input(std::shared_ptr<CDFG_Node> & input,
                            const unsigned & number) {
  if (number > this->_input_list.size())
    return -1;

  this->_input_list[number] = input;
  return 0;
}

/**
   出力のセット
   @param[in] output セットする出力ノード
   @param[in] number セットする位置
   @return 0 セットする位置が有効, -1 セットする位置が無効
*/
int CDFG_Element::set_output(std::shared_ptr<CDFG_Node> & output,
                             const unsigned & number) {
  if (number > this->_output_list.size())
    return -1;

  this->_output_list[number] = output;
  return 0;
}

/**
   実行ステート(CFG)のセット
   @param[in] state 処理の実行開始ステート (CFG)
 */
void CDFG_Element::set_state(const unsigned & state) {
  this->_state = state;
}

/**
   実行ステップ(clockベースのタイミング)のセット
   @param[in] step 処理の実行タイミング
*/
void CDFG_Element::set_step(const unsigned & step) {
  this->_step = step;
}

/**
   指定した位置の入力ノードの取得
   @param[in] at 入力ノードの位置
   @return 入力ノード
 */
std::shared_ptr<CDFG_Node> &
CDFG_Element::get_input_at(const unsigned & at) {
  return this->_input_list.at(at);
}

/**
   指定した位置の出力ノードの取得
   @param[in] at 出力ノードの位置
   @return 出力ノード
 */
std::shared_ptr<CDFG_Node> &
CDFG_Element::get_output_at(const unsigned & at) {
  return this->_output_list.at(at);
}

/**
   演算に用いる演算器の取得
   @return 演算に用いる演算器
 */
std::shared_ptr<CDFG_Operator> &
CDFG_Element::get_operator(void) {
  return this->_ope;
}

/**
   演算ステート(CFGのベースのタイミング)の取得
   @return 演算ステート
 */
unsigned CDFG_Element::get_state(void) {
  return this->_state;
}

/**
   演算ステップ(DFGベースのタイミング)の取得
   @return 演算ステップ
 */
unsigned CDFG_Element::get_step(void) {
  return this->_step;
}
