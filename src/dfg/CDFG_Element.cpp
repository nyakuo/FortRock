#include "CDFG_Element.hpp"

/**
   デフォルトコンストラクタ
   @param[in] type 演算の種類
   @param[in] num_input 演算の入力の数
   @param[in] state 演算の実行ステート
   @param[in] step 演算の実行ステップ
   @param[in] latency 演算にかかるレイテンシ (default: 0)
   @note load命令やphi命令など演算器を必要としない
   命令のDFG化にに使用
*/
CDFG_Element::CDFG_Element
(const CDFG_Operator::eType & type,
 const unsigned & num_input,
 const unsigned & state,
 const unsigned & step,
 const unsigned & latency)
  : _state(state),
    _step(step),
    CDFG_Node(CDFG_Node::eType::Elem)
{
  auto phony_ope
    = std::make_shared<CDFG_Operator>
    ("phony",
     "phony_mod",
     latency,
     type);

  this->_input_list.resize(num_input);
  this->_output_list.resize(1);

  // getElementPtr inst の場合は出力は無し
  if (type == CDFG_Operator::eType::Getelementptr)
    this->_output_list.resize(0);

  this->_ope = phony_ope;
  this->_ope->set_num_input(num_input);
} // CDFG_Element

/**
   コンストラクタ
   @param[in] ope 演算に使用する演算器の参照
*/
CDFG_Element::CDFG_Element
(const std::shared_ptr<CDFG_Operator> & ope,
 const unsigned & state,
 const unsigned & step)
  : _state(state),
    _step(step),
    CDFG_Node(CDFG_Node::eType::Elem)
{
  this->set_operator(ope);
} // CDFG_Element

/**
   オペレータのセット
   @param[in] ope セットするオペレータ
   @return 1 オペレータを上書き, 0 オペレータを新規にセット
*/
int CDFG_Element::set_operator
(const std::shared_ptr<CDFG_Operator> & ope)
{
  int ret = 0;

  // overwrite
  if (this->_ope)
    {
      this->_ope.reset();
      ret = 1;
    }

  this->_ope = ope;

  // 入出力数の設定
  auto cnt = 0;
  for (auto i=0; i<ope->get_num_input(); ++i)
    {
      auto input = ope->get_input_node_at(i);

      if (input->get_type() == CDFG_Node::eType::Wire)
        {
          auto reg = std::dynamic_pointer_cast<CDFG_Wire>(input);

          // clk, ceの場合は Elementの入力に登録しない
          if (reg->get_type() == CDFG_Wire::eWireType::Clk
              || reg->get_type() == CDFG_Wire::eWireType::Ce)
            continue;
        }
      ++cnt;
    } // for : i

  this->_input_list.resize(cnt);
  this->_output_list.resize(ope->get_num_output());

  return ret;
} // set_operator

/**
   入力のセット
   @param[in] input セットする入力ノード
   @param[in] number セットする位置
   @return 0 セットする位置が有効, -1 セットする位置が無効
*/
int CDFG_Element::set_input
(const std::shared_ptr<CDFG_Node> & input,
 const unsigned & number)
{
  if (number > this->_input_list.size())
    return -1;

  this->_input_list[number] = input;
  return 0;
} // set_input

/**
   出力のセット
   @param[in] output セットする出力ノード
   @param[in] number セットする位置
   @return 0 セットする位置が有効, -1 セットする位置が無効
*/
int CDFG_Element::set_output
(const std::shared_ptr<CDFG_Node> & output,
 const unsigned & number)
{
  if (number > this->_output_list.size())
    return -1;

  this->_output_list[number] = output;
  return 0;
} // set_output

/**
   実行ステート(CFG)のセット
   @param[in] state 処理の実行開始ステート (CFG)
*/
void CDFG_Element::set_state
(const unsigned & state)
{
  this->_state = state;
}

/**
   実行ステップ(clockベースのタイミング)のセット
   @param[in] step 処理の実行タイミング
*/
void CDFG_Element::set_step
(const unsigned & step)
{
  this->_step = step;
}

/**
   Elementの種類を設定
   @param[in] type Elementの種類
 */
void
CDFG_Element::set_type
(const eType & type)
{
  this->_type = type;
}

/**
   指定した位置の入力ノードの取得
   @param[in] at 入力ノードの位置
   @return 入力ノード
*/
std::shared_ptr<CDFG_Node> &
CDFG_Element::get_input_at
(const unsigned & at)
{
  return this->_input_list.at(at);
}

/**
   指定した位置の出力ノードの取得
   @param[in] at 出力ノードの位置
   @return 出力ノード
*/
std::shared_ptr<CDFG_Node> &
CDFG_Element::get_output_at
(const unsigned & at)
{
  return this->_output_list.at(at);
}

/**
   接続されている演算器の入力の数を取得
   @return 接続されている演算器の入力の数
   @note 演算器に接続されているCLK，CEは含まない
*/
unsigned
CDFG_Element::get_num_input
(void)
{
  return this->_input_list.size();
}

/**
   接続されている演算器の出力の数を取得
   @return 接続されている演算器の出力の数
*/
unsigned
CDFG_Element::get_num_output
(void)
{
  return this->_output_list.size();
}

/**
   演算に用いる演算器の取得
   @return 演算に用いる演算器
*/
std::shared_ptr<CDFG_Operator> &
CDFG_Element::get_operator
(void)
{
  return this->_ope;
}

/**
   演算ステート(CFGのベースのタイミング)の取得
   @return 演算ステート
*/
unsigned CDFG_Element::get_state
(void)
{
  return this->_state;
}

/**
   演算ステップ(DFGベースのタイミング)の取得
   @return 演算ステップ
*/
unsigned CDFG_Element::get_step
(void)
{
  return this->_step;
}
