#include "CDFG_LoadElem.hpp"

/**
   コンストラクタ
   @param[in] is_gepope getelementptr命令を含むか否か
   @param[in] state 命令の実行ステート
   @param[in] step 命令の実行ステップ
   @param[in] addr 読み込み元のアドレス
   @param[in] latency BRAMの読み込みレイテンシ
*/
CDFG_LoadElem::CDFG_LoadElem
(const bool & is_gepope,
 const unsigned & state,
 const unsigned & step,
 const unsigned & latency)
  : _is_gepope(is_gepope),
    CDFG_Element(CDFG_Operator::eType::Load,
                 1, // 入力数
                 state,
                 step,
                 latency) {}
/**
   getelementptr命令を含むか否かを取得
   @return getelementptr命令を含むか否か
*/
bool
CDFG_LoadElem::is_gepope
(void)
{
  return this->_is_gepope;
} // is_gepope

/**
   メモリの読み込み命令か判定
   @return メモリの読み込み命令か否か
 */
bool
CDFG_LoadElem::is_mem_load
(void)
{
  auto in = std::dynamic_pointer_cast<CDFG_Addr>
    (this->get_input_at(0));

  // レジスタおよびパラメータ参照の場合
  if (in->is_reg_ref() || in->is_param_ref())
    return false;
  else
    return true;
}

/**
   演算の入力の文字列(Verilog HDL)の取得
   @param[in] at don't care
   @return 演算の入力の文字列(Verilog HDL)
 */
std::string
CDFG_LoadElem::input_from_str
(const unsigned & at)
{
  std::string ret_str("");

  auto in = std::dynamic_pointer_cast<CDFG_Addr>
    (this->get_input_at(0));

  // レジスタおよびパラメータ参照の場合
  if (in->is_reg_ref() || in->is_param_ref())
    ret_str.assign(in->get_reference()->get_verilog_name());

  // メモリ参照の場合
  else if (in->is_mem_ref())
    {
      auto mem = std::dynamic_pointer_cast<CDFG_Mem>
        (in->get_reference());

      switch (mem->get_mem_type()) {
      case CDFG_Mem::eMemType::Array:
      case CDFG_Mem::eMemType::Ram:
        ret_str.assign(mem->access_string(in));

      default:;
      } // switch
    } // if : is_mem_ref()

  return ret_str;
} // input_from_str

/**
   入力先を出力 (Verilog HDL)
   @param[in] at don't care
   @return 入力先 (Verilog HDL)
 */
std::string
CDFG_LoadElem::input_to_str
(const unsigned & at)
{
  return this->output_to_str();
} // input_to_str

/**
   演算の出力の文字列(Verilog HDL)の取得
   @return 演算の出力の文字列(Verilog HDL)
 */
std::string
CDFG_LoadElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
} // output_str

/**
   出力元を出力 (Verilog HDL)
   @return 出力元 (Verilog HDL)
 */
std::string
CDFG_LoadElem::output_from_str
(void)
{
  return this->input_from_str();
} // output_from_str

/**
   メモリアクセス制御信号の操作を出力
   @param[in] indent インデント
   @return メモリアクセス制御信号の操作 (Veriog HDL)
 */
std::string
CDFG_LoadElem::other_str
(const std::string & indent)
{
  std::string ret_str("");

  if (this->is_mem_load())
    {
      auto addr = std::dynamic_pointer_cast<CDFG_Addr>
        (this->get_input_at(0));
      auto mem = std::dynamic_pointer_cast<CDFG_Mem>
        (addr->get_reference());

      if (mem->get_mem_type() == CDFG_Mem::eMemType::Ram)
        {
          auto port_num = 0;
          auto ram = std::dynamic_pointer_cast<CDFG_Ram>(mem);

          // rw の値を read mode(0) に
          ret_str.append
            (indent
             + ram->get_rw_port(port_num)->get_verilog_name()
             + " <= 0;\n");

          // アドレスポートに読み出すアドレスを指定
          ret_str.append
            (indent
             + ram->get_address_port(port_num)->get_verilog_name()
             + " <= "
             + addr->get_address(0)->get_verilog_name()
             + ";\n");
        } // if : Ram
    } // if : is_mem_load

  return ret_str;
} // other_str
