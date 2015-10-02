#include "CDFG_Scheduler.hpp"

/**
   スケジューリングの実行
   @brief データの依存性の確認
   演算器の使用状況の確認
   @return スケジューリング前後での演算器全体での
   レイテンシの変化
*/
#include <iostream>

unsigned
CDFG_Scheduler::do_schedule
(void) {
  std::list<std::list
            <std::shared_ptr<CDFG_Element> >
            > dfg;
  std::list<std::shared_ptr<CDFG_Element> > tmp_dfg;

  // DFGを各ステートで切り出す
  {
    auto state = 0;
    for (auto & elem
           : this->_module->get_element_list()) {
      // ステートが移動したか
      if (state != elem->get_state()) {
        dfg.push_back(tmp_dfg);
        tmp_dfg.clear();
        state = elem->get_state();
      }
      tmp_dfg.push_back(elem);
    }
  }

  unsigned ret_change_ltc = 0; // 演算器全体のレイテンシの変化
  // 各ステートに対してスケジューリング
  tmp_dfg.clear();
  for (auto & state_dfg : dfg) {

    // 各Elementのデータ依存がない最小ステップを求める
    for (auto elem = state_dfg.cbegin();
         elem != state_dfg.cend();
         ++elem) {
      auto original_step = (*elem)->get_step();
      if ((*elem)->get_step() == 0) {
        tmp_dfg.push_back(*elem);
        continue;
      }

      // データ依存の判定
      auto min_step
        = this->_min_step_data(tmp_dfg, (*elem));

      // 演算器依存の判定
      min_step
        = this->_min_step_operator
        (tmp_dfg, (*elem)->get_operator(),
         min_step);
      ret_change_ltc += original_step - min_step;

      // 実行ステップの変更
      (*elem)->set_step(min_step);

      // DFGの再構築
      tmp_dfg.push_back(*elem);
      tmp_dfg.sort([](const std::shared_ptr<CDFG_Element> & obj1,
                      const std::shared_ptr<CDFG_Element> & obj2)
                   -> bool {
                     return obj1->get_state() < obj2->get_state()
                       && obj1->get_step() < obj2->get_step();
                   });
    } // for : elem
    // br命令をDFGの最後に移動
    for (auto & elem : tmp_dfg)
      if (elem->get_operator()->get_type()
          == CDFG_Operator::eType::BR) {
        elem->set_step(this->_get_last_step(tmp_dfg));
        break;
      }


    //    _show_list(tmp_dfg);

    // ステートのDFGを更新
    state_dfg = tmp_dfg;
    tmp_dfg.clear();

  } // for : state_dfg

  return ret_change_ltc;
}

/**
   指定された演算器が利用可能となる最小ステップの取得
   @brief データ依存の判定
   @param[in] list 判定対象のDFG (opeより前の実行ステップのもの)
   @param[in] target_elem 判定対象のElement
   @return 指定された演算器の利用可能となる最小ステップ
*/
int
CDFG_Scheduler::_min_step_data
(const std::list<std::shared_ptr<CDFG_Element> > & list,
 const std::shared_ptr<CDFG_Element> & target_elem) {
  auto cp_list = list;

  // データ依存の判定の簡単化のために実行ステップでソート
  cp_list.sort([](const std::shared_ptr<CDFG_Element> & elem1,
                  const std::shared_ptr<CDFG_Element> & elem2)
               -> bool
               {
                 return elem1->get_step() < elem1->get_step();
               });

  auto min_step = 0U;

  // 入力が利用可能となる最小ステップの取得
  for (auto i=0; i<target_elem->get_num_input();
       ++i) {
    auto input = target_elem->get_input_at(i);
    // 入力が即値の場合は考慮しない
    if (input->get_type()
        == CDFG_Node::eNode::PARAM)
      continue;

    // 命令の出力との比較
    for (auto & elem : cp_list) {
      if (elem->get_num_output() == 0)
        continue;

      // 同一基本ブロック内で定義された変数が出力に存在する場合
      // 最小ステップを更新
      if (elem->get_output_at(0) == input)
        min_step
          = std::max(min_step,
                     elem->get_step()
                     + elem->get_operator()->get_latency()
                     + 1
                     + ((elem->get_operator()->get_latency() == 0)
                        ? 0 : 1)); // IPCoreは出力と同一クロックでは読めない

      // 配列の添字が確定する最小ステップで更新
      if (input->get_type()
          == CDFG_Node::eNode::ADDR) {
        auto addr
          = std::dynamic_pointer_cast<CDFG_Addr>
          (input);

        for (auto i=0; i<addr->get_addr_dim();
             ++i)
          if (addr->get_address(i)
              == elem->get_output_at(0))
            min_step
              = std::max(min_step,
                         elem->get_step()
                         + elem->get_operator()->get_latency()
                         + 1
                         + ((elem->get_operator()->get_latency() == 0)
                            ? 0 : 1)); // IPCoreは出力と同一クロックでは読めない
      } // if : input->get_type()
    } // for : elem
  } // for : i

  // 出力が可能となる最小ステップの取得
  if (target_elem->get_num_output() > 0) {
    auto output = target_elem->get_output_at(0);
    if (output->get_type()
        == CDFG_Node::eNode::ADDR) {
      auto addr
        = std::dynamic_pointer_cast<CDFG_Addr>(output);

      for (auto  & elem : cp_list) {
        if (elem->get_num_output() ==0)
          continue;

        for (auto i=0; i<addr->get_addr_dim();
             ++i)
          if (elem->get_output_at(0)
              == addr->get_address(i))
            min_step
              = std::max(min_step,
                         elem->get_step()
                         + elem->get_operator()->get_latency()
                         + 1
                         + ((elem->get_operator()->get_latency() == 0)
                            ? 0 : 1));
      } // for : elem
    } // if : output->get_type()
  }
  return min_step;
} // _min_step_data

/**
   指定された演算器が利用可能となる最小ステップの取得
   @brief 演算器依存の判定
   @param[in] list 判定対象のDFG (opeより前の実行ステップのもの)
   @param[in] ope 判定対象の演算器
   @param[in] data_depend_step データ依存がない最小実行可能ステップ
   @return 指定された演算器の利用可能となる最小ステップ
   @note 演算器のレイテンシは一定とする
   DFGのステートは全て同じとする
*/
int
CDFG_Scheduler::_min_step_operator
(const std::list<std::shared_ptr<CDFG_Element> > & list,
 const std::shared_ptr<CDFG_Operator> & ope,
 const unsigned & data_depend_step) {
  auto latency = ope->get_latency();

  // 同一演算器の命令のリストを取得
  std::list<std::shared_ptr<CDFG_Element> > target_elem;
  for (auto & elem : list)
    if (elem->get_operator() == ope)
      target_elem.push_back(elem);

  // 他の演算器がDFG上に存在しない場合
  if (target_elem.empty())
    return data_depend_step;

  // データ依存と同じタイミングで実行できるか判定
  {
    bool min_ok = true;
    for (auto & elem : target_elem) {
      // データ依存+レイテンシの間に他の命令で演算器が使用されている
      if (!(elem->get_step() + latency + 1 <= data_depend_step
            && elem->get_step() >= data_depend_step + latency + 1))
        min_ok = false;
    }

    if (min_ok)
      return data_depend_step;
  }

  target_elem.sort
    ([](const std::shared_ptr<CDFG_Element> & elem1,
        const std::shared_ptr<CDFG_Element> & elem2)
     -> bool
     {
       return elem1->get_step() <= elem2->get_step();
     });

  // 全ての同一演算の終了タイミングから演算を実施し
  // 他のどの同一演算ともタイミングが被らないかを検証
  for (auto & start_elem : target_elem) {
    bool can_use = true;

    if (start_elem->get_step() + latency
        < data_depend_step)
      continue;

    for (auto & another_elem : target_elem) {
      // start_elem よりも後に実行される another_elem
      if (start_elem->get_step() + latency >
          another_elem->get_step()
          && another_elem->get_step() - start_elem->get_step()
          < latency) {
          // 実行不可能
          can_use = false;
          break;
      }
    } // for : another_elem

      // 新たなタイミングで実行可能な場合
      if (can_use == true)
        return start_elem->get_step() + latency + 1;
  } // for : start_elem

  return std::max(this->_get_last_step(target_elem),
                  data_depend_step);
} // _min_step_operator

/**
   最後に実行が完了する命令の実行終了ステップを取得する
   @param[in] list 取得対象のDFG
   @return 最後に実行が完了する命令の実行終了ステップ
 */
unsigned
CDFG_Scheduler::_get_last_step
(const std::list<std::shared_ptr<CDFG_Element> > & list) {
  auto last_elem = std::max_element
    (list.begin(), list.end(),
     [](const std::shared_ptr<CDFG_Element> & elem1,
        const std::shared_ptr<CDFG_Element> & elem2)
     -> bool
     {
       return elem1->get_step() + elem1->get_operator()->get_latency()
         < elem2->get_step() + elem2->get_operator()->get_latency();
     });

  return (*last_elem)->get_step()
    + (*last_elem)->get_operator()->get_latency()
    + (((*last_elem)->get_operator()->get_latency() == 0) ? 0 : 1);
}

void // for debug
CDFG_Scheduler::_show_list
(const std::list<std::shared_ptr<CDFG_Element> > & list) {
  for (auto & elem : list)
    std::cout << elem->get_operator()->get_latency()
              << ": "
              << elem->get_state()
              << ' '
              << elem->get_step()
              << '\n'
              << std::endl;
}
