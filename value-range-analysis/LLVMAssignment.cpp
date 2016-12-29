//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include <llvm/Support/CommandLine.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Transforms/Scalar.h>

#include "Liveness.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;


///!TODO TO BE COMPLETED BY YOU FOR ASSIGNMENT 2
struct FuncPtrPass : public FunctionPass 
{
  static char ID; // Pass identification, replacement for typeid
  FuncPtrPass() : FunctionPass(ID) {}

  
  bool runOnFunction(Function &F) override 
  {
    // errs() << "Hello: ";
    // errs().write_escaped(F.getName()) << '\n';
    return false;
  }
};


char FuncPtrPass::ID = 0;
static RegisterPass<FuncPtrPass> X("funcptrpass", "Print function call instruction");

char Liveness::ID = 0;
static RegisterPass<Liveness> Y("liveness", "Liveness Dataflow Analysis");

static cl::opt<std::string>
InputFilename(cl::Positional,
              cl::desc("<filename>.bc"),
              cl::init(""));


int main(int argc, char **argv) 
{
   //LLVMContext &Context = getGlobalContext();
   static LLVMContext Context;
   SMDiagnostic Err;
   // Parse the command line to read the Inputfilename
   cl::ParseCommandLineOptions(argc, argv,
                              "FuncPtrPass \n My first LLVM too which does not do much.\n");


   // Load the input module
   std::unique_ptr<Module> M = parseIRFile(InputFilename, Err, Context);
   if (!M) 
   {
      Err.print(argv[0], errs());
      return 1;
   }

   llvm::legacy::PassManager Passes;

   ///Transform it to SSA
   Passes.add(llvm::createPromoteMemoryToRegisterPass());
   Passes.add(new LoopInfoWrapperPass());
   /// Your pass to print Function and Call Instructions
   Passes.add(new Liveness());
   Passes.run(*M.get());
}

