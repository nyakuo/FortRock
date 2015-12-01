#include "CDFG_PhiElem.hpp"

/**
   ���󥹥ȥ饯��
   @param[in] num_input ���Ϥο�
   @param[in] state �¹Գ��ϥ��ơ���
   @param[in] step �¹Գ��ϥ��ƥå�
 */
CDFG_PhiElem::CDFG_PhiElem
(const unsigned & num_input,
 const std::shared_ptr<CDFG_Node> prev_state,
 const unsigned & state,
 const unsigned & step)
  : _prev_state(prev_state),
    CDFG_Element(CDFG_Operator::eType::Phi,
                 num_input,
                 state,
                 step)
{
  this->_labels.resize(num_input);
}

/**
   phi̿��ξ���٥������
   @param[in] node ��٥�λ���
   @param[in] at ���ꤹ�����
 */
void
CDFG_PhiElem::set_label
(const std::shared_ptr<CDFG_Node> & node,
 const unsigned & at)
{
  this->_labels[at] = node;
} // set_label

/**
   prev_state�Ρ��ɤλ��Ȥ����
   @return prev_state�Ρ��ɤλ���
 */
const std::shared_ptr<CDFG_Node> &
CDFG_PhiElem::get_prev_state
(void) const
{
  return this->_prev_state;
} // get_prev_state

/**
   �������٥�̾�μ���
   @param[in] at ���������������٥�ΰ���
   @return �������٥�̾
 */
const std::string
CDFG_PhiElem::get_label_str
(const unsigned & at) const
{
  return this->_labels[at]->get_verilog_name();
} // get_label_str

/**
   �������Υ�����(Verilog HDL)���������
   @param[in] at �����������Ϥΰ���
   @return �������Υ�����(Verilog HDL)
 */
std::string
CDFG_PhiElem::input_from_str
(const unsigned & at)
{
  return this->_get_input_str(at);
} // input_from_str

/**
   �ͤ�������μ���
   @param[in] at don't care
 */
std::string
CDFG_PhiElem::input_to_str
(const unsigned & at)
{
  return "phi_" + this->get_output_at(0)->get_verilog_name();
} // input_to_str

/**
   ������Υ쥸����̾(Verilog HDL)�μ���
   @return ������Υ쥸����̾(Verilog HDL)
 */
std::string
CDFG_PhiElem::output_to_str
(void)
{
  return this->get_output_at(0)->get_verilog_name();
} // output_to_str

/**
   ���ϸ��Υ�����(Verilog HDL)�μ���
   @brief function��assign���줿wire
   @return ���ϸ��Υ�����(Verilog HDL)
 */
std::string
CDFG_PhiElem::output_from_str
(void)
{
  return "w_phi_" + this->get_output_at(0)->get_verilog_name();
} // output_from_str
