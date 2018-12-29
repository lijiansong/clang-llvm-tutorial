#include <llvm/Bitcode/ReaderWriter.h>
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

#include <map>
#include <string>
using namespace llvm;

namespace {
    struct CountStaticInstructions : public FunctionPass {
        static char ID;
        CountStaticInstructions() : FunctionPass(ID) {}
        bool runOnFunction(Function &F) override {
            std::map<std::string, int> instMap;
            Function::iterator func_it = F.begin();
            Function::iterator func_ie = F.end();
            for(; func_it!=func_ie; ++func_it) {
                BasicBlock::iterator bb_it = func_it->begin();
                BasicBlock::iterator bb_ie = func_it->end();
                for(; bb_it!=bb_ie; ++bb_it) {
                    instMap[bb_it->getOpcodeName()]++;
                }
            }
            auto it = instMap.begin();
            auto ie = instMap.end();
            for(; it!=ie; ++it) {
                errs() << it->first << "\t" << instMap[it->first] << "\n";
            }
            return false;
        }
    };
}

char CountStaticInstructions::ID = 0;
static RegisterPass<CountStaticInstructions> X("count-static-insts", "count static instructions");
static cl::opt < std::string > InputFilename (cl::Positional, cl::desc ("<filename>.bc"), cl::init (""));

int main(int argc, const char *argv[]) {
    //LLVMContext &Context = getGlobalContext();
    static LLVMContext Context;
    SMDiagnostic Err;
    // Parse the command line to read the Inputfilename
    cl::ParseCommandLineOptions(argc, argv, "count static insts");
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
    Passes.add (new CountStaticInstructions());
    //bool updated = Passes.run (*M.get());
    Passes.run (*M.get());
    return 0;
}

