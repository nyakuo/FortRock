#ifndef _CDFG_LOADELEM_H
#define _CDFG_LOADELEM_H

#include "CDFG_Element.hpp"
#include "CDFG_Addr.hpp"
#include "CDFG_Mem.hpp"
#include "CDFG_Ram.hpp"

/**
   @class CDFG_LoadElem
   @brief Load命令を表すElementクラス
 */
class CDFG_LoadElem : public CDFG_Element {
public:
  CDFG_LoadElem(const unsigned & port_num,
                const bool & is_gepope,
                const unsigned & state,
                const unsigned & step,
                const unsigned & latency);

  ~CDFG_LoadElem(void) {}

  // setter
  void set_use_port(const unsigned & port_num);

  // getter
  bool is_gepope(void);
  bool is_mem_load(void);

  // override
  virtual std::string input_from_str(const unsigned & at=0) override final;
  virtual std::string input_to_str(const unsigned & at=0) override final;
  virtual std::string output_from_str(void) override final;
  virtual std::string output_to_str(void) override final;
  virtual std::string other_str(const std::string & indent) override final;

private:
  const bool _is_gepope; ///< getelementptr命令を含むか否か
  unsigned _port_num; ///< 使用するアクセスポート
};

#endif
