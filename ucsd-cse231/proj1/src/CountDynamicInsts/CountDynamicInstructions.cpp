//#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Transforms/Scalar.h>
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/User.h"
#include "llvm/IR/Use.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/Metadata.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/TypeBuilder.h"

#include <map>
#include <string>
#include <vector>
using namespace llvm;

namespace {
    struct CountDynamicInstructions : public FunctionPass {
        static char ID;
        CountDynamicInstructions() : FunctionPass(ID) {}
        bool runOnFunction(Function &F) override {
            Module *mod = F.getParent();
            // Look up the specified function in the module symbol table
            Constant *analysisFuncConst = mod->getOrInsertFunction("updateSingleInst",
                    Type::getVoidTy(F.getContext()),
                    IntegerType::getInt32Ty(F.getContext()),
                    NULL);
            Function *analysisFuncConstant = cast<Function>(analysisFuncConst);
            Function::iterator func_it = F.begin();
            Function::iterator func_ie = F.end();
            for(; func_it!=func_ie; ++func_it) {
                BasicBlock::iterator bb_it = func_it->begin();
                BasicBlock::iterator bb_ie = func_it->end();
                for(; bb_it!=bb_ie; ++bb_it) {
                    IRBuilder<> Builder(&*(bb_it));
                    Constant *numVal = ConstantInt::get(Type::getInt32Ty(F.getContext()), bb_it->getOpcode());
                    Value *args[] = {numVal};
                    Builder.CreateCall(analysisFuncConstant, args);
                    bb_it->dump();
                    numVal->dump();
                }
            }

            Function::iterator f_it = F.end();
            --f_it;
            BasicBlock::iterator bb_it = f_it->end();
            --bb_it;

            Constant *infoPrintConstant = mod->getOrInsertFunction("printOutInstInfo",
                    Type::getVoidTy(F.getContext()),
                    NULL);
            Function *infoPrintFunc = cast<Function>(infoPrintConstant);
            Instruction *inst = &*(bb_it);
            inst->dump();
            IRBuilder<> Builder(inst);
            Builder.CreateCall(infoPrintFunc);
            return false;
        }
    };
}

char CountDynamicInstructions::ID = 0;
static RegisterPass<CountDynamicInstructions> X("count-dynamic-insts", "count dynmatic instructions", false, false);
static cl::opt < std::string > InputFilename (cl::Positional, cl::desc ("<filename>.bc"), cl::init (""));

int main(int argc, const char *argv[]) {
    //LLVMContext &Context = getGlobalContext();
    static LLVMContext Context;
    SMDiagnostic Err;
    // Parse the command line to read the Inputfilename
    cl::ParseCommandLineOptions(argc, argv, "count dynamic insts");
    // Load the input module
    std::unique_ptr<Module> M = parseIRFile(InputFilename, Err, Context);
    if(!M) {
        Err.print(argv[0], errs());
        return -1;
    }
    llvm::legacy::PassManager Passes;
    ///Transform it to SSA
    //Passes.add (llvm::createPromoteMemoryToRegisterPass());
    /// Your pass to print Function and Call Instructions
    Passes.add (new CountDynamicInstructions());
    //bool updated = Passes.run (*M.get());
    Passes.run (*M.get());
    return 0;
}

