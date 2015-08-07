#include "CDFG_Addr.hpp"

/**
   メモリに対する参照か否かを取得
   @return メモリに対する参照か否か
 */
bool
CDFG_Addr::is_mem_ref(void) {
  return this->_ref->get_type()
    == CDFG_Node::eNode::MEM;
}

/**
   レジスタに対する参照か否かを取得
   @return レジスタに対する参照か否か
 */
bool
CDFG_Addr::is_reg_ref(void) {
  return this->_ref->get_type()
    == CDFG_Node::eNode::REG;
}
