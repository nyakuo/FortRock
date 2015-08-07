#ifndef _CDFG_ADDR_H
#define _CDFG_ADDR_H

#include <string>
#include <memory>
#include "CDFG_Node.hpp"
#include "CDFG_Mem.hpp"

class CDFG_Mem; // 循環参照によるクラスの重複定義対応

/**
   @class CDFG_Addr
   @brief アドレスを表すクラス
 */
class CDFG_Addr : public CDFG_Node {
public:
  /**
     コンストラクタ
     @param[in] asm_name アセンブリ上での信号名
     @param[in] bit_width 参照先の型のビット幅
     @param[in] mem 参照 (mem or reg)
     @param[in] addr アドレス
   */
  CDFG_Addr(const std::string & asm_name,
            const unsigned & bit_width,
            const std::shared_ptr<CDFG_Node> & ref,
            const unsigned & addr)
    : _addr(addr),
      _ref(ref),
      CDFG_Node(asm_name,
                bit_width,
                false, // == is signed
                CDFG_Node::eNode::ADDR,
                "ra_") {}

  CDFG_Addr(const std::string & asm_name,
            const unsigned & bit_width,
            const std::shared_ptr<CDFG_Node> & ref)
    : _addr((unsigned)0),
      _ref(ref),
      CDFG_Node(asm_name,
                bit_width,
                false, // == is signed
                CDFG_Node::eNode::ADDR,
                "ra_") {}

  ~CDFG_Addr(void) {}

  // getter
  virtual bool is_input(void) override final { return false; }
  virtual bool is_output(void) override final { return false; }
  bool is_mem_ref(void);
  bool is_reg_ref(void);
  std::shared_ptr<CDFG_Node> &
  get_reference(void) { return this->_ref; }

private:
  const unsigned _addr;            ///< アドレス
  std::shared_ptr<CDFG_Node> _ref; ///< 参照 (mem or reg)
};

#endif
