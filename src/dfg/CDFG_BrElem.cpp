#include "CDFG_BrElem.hpp"
/**
   ���󥹥ȥ饯��
   @param[in] state ̿��¹Գ��ϥ��ơ���
   @param[in] step ̿��¹Գ��ϥ��ƥå�
 */
CDFG_BrElem::CDFG_BrElem
(const std::shared_ptr<CDFG_Node> & prev_state,
 const std::shared_ptr<CDFG_Node> & step_node,
 const unsigned & state,
 const unsigned & step)
  : _prev_state(prev_state),
    _step_node(step_node),
    CDFG_Element(CDFG_Operator::eType::Br,
                 0, state, step)
{} // CDFG_BrElem

/**
   ���Ρ��ɤ�����
   @param[in] tf ���Ρ��ɤλ��� (True or False)
 */
void
CDFG_BrElem::set_condition
(const std::shared_ptr<CDFG_Node> & tf)
{
  this->_tf_node = tf;
} // set_condition

/**
   �������ܽ����Υ�����(Verilog HDL)�μ���
   @param[in] indent ����ǥ��
   @return �������ܽ����Υ�����(Verilog HDL)
   @note input(0) : ̵���ʬ���ڤӾ���դ�ʬ����������1
         input(1) : ����դ�ʬ����ʬ����2
 */
std::string
CDFG_BrElem::other_str
(const std::string & indent)
{
  std::string ret_str("");

  auto out = this->get_output_at(0)->get_verilog_name();

  // ����դ�ʬ���ξ��
  if (this->get_num_input() == 2)
      ret_str.assign
        (indent
         + out + " <= ("
         + this->_get_input_str(this->_tf_node)
         + " ) ? "
         + this->get_input_at(0)->get_verilog_name()
         + " : "
         + this->get_input_at(1)->get_verilog_name()
         + ";\n");
  // ̵���ʬ���ξ��
  else
      ret_str.assign
        (indent
         + out + " <= "
         + this->get_input_at(0)->get_verilog_name()
         + ";\n");

  // prev_state���ѹ�
  ret_str.append
    (indent + this->_prev_state->get_verilog_name()
      + " <= " + out + ";\n");

  // step�ν����
  ret_str.append
    (indent + this->_step_node->get_verilog_name()
     + " <= 0;\n");

  return ret_str;
} // other_str
