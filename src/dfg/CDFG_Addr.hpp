#ifndef _CDFG_ADDR_H
#define _CDFG_ADDR_H

#include <string>
#include <memory>
#include <vector>

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
            const std::shared_ptr<CDFG_Node> & ref = NULL,
            const std::shared_ptr<CDFG_Node> & addr = NULL)
    : CDFG_Node(asm_name,
                bit_width,
                false, // == is signed
                CDFG_Node::eNode::Addr,
                "ra_")
  {
    if (ref != NULL)
      this->_ref = ref;

    if (addr != NULL)
      this->_addr.push_back(addr);
  }

  ~CDFG_Addr(void) {}

  // setter
  void add_addr(const std::shared_ptr<CDFG_Node> & addr)
  { this->_addr.push_back(addr); }
  void set_ref(const std::shared_ptr<CDFG_Node> & ref)
  { this->_ref = ref; }

  // getter
  virtual bool is_input(void) override final { return false; }
  virtual bool is_output(void) override final { return false; }

  bool is_mem_ref(void);
  bool is_reg_ref(void);
  bool is_param_ref(void);

  /**
     参照対象の取得
     @return メモリ，レジスタへの参照
   */
  std::shared_ptr<CDFG_Node> &
  get_reference(void) { return this->_ref; }

  /**
     アドレスの取得
     @param[in] idx 取得する
   */
  std::shared_ptr<CDFG_Node> &
  get_address(const unsigned & idx)
  { return this->_addr[idx]; }

  /**
     アドレスの数を取得
     @return アドレスの数 (配列の次元)
   */
  unsigned get_addr_dim(void)
  { return this->_addr.size(); }

private:
  std::vector<std::shared_ptr<CDFG_Node> > _addr;
  std::shared_ptr<CDFG_Node> _ref; ///< 参照 (mem or reg)
};

#endif
