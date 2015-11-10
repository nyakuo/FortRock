#include "CDFG_Scheduler.hpp"

/**
   スケジューリングの実行
   @brief データの依存性の確認
   演算器の使用状況の確認
   @return スケジューリング前後での演算器全体での
   レイテンシの変化
*/
unsigned
CDFG_Scheduler::do_schedule
(void) {
  std::list<std::list
            <std::shared_ptr<CDFG_Element> >
            > dfg;
  std::list<std::shared_ptr<CDFG_Element> > tmp_dfg;

  std::map<CDFG_Operator::eType,
           std::list<std::shared_ptr<CDFG_Operator> > > ope_list;

  // DFGを各ステートで切り出す
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
      } // for
  }

  // 演算器の種類ごとの一覧の作成
  for (auto & ope : this->_module->get_operator_list())
    ope_list[ope->get_type()].emplace_back(ope);

  auto ret_change_ltc = 0; // 演算器全体のレイテンシの変化

  // 各ステートに対してスケジューリング
  tmp_dfg.clear();
  for (auto & state_dfg : dfg)
    {
      for (auto elem = state_dfg.cbegin();
           elem != state_dfg.cend();
           ++elem)
        {
          auto original_step = (*elem)->get_step();

          // step(0)実行は依存が発生しない
          if ((*elem)->get_step() == 0)
            {
              tmp_dfg.emplace_back(*elem);
              continue;
            }

          // データ依存の判定
          auto data_depend_step
            = this->_min_step_data(tmp_dfg, (*elem));

          // 演算器依存の判定
          auto min_step = this->_get_last_step(state_dfg);
          {
            std::shared_ptr<CDFG_Operator> & min_ope
              = (*elem)->get_operator();

            if (ope_list[min_ope->get_type()].size() > 0)
              {
                for (auto & ope : ope_list[min_ope->get_type()])
                  {
                    auto new_min_step
                      = this->_min_step_operator
                      (tmp_dfg,
                       ope,
                       data_depend_step);

                    if (new_min_step < min_step)
                      {
                        // 実行演算器の変更
                        (*elem)->set_operator(ope);
                        min_step = new_min_step;
                      }
                  } // for : ope
              } // if : ope_list.size() > 0

            else
              min_step = data_depend_step;
          }

          // 実行ステップの変更
          (*elem)->set_step(min_step);

          ret_change_ltc += original_step - min_step;

          // DFGの再構築
          tmp_dfg.emplace_back(*elem);
          tmp_dfg.sort([](const std::shared_ptr<CDFG_Element> & obj1,
                          const std::shared_ptr<CDFG_Element> & obj2)
                       -> bool
                       {
                         return obj1->get_state() < obj2->get_state()
                           && obj1->get_step() < obj2->get_step();
                       });
        } // for : elem

      // br命令をDFGの最後に移動
      for (auto & elem : tmp_dfg)
        if (elem->get_operator()->get_type()
            == CDFG_Operator::eType::Br)
          {
            elem->set_step(this->_get_last_step(tmp_dfg));
            break;
          }

      //    _show_list(tmp_dfg);

      // ステートのDFGを更新
      state_dfg = tmp_dfg;
      tmp_dfg.clear();

    } // for : state_dfg

  return ret_change_ltc;
} // do_schedule

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
 const std::shared_ptr<CDFG_Element> & target_elem)
{
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
        == CDFG_Node::eNode::Param)
      continue;

    // 命令の出力との比較
    for (auto & elem : cp_list)
      {
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
            == CDFG_Node::eNode::Addr)
          {
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
  if (target_elem->get_num_output() > 0)
    {
      auto output = target_elem->get_output_at(0);

      // SREMの場合は余剰に接続
      if (target_elem->get_operator()->get_type()
          == CDFG_Operator::eType::Srem)
        output = target_elem->get_output_at(1);

      if (output->get_type()
          == CDFG_Node::eNode::Addr)
        {
          auto addr
            = std::dynamic_pointer_cast<CDFG_Addr>(output);

          for (auto  & elem : cp_list)
            {
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
    } // if : target_elem

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
 const unsigned & data_depend_step)
{
  auto latency = ope->get_latency();

  // 同一演算器の命令のリストを取得
  std::list<std::shared_ptr<CDFG_Element> > target_elem;
  for (auto & elem : list)
    if (elem->get_operator() == ope)
      target_elem.emplace_back(elem);

  // 他の演算が前に存在しない場合
  if (target_elem.empty())
    return data_depend_step;

  // データ依存と同じタイミングで実行できるか判定
  if (this->_can_use(data_depend_step,
                     ope,
                     target_elem))
    return data_depend_step;

  auto step = data_depend_step+1;
  auto last_step = this->_get_last_step(target_elem);

  for (;step < this->_get_last_step(target_elem);
       ++step)
    {
      if (this->_can_use(step, ope, target_elem))
        return step;
    }

  return last_step;
} // _min_step_operator

/**
   指定されたステップで演算器が使用可能か判定
   @param[in] step 演算の実行開始ステップ
   @param[in] ope 演算に使用する演算器
   @param[in] dfg 演算前のdfg
   @return 指定されたステップで演算器が使用可能か
 */
bool
CDFG_Scheduler::_can_use
(const unsigned & step,
 const std::shared_ptr<CDFG_Operator> & ope,
 const std::list<std::shared_ptr<CDFG_Element> > & dfg)
{
  auto latency = ope->get_latency();

  for (auto & elem : dfg)
    {
      if (elem->get_operator() != ope)
        continue;

      auto tmp_step = elem->get_step();
      if (step < tmp_step + latency
          && step + latency > tmp_step)
        return false;
    }

  return true;
} // _can_use
#if 0
/**
   指定されたステップでRAMが使用可能か判定
   @param[in] step 演算の実行開始ステップ
   @param[in] ram 演算に使用するRAM
   @parma[in] dfg 演算前のdfg
   @return 指定されたステップでRAMが使用可能か
 */
bool
CDFG_Scheduler::_can_use
(const unsigned & step,
 const std::shared_ptr<CDFG_Ram> & ram,
 const std::list<std::shared_ptr<CDFG_Element> > & dfg)
{
  auto latency = ram->get_latency();
  bool can_use = true;
  auto num_col = 0;

  for (auto & elem : dfg)
    {
      // 他入力との衝突判定
      for (auto i=0; i<elem->get_num_input();
           ++i)
        {
          auto in = elem->get_input_at(i);

          if (in == ram)
            {
              auto tmp_step = elem->get_step();

              // 衝突
              if (tmp_step <= step + ram->get_latency()
                  && tmp_step + ram->get_latency() >= step)
                ++num_col;
            }
        } // for : i

      auto out = elem->get_output_at(0);
      if (out == ram)
        {
          auto tmp_step = elem->get_step();

          // 衝突
          if (tmp_step <= step + ram->get_latency()
              && tmp_step + ram->get_latency() >= step)
            ++num_col;
        }
    } // for : elem

  return can_use;
} // _can_use
#endif
/**
   最後に実行が完了する命令の実行終了ステップを取得する
   @param[in] list 取得対象のDFG
   @return 最後に実行が完了する命令の実行終了ステップ
 */
unsigned
CDFG_Scheduler::_get_last_step
(const std::list<std::shared_ptr<CDFG_Element> > & list)
{
  auto last_elem = std::max_element
    (list.begin(), list.end(),
     [](const std::shared_ptr<CDFG_Element> & elem1,
        const std::shared_ptr<CDFG_Element> & elem2)
     -> bool
     {
       return elem1->get_step() + elem1->get_operator()->get_latency()
         < elem2->get_step() + elem2->get_operator()->get_latency();
     });

  // 演算器を使用する場合は +1
  return (*last_elem)->get_step()
    + (*last_elem)->get_operator()->get_latency()
    + (((*last_elem)->get_operator()->get_latency() == 0) ? 0 : 1);
} // get_last_step

// for debug
#include <iostream>
void
CDFG_Scheduler::_show_list
(const std::list<std::shared_ptr<CDFG_Element> > & list)
 {
  for (auto & elem : list)
    std::cout << elem->get_operator()->get_latency()
              << ": "
              << elem->get_state()
              << ' '
              << elem->get_step()
              << '\n'
              << std::endl;
} // _show_list
