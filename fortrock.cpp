#include <sstream>
#include <iostream>
#include <list>
#include <llvm/Pass.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/InstIterator.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FormattedStream.h>

using namespace llvm;

class FortRock : public FunctionPass  {
private:
  formatted_raw_ostream &Out;

public:
  static char ID;
  explicit FortRock(formatted_raw_ostream &o) : FunctionPass(ID), Out(o){}

  const char *getPassName() const { return "FortRock: Fortran to Verilog"; }

  void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
  }

  virtual bool doInitialization(Module &M);

  bool runOnFunction(Function &F) {
    errs() << "Function Name: " << F.getName().str() << "\n";

    std::list<Instruction*> insts;

    inst_iterator inst_it = inst_begin(F);
    inst_iterator inst_it_end = inst_end(F);
    for(;inst_it != inst_it_end; ++inst_it)
      insts.push_back(&*inst_it);

    Instruction *inst = NULL;
    while(!insts.empty()) {
      inst = insts.front();
      insts.pop_front();
    }

    return false;
  }

  bool doFinalization(Module &M) {
  }
};

char FortRock::ID = 0;
RegisterPass<FortRock> X("fortrock", "Fortran to Verilog");

// ==================================================
// External Interface declaration
// ==================================================
