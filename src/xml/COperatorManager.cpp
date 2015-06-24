#include "COperatorManager.hpp"

COperatorManager::COperatorManager(void){}

/**
   演算器の追加
   @param[in] type 演算器の種類
   @param[in] ope 演算器の参照
 */
void COperatorManager::add_operator
(std::shared_ptr<CDFG_Operator> & ope) {
  this->_operators[ope->get_type()].push_back(ope);
}

/**
   同時に使用可能な演算器の数を返す
   @param[in] type 同時使用可能数を取得したい演算器の種類
   @return 演算器の同時使用可能数
 */
unsigned COperatorManager::get_num_module
(const CDFG_Operator::eType & type) {
  return this->_operators[type].size();
}

/**
   モジュールの取得
   @param[in] at 取得するモジュールの位置
   @param[in] type 取得するモジュールの種類
   @return モジュールの参照
 */
std::shared_ptr<CDFG_Operator>
COperatorManager::get_module_at
(const unsigned & at,
 const CDFG_Operator::eType & type) {
  return this->_operators[type].at(at);
}
