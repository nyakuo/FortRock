#include "CDFG_Addr.hpp"

/**
   メモリに対する参照か否かを取得
   @return メモリに対する参照か否か
 */
bool
CDFG_Addr::is_mem_ref(void) {
  return this->_ref->get_type()
    == CDFG_Node::eType::Mem;
}

/**
   レジスタに対する参照か否かを取得
   @return レジスタに対する参照か否か
 */
bool
CDFG_Addr::is_reg_ref(void) {
  return this->_ref->get_type()
    == CDFG_Node::eType::Reg;
}

/**
   パラメータに対する参照か否かを取得
   @return パラメータに対する参照か否か
 */
bool
CDFG_Addr::is_param_ref(void) {
  return this->_ref->get_type()
    == CDFG_Node::eType::Param;
}
