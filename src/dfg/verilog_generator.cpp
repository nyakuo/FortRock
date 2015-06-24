/**
   @file verilog_generator.cpp
   @brief テストプログラム
          命令列のDFG化に関するクラスの実装
          およびVerilogコードの出力機能に関する検証
*/

#include "CModuleGenerator.hpp"

int main(int argc, char **argv) {
  CModuleGenerator generator("output.v", "test_module");
  generator.generate();

  return 0;
}
