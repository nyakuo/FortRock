#include "CDFG_RetElem.hpp"

/**
   ���󥹥ȥ饯��
   @brief ��λ���ơ��Ȥλ��Ȥ�����
   @param[in] finish_state ��λ���ơ��ȥ�٥�λ���
   @param[in] state_node state�Ρ��ɤλ���
   @param[in] step_node step�Ρ��ɤλ���
   @param[in] prev_state prev_state�Ρ��ɤλ���
   @param[in] state ̿��¹Գ��ϥ��ơ���
   @param[in] step ̿��¹Գ��ϥ��ƥå�
 */
CDFG_RetElem::CDFG_RetElem
(const std::shared_ptr<CDFG_Node> & finish_state,
 const std::shared_ptr<CDFG_Node> & state_node,
 const std::shared_ptr<CDFG_Node> & step_node,
 const std::shared_ptr<CDFG_Node> & prev_state,
 const std::shared_ptr<CDFG_Node> & fin_signal,
 const std::shared_ptr<CDFG_Node> & true_signal,
 const unsigned & state,
 const unsigned & step)
  : _finish_state(std::dynamic_pointer_cast<CDFG_Label>(finish_state)),
    _state_node(state_node),
    _step_node(step_node),
    _prev_state(prev_state),
    _fin_signal(fin_signal),
    _true_signal(true_signal),
    CDFG_Element(CDFG_Operator::eType::Ret,
                 0,
                 state, step)
{} // CDFG_RetElem

/**
   ��λ���֤Υ�����(Verilog HDL)�μ���
   @param[in] indent ����ǥ��
   @return ��λ���֤Υ�����(Verilog HDL)
 */
std::string
CDFG_RetElem::finish_state_str
(const std::string & indent)
{
  return indent
    + this->_fin_signal->get_verilog_name()
    + " <= "
    + this->_true_signal->get_verilog_name()
    + ";\n"
    + indent
    + this->_state_node->get_verilog_name()
    + " <= 0;\n";
} // finish_state_str

/**
   ��λ���ơ����ֹ�����
   @return ��λ���ơ����ֹ�
 */
unsigned
CDFG_RetElem::get_finish_state
(void)
{
  return this->_finish_state->get_state();
} // get_finish_state

/**
   ��λ���ơ��Ȥؤ�����̿��Υ�����(Verilog HDL)�μ���
   @param[in] inent ����ǥ��
   @return ��λ���ơ��Ȥؤ�����̿��Υ�����(Verilog HDL)
 */
std::string
CDFG_RetElem::other_str
(const std::string & indent)
{
  return indent
    + this->_state_node->get_verilog_name()
    + " <= "
    + this->_finish_state->get_verilog_name()
    + ";\n"
    + indent
    + this->_step_node->get_verilog_name()
    + " <= 0;\n";
} // other_str
