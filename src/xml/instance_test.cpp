#include <iostream>
#include <memory>

#include "./CInstancingOperator.hpp"

int main(int argc, char * argv[]) {
  CInstancingOperator cinst;
  auto module_gen = std::make_shared<CModuleGenerator>("output.v", "test_module");
  cinst.instancing_operators(module_gen, "test.xml");
  module_gen->generate();

  return 0;
}
