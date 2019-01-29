#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;

#define DEBUG_TYPE "count-dynamic-insts"

namespace {
struct CountDynamicInstructions : public FunctionPass {
  static char ID;
  CountDynamicInstructions() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {

    // void updateInstrInfo(unsigned num, uint32_t * keys, uint32_t * values);
    Function *udt = cast<Function>(F.getParent()->getOrInsertFunction(
        "updateInstrInfo", Type::getVoidTy(F.getParent()->getContext()),
        Type::getInt32Ty(F.getParent()->getContext()),
        Type::getInt32PtrTy(F.getParent()->getContext()),
        Type::getInt32PtrTy(F.getParent()->getContext())));

    // void printOutInstrInfo();
    Function *prt = cast<Function>(F.getParent()->getOrInsertFunction(
        "printOutInstrInfo", Type::getVoidTy(F.getParent()->getContext())));

    for (Function::iterator B = F.begin(), BE = F.end(); B != BE; ++B) {
      std::map<int, int> cdi;
      bool flag = false;
      for (BasicBlock::iterator I = B->begin(), IE = B->end(); I != IE; ++I) {
        ++cdi[I->getOpcode()];
        if ((std::string)I->getOpcodeName() == "ret") {
          flag = true;
        }
      }
      int count = cdi.size();
      std::vector<Constant *> k;
      std::vector<Constant *> v;
      for (auto &pair : cdi) {
        k.push_back(ConstantInt::get(
            Type::getInt32Ty(F.getParent()->getContext()), pair.first));
        v.push_back(ConstantInt::get(
            Type::getInt32Ty(F.getParent()->getContext()), pair.second));
      }

      GlobalVariable *KG = new GlobalVariable(
          *(F.getParent()),
          ArrayType::get(Type::getInt32Ty(F.getParent()->getContext()), count),
          true, GlobalValue::InternalLinkage,
          ConstantArray::get(
              ArrayType::get(Type::getInt32Ty(F.getParent()->getContext()),
                             count),
              k),
          "keys global");

      GlobalVariable *VG = new GlobalVariable(
          *(F.getParent()),
          ArrayType::get(Type::getInt32Ty(F.getParent()->getContext()), count),
          true, GlobalValue::InternalLinkage,
          ConstantArray::get(
              ArrayType::get(Type::getInt32Ty(F.getParent()->getContext()),
                             count),
              v),
          "values global");

      std::vector<Value *> args;
      args.push_back(ConstantInt::get(
          Type::getInt32Ty(F.getParent()->getContext()), count));

      IRBuilder<> Builder(&*B);
      Builder.SetInsertPoint(B->getTerminator());

      args.push_back(Builder.CreatePointerCast(
          KG, Type::getInt32PtrTy(F.getParent()->getContext())));
      args.push_back(Builder.CreatePointerCast(
          VG, Type::getInt32PtrTy(F.getParent()->getContext())));

      // instrument updateInstrInfo
      Builder.CreateCall(udt, args);

      if (flag) {
        BasicBlock::iterator I = B->end();
        I--;
        IRBuilder<> Builder2(&*I);
        // instrument printOutInstrInfo
        Builder2.CreateCall(prt);
      }
    }

    return false;
  }
}; // end of struct CountDynamicInstructions
} // end of anonymous namespace

char CountDynamicInstructions::ID = 0;
static RegisterPass<CountDynamicInstructions>
    X(DEBUG_TYPE, "Collecting Dynamic Instruction Counts",
      false /* Only looks at CFG */, false /* Analysis Pass */);

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
  cl::ParseCommandLineOptions(argc, argv, "Live variable analysis...\n");

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

  Passes.add(new CountDynamicInstructions());
  Passes.run(*M.get());

  // Write back the instrumentation info into LLVM IR
  std::error_code EC;
  std::unique_ptr<tool_output_file> Out(
      new tool_output_file(InputFilename, EC, sys::fs::F_None));
  WriteBitcodeToFile(M.get(), Out->os());
  Out->keep();

  return 0;
}
