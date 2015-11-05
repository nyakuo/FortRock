#include "CDFG_Node.hpp"

/**
   コンストラクタ
   @param[in] name NodeのLLVM IR上での名前
   @param[in] bit_width Nodeのビット幅
   @param[in] is_signed Nodeが is_signed かどうか
   @param[in] type Nodeの種類
   @param[in] parameter Nodeに設定する定数 (定数やアドレス)
   @note verilog_name の作成も行っている
 */
CDFG_Node::CDFG_Node(const std::string & asm_name,
                     const unsigned & bit_width,
                     const bool & is_signed,
                     const CDFG_Node::eNode & type,
                     const std::string & prefix)
  : _asm_name(asm_name),
    _bit_width(bit_width),
    _is_signed(is_signed),
    _type(type)
{
  // 変数に使用できない文字の置換
  std::string safe_name = this->get_safe_name();
  this->_verilog_name = prefix + safe_name;
} // CDFG_Node

/**
   Node名に含まれる'.'文字を'_'に変換する
   @param[in] name 変換する名前
   @return 変換後の名前
   @note CModuleGeneratorでPHI命令の出力に使用
 */
std::string
CDFG_Node::get_safe_name(void)
{
  auto safe_name = this->_asm_name;

  int at;

  while((at = safe_name.find('.', 0)) != std::string::npos)
    safe_name.replace(at, 1, "_");

  while((at = safe_name.find('-', 0)) != std::string::npos)
    safe_name.replace(at, 1, "m");

  return safe_name;
}

/**
   NodeのLLVM IR上での信号名を取得
   @return NodeのLLVM IR上での信号名
 */
std::string &
CDFG_Node::get_asm_name(void)
{
  return this->_asm_name;
}

/**
   NodeのVerilog HDL上での信号名を取得
   @return NodeのVerilog HDL上での信号名
 */
std::string &
CDFG_Node::get_verilog_name(void)
{
  return this->_verilog_name;
}

/**
   Nodeのビット幅の取得
   @return Nodeのビット幅
 */
unsigned &
CDFG_Node::get_bit_width(void)
{
  return this->_bit_width;
}

/**
   Nodeが is_signed かどうかを取得
   @return Nodeが is_signed かどうか
 */
bool &
CDFG_Node::get_is_signed(void)
{
  return this->_is_signed;
}

/**
   Nodeの種類を取得
   @return Nodeの種類
 */
CDFG_Node::eNode &
CDFG_Node::get_type(void)
{
  return this->_type;
}

/**
   Nodeの種類の再設定
   @param[in] type 再設定する種類
   @note fortrock.cpp(FortRockの本体)で利用するためだけに実装
 */
void CDFG_Node::set_type
(const CDFG_Node::eNode & type)
{
  this->_type = type;
}
