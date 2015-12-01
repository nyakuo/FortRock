#include "CDFG_Optimizer.hpp"

/**
   最適化の実行
   @brief 演算のチェイニング，不要なレジスタの削除
   @return 最適化前後で演算器全体でのレイテンシの減少数
 */
unsigned
CDFG_Optimizer::do_optimize
(void)
{
  std::list<std::list
            <std::shared_ptr<CDFG_Element> >
            > dfg; // ステートのDFGのリスト
  std::list<std::shared_ptr<CDFG_Element> > tmp_dfg; // ステートのDFG

  std::list<std::shared_ptr<CDFG_Element> > phi_list; // PHI命令のリスト

  // DFGを各ステートで切り出す
  // PHI命令のリストを作成
  {
    auto state = 0;
    for (auto & elem
           : this->_module->get_element_list())
      {
        // ステートが移動したか
        if (state != elem->get_state())
          {
            dfg.emplace_back(tmp_dfg);
            tmp_dfg.clear();
            state = elem->get_state();
          } // if

        tmp_dfg.emplace_back(elem);

        // PHI命令をリストに追加
        if (elem->get_operator()->get_type()
            == CDFG_Operator::eType::Phi)
          phi_list.emplace_back(elem);
      } // for
    tmp_dfg.clear();
  }

  auto ret_change_ltc = 0;

  // 各ステートの最適化
  for (auto & state_dfg : dfg)
    {
      for (auto & elem : state_dfg)
        {
          auto original_step = elem->get_step();

          // for debug
          // Phi命令以外を回避
          if (elem->get_operator()->get_type()
              != CDFG_Operator::eType::Phi)
            continue;

          // チェイニングルールの判定
          for (auto i=0; i<elem->get_num_output(); ++i)
            {
              auto output = elem->get_output_at(i);

              if (this->_used_in_phi(output, phi_list))
                break;

              if (this->_used_in_another_state(elem->get_state(),
                                               output, dfg))
                break;

              if (this->_is_multiple_use(elem, dfg))
                break;

              // Regの置換
              switch(output->get_type()) {
              case CDFG_Node::eType::Reg:
                std::cout << this->_replace_reg(elem, state_dfg) << std::endl;
                break;

              default:;
              } // switch

              // 不要となったRegの削除
              // 不要な命令の削除
            } // for : i
        } // elem
    } // state_dfg

  return ret_change_ltc; // 演算器全体のレイテンシの変化
} // do_optimize

/**
   NodeがPhi命令の出力か判定
   @param[in] node 判定対象のNodeの参照
   @param[in] phi_list Phi命令のリスト
   @return 0: Phi命令の出力ではない
           0以外: 使用されるPhi命令のphi_list内でのインデックス
 */
std::shared_ptr<CDFG_Node>
CDFG_Optimizer::_is_phi_output
(const std::shared_ptr<CDFG_Node> & node,
 const std::list<std::shared_ptr<CDFG_Element> > & phi_list)
{
  auto idx = 0;

  for (auto & phi : phi_list) {
    for (auto i=0; i<phi->get_num_output(); ++i) {
      auto out = phi->get_output_at(i);

      if (out == node) {
        return out;
      }
    }
    ++idx;
  }
  return NULL;
} // _is_phi_output

/**
   Node(Param, Reg) がPhi命令のトリガとして
   参照されているかを判定
   @param[in] node Reg の参照
   @param[in] phi_list Phi命令のリスト
   @return Node(Reg) がPhi命令のトリガとして参照されているか
*/
bool
CDFG_Optimizer::_used_in_phi
(const std::shared_ptr<CDFG_Node> & node,
 const std::list<std::shared_ptr<CDFG_Element> > & phi_list)
{
  for (auto & phi : phi_list) {
    // Phi のトリガと判定
    for (auto i=1; i<phi->get_num_input(); i+=2) {
      auto val = phi->get_input_at(i);

      if (val == node)
        return true;
    }
  }
  return false;
} // _used_in_phi

/**
   Nodeが複数のステートで参照されていないか判定
   @param[in] state Nodeが参照されるステート
   @param[in] node 判定対象のNodeの参照
   @param[in] dfg モジュールのDFG
   @return Nodeが複数のステートで参照されていないか
*/
bool
CDFG_Optimizer::_used_in_another_state
(const unsigned & state,
 const std::shared_ptr<CDFG_Node> & node,
 const std::list<std::list<std::shared_ptr<CDFG_Element> > > & dfg)
{
  for (auto & state_dfg : dfg) {
    for (auto & elem : state_dfg) {
      // 同一ステートはスキップ
      if (elem->get_state() == state)
        break;

      // 入力と判定
      for (auto i=0; i<elem->get_num_input(); ++i)
        if (elem->get_input_at(i) == node)
          return true;

      // 出力と判定
      for (auto i=0; i<elem->get_num_output(); ++i)
        if (elem->get_output_at(i) == node)
          return true;
    }
  }

  return false;
} // used_in_another_state

/**
   演算器が同一ステートで複数回使用されるか判定
   @param[in] state 演算器が使用されるステート
   @param[in] ope 演算器の参照
   @param[in] dfg モジュールのDFG
   @return 演算器が同一ステートで複数回使用されるか
*/
bool
CDFG_Optimizer::_is_multiple_use
(const std::shared_ptr<CDFG_Element> & elem,
 const std::list<std::list<std::shared_ptr<CDFG_Element> > > & dfg)
{
 auto state = elem->get_state();
  auto ope = elem->get_operator();

  if (elem->get_operator()->get_type()
      == CDFG_Operator::eType::Phi)
    return false;

  for (auto & state_dfg : dfg) {
    for (auto & elem : state_dfg) {
      // 同一ステート以外はスキップ
      if (elem->get_state() != state)
        break;

      if (elem->get_operator() == ope)
        return true;
    }
  }

  return false;
} // _is_multiple_use

/**
   同一ステート内の対象Regの置換
   @param[in] node 置換対象のReg
   @param[in] state_dfg ステートのDFG
   @return 置換した数
 */
unsigned
CDFG_Optimizer::_replace_reg
(const std::shared_ptr<CDFG_Element> & rm_elem,
 std::list<std::shared_ptr<CDFG_Element> > & state_dfg)
{
  auto target
    = std::dynamic_pointer_cast<CDFG_Reg>
    (rm_elem->get_output_at(0));

  auto cnt = 0;

  // 置換対象の検索
  for (auto & elem : state_dfg) {
    for (auto i=0; i<elem->get_num_input(); ++i) {
      auto in = elem->get_input_at(i);

      if (in->get_type() != CDFG_Node::eType::Reg
          || in != target)
        continue;

      // Regの置換
      std::cout << "replace" << std::endl;
      rm_elem->set_type(CDFG_Element::eType::Input);
      elem->set_input(rm_elem, i);
      ++cnt;
    } // for : i
  } // for : elem

  return cnt;
} // _replace_reg
