#ifndef _CDFG_RAM_H
#define _CDFG_RAM_H

#include <string>
#if 0
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#endif

#include "CDFG_Mem.hpp"
#include "CDFG_Array.hpp"
#include "../core/fortrock.hpp"
#if 0
#include "../xml/CNode_data.hpp"
#endif
/**
   @class CDFG_Ram
   @brief RAMモジュールを表すクラス
 */
class CDFG_Ram : public CDFG_Mem {
public:
  CDFG_Ram(const std::string & name,
           const unsigned & num_datas,
           const unsigned & word_length,
           const unsigned & address_width,
           const eDataType & data_type,
           const bool & is_initialized,
           const unsigned & latency);

  // override
  virtual std::string init_string
  (const std::string & indent) override final;
  virtual std::string define_string(void) override final;
  virtual std::string access_string
  (const std::shared_ptr<CDFG_Addr> & addr) override final;

  // setter
  static void set_num_port(const unsigned & num_r_port,
                           const unsigned & num_w_port);

  // getter
  std::shared_ptr<CDFG_Reg> get_address_port(const unsigned & at);
  std::shared_ptr<CDFG_Reg> get_rw_port(const unsigned & at);
  unsigned get_num_addr_port(void);
  unsigned get_num_rw_port(void);

private:
  unsigned _address_width;                   ///< アドレス幅
  std::vector<std::shared_ptr<CDFG_Reg> > _address_ports; ///< アドレスポート
  std::vector<std::shared_ptr<CDFG_Reg> > _rw_ports;      ///< R/Wポート (0: read  1: write)
  const static std::string _RAM_MODULE_NAME; ///< RAMのモジュール名
  static unsigned _NUM_INSTANCE;             ///< インスタンス化されたモジュールの数
  static unsigned _NUM_R_PORT;               ///< 読み出しポート数
  static unsigned _NUM_W_PORT;               ///< 書き込みポート数
};

#endif
