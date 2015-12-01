#include "CDFG_SelectElem.hpp"

/**
   ���󥹥ȥ饯��
   @param[in] state ̿��¹Գ��ϥ��ơ���
   @param[in] step ̿��¹Գ��ϥ��ƥå�
 */
CDFG_SelectElem::CDFG_SelectElem
(const std::shared_ptr<CDFG_Node> & tf_node,
 const unsigned & state,
 const unsigned & step)
  : _tf_node(tf_node),
    CDFG_Element(CDFG_Operator::eType::Select,
                 2, // num input
                 state, step)
{} // CDFG_SelelctElem


/**
   �������ͤΥ�����(Verilog HDL)�����
   @param[in] at ����������� (0 or 1)
   @return �������ͤΥ�����(Verilog HDL)
 */
std::string
CDFG_SelectElem::input_from_str
(const unsigned & at)
{
  return this->_get_input_str(at);
} // input_from_str

/**
   �������Υ�����(Verilog HDL)�����
   @return �������Υ�����(Verilog HDL)
 */
std::string
CDFG_SelectElem::output_from_str
(void)
{
  std::string tf;

  if (this->_tf_node->get_type() == CDFG_Node::eType::Elem)
    tf = std::dynamic_pointer_cast<CDFG_Element>
      (this->_tf_node)->output_from_str();
  else
    tf = this->_tf_node->get_verilog_name();

  return "(" + tf + " ? "
    + this->input_from_str(0)
    + " : "
    + this->input_from_str(1)
    + ")";
} // output_from_str

/**
   �������Υ�����(Verilog HDL)�μ���
   @return �������Υ�����(Verilog HDL)
 */
std::string
 CDFG_SelectElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
} // output_to_str
