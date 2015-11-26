#ifndef _CDFG_STOREELEM_H
#define _CDFG_STOREELEM_H

#include "CDFG_Element.hpp"
#include "CDFG_Addr.hpp"
#include "CDFG_Ram.hpp"

/**
   @class CDFG_StoreElem
   @brief Store 命令を表すクラス
 */
class CDFG_StoreElem : public CDFG_Element {
public:
  CDFG_StoreElem(const unsigned & state,
                 const unsigned & step,
                 const unsigned & latency);

  // setter
  void set_use_port(const unsigned & port_num);
  bool is_mem_store(void);

  // override
  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string input_to_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;
  virtual std::string other_str(const std::string & indent) override final;

private:
  unsigned _port_num; ///< 使用するアクセスポート
};

#endif
