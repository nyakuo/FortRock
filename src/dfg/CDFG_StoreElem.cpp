#include "CDFG_StoreElem.hpp"

/**
   コンストラクタ
   @param[in] state 命令実行ステート
   @param[in] step 命令実行ステップ
   @param[in] latency
*/
CDFG_StoreElem::CDFG_StoreElem
(const unsigned & state,
 const unsigned & step,
 const unsigned & latency)
  : CDFG_Element(CDFG_Operator::eType::Store,
                 1, // 入力数
                 state, step, latency)
{} // CDFG_StoreElem

/**
   アクセスに使用するポートの設定
   @param[in] port_num アクセスに使用するポート
 */
void
CDFG_StoreElem::set_use_port
(const unsigned & port_num)
{
  this->_port_num = port_num;
} // set_use_port

/**
   メモリの書き込み命令か判定
   @return メモリの書き込み命令か否か
 */
bool
CDFG_StoreElem::is_mem_store
(void)
{
  auto out = std::dynamic_pointer_cast<CDFG_Addr>
    (this->get_output_at(0));

  // レジスタおよびパラメータ参照の場合
  if (out->is_reg_ref())
    return false;
  else
    return true;
}

/**
   演算の入力のコード(Verilog HDL)の取得
   @param[in] at don't care
   @return 演算の入力のコード(Verilog HDL)
 */
std::string
CDFG_StoreElem::input_from_str
(const unsigned & at)
{
  return this->_get_input_str(0);
} // input_from_str

/**
   入力先のコード(Verilog HDL)の取得
   @param[in] at don't care
   @return 入力先のコード(Verilog HDL)
 */
std::string
CDFG_StoreElem::input_to_str
(const unsigned & at)
{
  return this->output_to_str();
}

/**
   出力元のコード(Verilog HDL)の取得
   @return 出力元のコード(Verilog HDL)
 */
std::string
CDFG_StoreElem::output_from_str
(void)
{
  return this->input_from_str();
} // output_from_str

/**
   演算の出力の文字列(Verilog HDL)の取得
   @return 演算の出力の文字列(Verilog HDL)
 */
std::string
CDFG_StoreElem::output_to_str
(void)
{
  std::string ret_str("");
  auto out = std::dynamic_pointer_cast<CDFG_Addr>
    (this->get_output_at(0));

  if (out->is_reg_ref())
    ret_str.assign
      (out->get_reference()->get_verilog_name());

  else if (out->is_mem_ref())
    ret_str.assign
      (std::dynamic_pointer_cast<CDFG_Mem>
       (out->get_reference())->access_string(out));

  return ret_str;
} // output_to_str

/**
   メモリのアクセス制御コード(Verilog HDL)の出力
   @param[in] indent インデント
   @return メモリのアクセス制御コード(Verilog HDL)
 */
std::string
CDFG_StoreElem::other_str
(const std::string & indent)
{
  std::string ret_str("");
  auto addr = std::dynamic_pointer_cast<CDFG_Addr>
    (this->get_output_at(0));

  if (addr->is_mem_ref())
    {
      auto mem = std::dynamic_pointer_cast<CDFG_Mem>
        (addr->get_reference());

      if (mem->get_mem_type() == CDFG_Mem::eMemType::Ram)
        {
          auto ram  = std::dynamic_pointer_cast<CDFG_Ram>(mem);
          auto port_num = 0;

          // rw信号を write mode(1) に
          ret_str.assign
            (indent
             + ram->get_rw_port(port_num)->get_verilog_name()
             + " <= 1;\n");

          // アドレスポートに書き込むアドレスを指定
          ret_str.append
            (indent
             + ram->get_address_port(port_num)->get_verilog_name()
             + " <= "
             + addr->get_address(0)->get_verilog_name()
             + ";\n");
        }
    }

  return ret_str;
} // other_str
