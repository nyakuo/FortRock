#include "CDFG_ZextElem.hpp"

/**
   ���󥹥ȥ饯��
   @param[in] state ̿��¹Գ��ϥ��ơ���
   @param[in] step ̿��¹Գ��ϥ��ƥå�
 */
CDFG_ZextElem::CDFG_ZextElem
(const unsigned & state,
 const unsigned & step)
  : CDFG_Element(CDFG_Operator::eType::Zext,
                 1, // num input
                 state, step)
{} // CDFG_ZextElem

/**
   ̿������ϸ��Υ�����(Verilog HDL)�ν���
   @parma[in] at don't care
   @return ̿����������Υ�����(Verilog HDL)
 */
std::string
CDFG_ZextElem::input_from_str
(const unsigned & at)
{
  return this->_get_input_str(at);
} // input_from_str

/**
   ̿��ν��ϸ��Υ�����(Verilog HDL)�ν���
   @return ̿��ν������Υ�����(Verilog HDL)
 */
std::string
CDFG_ZextElem::output_from_str
(void)
{
  return "("
    + this->get_input_at(0)->get_verilog_name()
    + "| 0)";
} // output_from_str

/**
   ̿��ν�����Υ�����(Verilog HDL)�μ���
   @return ̿��ν�����Υ�����(Verilog HDL)
 */
std::string
CDFG_ZextElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
} // output_to_str
