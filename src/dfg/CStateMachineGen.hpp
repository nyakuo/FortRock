#ifndef _CSTATE_MACHINE_GEN_H
#define _CSTATE_MACHINE_GEN_H

#include <string>
#include <map>
#include <list>
#include <algorithm>

/**
   @class CStateMachineGen
   @brief ステートマシンの処理を記述するクラス
 */
class CStateMachineGen {
public:
  CStateMachineGen(void) {}
  ~CStateMachineGen(void) {}

  std::string gen_state_machine(const unsigned & state,
                                const unsigned & step); /** ステートマシンの処理を出力 */

  void add_state_process(const unsigned & state,
                         const unsigned & step,
                         const std::string & process); /** ステートマシンの処理をリストに追加 */

  std::multimap<unsigned, unsigned> get_state_step_list(void); /** ステートマシンに存在する
                                                                                       すべてのステート・ステップの
                                                                                       ペアのリストを取得 */

private:
  /**
     ステートマシンの処理を記述
     <state, <step, 接続情報 (文字列)> >
   */
  std::map<unsigned, std::map<unsigned, std::string> > _state_machine;
};

#endif
