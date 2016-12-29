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

#include <string>
#include <iostream>
#include <vector>
#include <set>

using namespace llvm;
using namespace std;

// class function_data
// {
// public:
//   llvm::StringRef fun_name;
//   unsigned int numOfParams ;
//   std::vector<llvm::Type::TypeID> params;
// };

///!TODO TO BE COMPLETED BY YOU FOR ASSIGNMENT 2
struct FuncPtrPass : public FunctionPass 
{
  static char ID; // Pass identification, replacement for typeid
  FuncPtrPass() : FunctionPass(ID) {}

  //type str match
  StringRef getTypeString(Type *type)
  {
      StringRef param_type = "";
      switch(type->getTypeID())
      {
      case 0 :
        param_type = "void";
        break;

      case 1:
      case 2:
        param_type = "float";
        break;

      case 3:
      case 4:
      case 5:
      case 6:
        param_type = "double";
        break;

      case 7:
         param_type = "label";
         break;

      case 10:
          if(type->getIntegerBitWidth()==8)
          {
              param_type = "char";
          }
          else
          {
              param_type = "int";
          }
          break;

      case 11:
          param_type = "function";
          break;

      case 12:
          param_type = "struct";
          break;

      case 13:
          param_type = "array";
          break;

      case 14:
          param_type = "pointer";
          break;   

      default :
          param_type = "default";
          break;
     }
     return param_type;
  }
  //todo: arg num match
  // void _GetElementPtrInst(GetElementPtrInst* GEP)
  // {
  //   Value* v = GEP->getOperand(0);
  //   for(User* U : v->users())
  //   {
  //     if(GetElementPtrInst* GEPL = dyn_cast<GetElementPtrInst>(U))
  //     {
  //       if(cmpGEPs(GEP, GEPL)==1)
  //       {
  //         for(User* UL : GEPL->users())
  //         {
  //           if(StoreInst* storeInst = dyn_cast<StoreInst>(UL))
  //           {
  //             Value* vl = storeInst->getOperand(0);
  //             if(Function* func = dyn_cast<Function>(vl))
  //             {
  //               fnList.insert(func->getName());
  //               _fnList.insert(vl);
  //             }
  //           }
  //         }
  //       }
  //       else
  //       {

  //       }
  //     }
  //   }
  // }

  // int cmpGEPs(GetElementPtrInst* GEPL, GetElementPtrInst* GEPR)
  // {
  //   unsigned int ASL = GEPL->getPointerAddressSpace();
  //   unsigned int ASR = GEPR->getPointerAddressSpace();

  //   if( ASL != ASR ) return 0;
  //   Type* ETL = GEPL->getSourceElementType();
  //   Type* ETR = GEPR->getSourceElementType();
  //   string bufferL;
  //   raw_string_ostream osL(bufferL);
  //   osL << *ETL;
  //   string strETL = osL.str();
  //   string bufferR;
  //   raw_string_ostream osR(bufferR);
  //   osR << *ETL;
  //   string strETR = osR.str();

  //   if(strETL != strETR) return 0;

  //   unsigned int NPL = GEPL->getNumOperands();
  //   unsigned int NPR = GEPR->getNumOperands();

  //   if(NPL != NPR) return 0;

  //   for(unsigned i=0, e = GEPL->getNumOperands(); i!=e; ++i)
  //   {
  //     Value* vL = GEPL->getOperand(i);
  //     Value* vR = GEPR->getOperand(i);
  //     if(cmpValues(vL, vR)==0) return 0;
  //   }
  //   return 1;
  // }

  // int cmpValues(Value* L, Value* R)
  // {
  //   string bufferL;
  //   raw_string_ostream osL(bufferL);
  //   osL << *L;
  //   string strVL = osL.str();

  //   string bufferR;
  //   raw_string_ostream osR(bufferR);
  //   osR << *R;
  //   string strVR = osR.str();

  //   if(strVL != strVR) return 0;
  //   return 1;
  // }

  //process return instruction to get the function pointer
  void getRetFuncptr(Function * funcptr,set<Function*> &ret_list)
  {
    //for each basic block in the function that funcptr points to
    Function::iterator bb_it = funcptr->begin() , bb_ie = funcptr->end();
    for(;bb_it != bb_ie ; ++bb_it)
    {
      //for each instruction in the basic block
      BasicBlock::iterator ii = bb_it->begin() , ie = bb_it->end();
      for(;ii != ie ; ++ii)
      {
        ReturnInst *ret_inst = dyn_cast<ReturnInst>(ii);
        if(ret_inst)
        {
          Value *value = ret_inst->getReturnValue();
          //errs()<<value->getName()<<"\n";
          getFunc(value,ret_list);
        }
      }
    }
  }
  //process the function pointer that is passed by argument
  void getArgFuncptr(Argument *arg,set<Function*> &arg_list)
  {
    //get the position of the argument that is a function pointer
    unsigned pos = arg->getArgNo();
    Function *func = arg->getParent();
    auto user_it = func->user_begin() , user_ie = func->user_end();
    Value * v;
    for(; user_it != user_ie ; ++user_it)
    {
      set<CallInst*> call_inst_list;
      User * up = dyn_cast<User>(*user_it);
      getCallInst(up,call_inst_list);
      set<CallInst*>::iterator i = call_inst_list.begin() , e = call_inst_list.end();
      for (; i != e; ++i)
      {
        v = (*i)->getArgOperand(pos);
        getFunc(v,arg_list);
      }
    }
  }
  //process the call instruction by its user
  void  getCallInst(User * user,set<CallInst*> &call_inst_list)
  {
    CallInst *call = dyn_cast<CallInst>(user);
    if(call)
    {
      call_inst_list.insert(call);
    }
    else
    {
      auto user_it = user->user_begin() , user_ie = user->user_end();
      for(; user_it != user_ie ; ++user_it)
      {
        call = dyn_cast<CallInst>(*user_it);
        if(call)
        {
          call_inst_list.insert(call);
        }
        else
        {
          User *up = *user_it;
          //errs()<<up->getName()<<"\n";
          getCallInst(up,call_inst_list);
        }
      }
    }
  }

  //process the function pointer
  void getFunc(Value *value,set<Function*> &call_list)
  {
		Type *type = value->getType();
    //errs()<<*type<<"\n";
		if(type->isPointerTy())
		{
      Function *funcptr = dyn_cast<Function>(value);
      //if function pointer is not null, we get the real function,exit
      if(funcptr)
      {
        call_list.insert(funcptr);
        return;
      }
      //the function pointer may be passed by the arg, get the use of the argument
      else if(Argument *arg = dyn_cast<Argument>(value))
      {
        getArgFuncptr(arg,call_list);
      }
      //the function pointer is a call intruction,recursively process
      else if(CallInst *call = dyn_cast<CallInst>(value))
      {
        set<Function*> possible_call_list;
        getFunc(call->getCalledValue(),possible_call_list);
        set<Function*>::iterator i = possible_call_list.begin() , e = possible_call_list.end();
        //process the return instruction
        for (; i != e; ++i)
        {
          getRetFuncptr(*i,call_list);
        }
      }
      //todo:corner case-bonus,load instruction

      else
      {
        Value::use_iterator use_it=value->use_begin(),use_ie=value->use_end();
        //for each instruction that use the function pointer
        for(;use_it != use_ie; ++use_it)
        {
          Instruction *inst = dyn_cast<Instruction>(*use_it);
          if(inst)
          {
            auto op_it = inst->op_begin(),op_ie=inst->op_end();
            for(;op_it!=op_ie;++op_it)
            {
              //get the function pointer in the instruction
              Function *funcptr = dyn_cast<Function>(op_it->get()); 
              if(funcptr)
              {
                call_list.insert(funcptr);
                //errs()<<funcptr->getName()<<"\n";
              }
              else
              {
                getFunc(op_it->get(),call_list);
              }
            }
          }
        }//end of for

      }//end of else

    }//end of if(type)
  }

  bool runOnFunction(Function &F) override 
  {
    //for each basic block in the function
    Function::iterator bb_it = F.begin() , bb_ie = F.end();
    for(;bb_it != bb_ie ; ++bb_it)
    {
      //for each intruction in the basic block
    	BasicBlock::iterator ii = bb_it->begin() , ie = bb_it->end();
    	for(;ii != ie ; ++ii)
      {
        Instruction *inst=dyn_cast<Instruction>(ii);
        //only process the call instruction to get the possible function call
    		if(isa<CallInst>(ii))
        {
          CallInst *call = dyn_cast<CallInst>(inst);
          //errs()<<*call<<"\n";
    			Function *func = call->getCalledFunction();
    			//FunctionType *type = call->getFunctionType();
          //function pointer
    			if(func==NULL)
          {
    				set<Function *> call_list;
            call_list.clear();
            getFunc(call->getCalledValue(),call_list);
            bool flag = false;
            if(call_list.size() != 0)
            {
              //replace the determined function
              // if(call_list.size()==1)
              // {
              //   set<Function *>::iterator call_it=call_list.begin(),call_ie=call_list.end();
              //   for( ;call_it != call_ie; ++call_it)
              //   {
              //     errs()<<(*call_it)->getName()<<"\n";
              //     call->setCalledFunction(*call_it);
              //     errs()<<*call<<"\n";
              //   }
              // }
              //output the function name from the call_list
              errs()<<call->getDebugLoc()->getLine()<<" : ";
              set<Function *>::iterator call_it=call_list.begin(),call_ie=call_list.end();
              for( ;call_it != call_ie; ++call_it)
              {
                if(!flag)
                {
                  errs()<<(*call_it)->getName();
                }
                else
                {
                  errs()<<", "<<(*call_it)->getName();
                }
                flag=true;
              }
              errs()<<"\n";
            }//end of call_list
    			}//end of func==NULL
          //debug call,e.g. llvm.dbg.value, ignore these info
          else if(func->isIntrinsic())
          {
    				continue;
    			}
          //func is not null, so it is direct function call
          else
          {
            //output the call info directly
            errs()<<call->getDebugLoc().getLine()<<" : "<<func->getName()<<"\n";
    			}

    		}//end of if()

    	}//end of for(ii)

	  }//end of outer loop
    return false;
  }
};


char FuncPtrPass::ID = 0;
static RegisterPass<FuncPtrPass> X("funcptrpass", "Print function call instruction");

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

   /// Your pass to print Function and Call Instructions
   Passes.add(new FuncPtrPass());
   Passes.run(*M.get());

   //write the changed call instruction to file
   std::error_code EC;
   std::unique_ptr<tool_output_file> Out(
    new tool_output_file(InputFilename, EC, sys::fs::F_None));
   WriteBitcodeToFile(M.get(), Out->os());
   Out->keep();

}

