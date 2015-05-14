#ifndef _CDFG_OPERATOR_H
#define _CDFG_OPERATOR_H

#include <vector>
#include "CDFG_Port.hpp"

/**
   @class CDFG_Operator
   @brief 演算器を定義するクラス
*/
class CDFG_Operator {
public:
  CDFG_Operator(const unsigned latency)
    : _latency(latency) {}
  ~CDFG_Operator(void) {}

  /** ゲッター */
  inline unsigned get_num_input(void) const { return this->_input_list.size(); }
  inline unsigned get_num_output(void) const { return this->_output_list.size(); }
  inline unsigned get_latency(void) const { return this->_latency; }
  inline CDFG_Port get_input_at(const unsigned & at) const { return this->_input_list.at(at); }
  inline CDFG_Port get_output_at(const unsigned & at) const { return this->_output_list.at(at); }

  /* セッター */
  inline void add_input_port(const CDFG_Port & port) const { this->_input_list.push_back(port); }
  inline void add_output_port(const CDFG_Port & port) const { this->_output_list.push_back(port); }
  inline void set_latency(const unsigned latency) const {this->_latency = latency; }

private:
  std::vector<CDFG_Port> _input_list;
  std::vector<CDFG_Port> _output_list;
  unsigned _latency;
};

#endif
