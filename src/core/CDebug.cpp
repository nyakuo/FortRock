#include "CDebug.hpp"

/**
   モジュールに含まれる全Nodeの情報を標準出力に出力
 */
void
CDebug::output_node_info
(const std::list<std::shared_ptr<CDFG_Node> > & dfg) {
  std::cout << "-------- node info --------"
            << std::endl;

  for (auto & node : dfg) {
    std::cout << "asm_name: "
              << node->get_asm_name()
              << std::endl
              << "safe_name: "
              << node->get_safe_name()
              << std::endl
              << "verilog name: "
              << node->get_verilog_name()
              << std::endl
              << "bit width: "
              << node->get_bit_width()
              << std::endl
              << "is signed: "
              << node->get_is_signed()
              << std::endl
              << "get type: "
              << (unsigned)node->get_type()
              << std::endl
              << "is input: "
              << node->is_input()
              << std::endl
              << "is output: "
              << node->is_output()
              << "\n" << std::endl;
  }
}
