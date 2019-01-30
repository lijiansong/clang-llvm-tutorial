#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "profiling-branch-bias"

using namespace llvm;
using namespace std;

namespace {
struct ProfilingBranchBias : public FunctionPass {
  static char ID;
  ProfilingBranchBias() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    Module *mod = F.getParent();
    LLVMContext &context = mod->getContext();

    // void updateBranchInfo(bool taken);
    Function *udt = cast<Function>(
        mod->getOrInsertFunction("updateBranchInfo", Type::getVoidTy(context),
                                 Type::getInt1Ty(context)));
    // void printOutBranchInfo();
    Function *prt = cast<Function>(mod->getOrInsertFunction(
        "printOutBranchInfo", Type::getVoidTy(context)));

    for (Function::iterator B = F.begin(), BE = F.end(); B != BE; ++B) {

      bool flag = false;
      for (BasicBlock::iterator I = B->begin(), IE = B->end(); I != IE; ++I) {
        // BranchInst* branch = dyn_cast<BranchInst>(&*I);
        if ((*I).getOpcode() == 2 && (*I).getNumOperands() > 1) {
          vector<Value *> args;
          args.push_back((*I).getOperand(0));
          IRBuilder<> Builder(&*I);
          Builder.CreateCall(udt, args);
        }

        if ((string)I->getOpcodeName() == "ret")
          flag = true;
      }

      if (flag) {
        BasicBlock::iterator I = B->end();
        I--;
        IRBuilder<> Builder2(&*I);
        Builder2.CreateCall(prt);
      }
    }
    return false;
  }
}; // end of struct TestPass
} // end of anonymous namespace

char ProfilingBranchBias::ID = 0;
static RegisterPass<ProfilingBranchBias> X(DEBUG_TYPE, "Profiling Branch Bias",
                                           false /* Only looks at CFG */,
                                           false /* Analysis Pass */);

static cl::opt<std::string>
    InputFilename(cl::Positional, cl::desc("<filename>.bc"), cl::init(""));

#if LLVM_VERSION_MAJOR >= 4
static ManagedStatic<LLVMContext> GlobalContext;
static LLVMContext &getGlobalContext() { return *GlobalContext; }
#endif

int main(int argc, const char *argv[]) {
  LLVMContext &Context = getGlobalContext();
  // static LLVMContext Context;
  SMDiagnostic Err;
  // Parse the command line to read the Inputfilename
  cl::ParseCommandLineOptions(argc, argv, "LLVM profiling branch bias...\n");

  // load the input module
  std::unique_ptr<Module> M = parseIRFile(InputFilename, Err, Context);
  if (!M) {
    Err.print(argv[0], errs());
    return -1;
  }

  llvm::legacy::PassManager Passes;

  /// Transform it to SSA
  // Passes.add(llvm::createPromoteMemoryToRegisterPass());
  // Passes.add(new LoopInfoWrapperPass());

  Passes.add(new ProfilingBranchBias());
  Passes.run(*M.get());

  // Write back the instrumentation info into LLVM IR
  std::error_code EC;
  std::unique_ptr<tool_output_file> Out(
      new tool_output_file(InputFilename, EC, sys::fs::F_None));
  WriteBitcodeToFile(M.get(), Out->os());
  Out->keep();

  return 0;
}
