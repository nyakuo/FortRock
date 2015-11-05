#include "CStateMachineGen.hpp"

/**
   ステートマシンの処理を出力
   @param[in] state 取得するステートの番号
   @param[in] step 取得するステップの番号
   @return ステートとステップで指定されたタイミングの処理
*/
std::string
CStateMachineGen::gen_state_machine
(const unsigned & state,
 const unsigned & step)
{
  auto ite_state = this->_state_machine.find(state);
  if (ite_state == this->_state_machine.end())
    return "";

  auto ite_step = ite_state->second.find(step);
  if (ite_step == ite_state->second.end())
    return "";

  return ite_step->second;
}

/**
   ステートマシンの処理をリストに追加
   @param[in] state 処理を実行するステートの番号
   @param[in] step 処理を実行するステップの番号
   @param[in] process 処理の内容 (接続)
*/
void
CStateMachineGen::add_state_process
(const unsigned & state,
 const unsigned & step,
 const std::string & process)
{
  // ステートの存在確認
  auto ite_state = this->_state_machine.find(state);
  if (ite_state == this->_state_machine.end())
    {
      // ステートの追加
      std::map<unsigned, std::string> map_state;

      this->_state_machine.insert
        (std::pair<unsigned,
         std::map<unsigned, std::string> >
         (state, map_state));

      ite_state = this->_state_machine.find(state);
  }

  // ステップの存在確認
  auto ite_step = ite_state->second.find(step);
  if (ite_step == ite_state->second.end())
    {
      // ステップの追加
      ite_state->second.insert
        (std::pair<unsigned, std::string>(step, ""));
      ite_step = ite_state->second.find(step);
    }

  // 処理の追加
  ite_step->second.append(process);
}

/**
   ステートマシンに存在するすべてのステート・ステップのペアのリストを取得
   @return ステートマシンに存在するすべてのステート・ステップのペアのリスト
*/
std::multimap<unsigned, unsigned>
CStateMachineGen::get_state_step_list
(void)
{
  std::multimap<unsigned, unsigned> ret_list;

  for (auto & state : this->_state_machine)
    for (auto & step : state.second)
      ret_list.insert(std::pair<unsigned, unsigned>
                      (state.first, step.first));

  return ret_list;
}
