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
#include <llvm/IR/Constants.h>
#include <llvm/Support/SourceMgr.h>
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Analysis/CallGraphSCCPass.h>
#include "llvm/IR/Module.h"
#include <cstdlib>
#include <string>

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <set>
#include <sstream>
using namespace llvm;
using namespace std;

///!TODO TO BE COMPLETED BY YOU FOR ASSIGNMENT 2
struct FuncPtrPass:public FunctionPass
{
	static char ID;				// Pass identification, replacement for typeid
	  FuncPtrPass ():FunctionPass (ID)
	{
	}

	std::set < StringRef > fnList;
	std::set < Value * >_fnList;
	std::set < StringRef >::iterator it;

	bool runOnFunction (Function & F) override
	{
		/*errs() << "Hello: ";
		   errs().write_escaped(F.getName()) << '\n'; */
		CallInst *callInst;
		Instruction *inst;
		bool modified = false;

		for (inst_iterator I = inst_begin (F), E = inst_end (F); I != E; ++I)
		{
			if (callInst = dyn_cast < CallInst > (&*I))
			{
				fnList.clear ();
				_fnList.clear ();
				Function *func = callInst->getCalledFunction ();
				if (func == NULL)
				{
					fnPointer (callInst);
					print_vec (callInst->getDebugLoc ().getLine ());
					if (_fnList.size () == 1)
					{
						callInst->setCalledFunction (*_fnList.begin ());
						//errs()<<*callInst<<" getCalledFunction Modified\n";
						modified = true;
					}
				}
				else
				{
					print (callInst->getDebugLoc ().getLine (), func->getName ());
				}
			}
		}

		if (modified)
			return true;
		return false;
	}

	void fnPointer (CallInst * callInst)
	{
		Value *funPtr = callInst->getCalledValue ();
		if (LoadInst * loadInst = dyn_cast < LoadInst > (funPtr))
		{
			_LoadInst (loadInst);
		}
		else if (Argument * agu = dyn_cast < Argument > (funPtr))
		{
			_Argument (agu);
		}
		else if (PHINode * PN = dyn_cast < PHINode > (funPtr))
		{
			_PHINode (PN);
		}
		else if (CallInst * callInstc = dyn_cast < CallInst > (funPtr))
		{
			_CallInst (callInstc);
		}

	}

	void _CallInst (CallInst * callInst)
	{
		Function *func = callInst->getCalledFunction ();
		if (func != NULL)
		{
			for (inst_iterator I = inst_begin (func), E = inst_end (func); I != E; ++I)
			{
				if (ReturnInst * ret = dyn_cast < ReturnInst > (&*I))
				{
					Value *v = ret->getReturnValue ();
					if (Argument * agu = dyn_cast < Argument > (v))
					{
						_Argument (agu);
					}
				}
			}
		}
		else
		{
			Value *funPtr = callInst->getCalledValue ();
			if (PHINode * PN = dyn_cast < PHINode > (funPtr))
			{
			  for (Value * Incoming:PN->incoming_values ())
				{
					if (Function * func = dyn_cast < Function > (Incoming))
					{
						for (inst_iterator I = inst_begin (func), E = inst_end (func); I != E; ++I)
						{
							if (ReturnInst * ret = dyn_cast < ReturnInst > (&*I))
							{
								Value *v = ret->getReturnValue ();
								if (Argument * agu = dyn_cast < Argument > (v))
								{
									_Argument (agu);
								}
							}
						}
					}
				}
			}

		}
	}

	void _PHINode (PHINode * PN)
	{
	  for (Value * Incoming:PN->incoming_values ())
		{
			if (Function * func = dyn_cast < Function > (Incoming))
			{
				fnList.insert (func->getName ());
				_fnList.insert (Incoming);
				/*for(inst_iterator I = inst_begin(func), E = inst_end(func); I!=E; ++I){
				   if(ReturnInst *ret = dyn_cast<ReturnInst>(&*I) ){
				   Value* v = ret->getReturnValue();
				   if(Argument* agu = dyn_cast<Argument>(v)){
				   _Argument(agu);
				   }
				   }
				   } */
			}
			else if (Constant * cons = dyn_cast < Constant > (Incoming))
			{
				if (ConstantPointerNull * CPN = dyn_cast < ConstantPointerNull > (cons))
				{

					fnList.insert (StringRef ("NULL"));
					Function *func;
					_fnList.insert (func);

				}
			}
			else if (PN = dyn_cast < PHINode > (Incoming))
			{
				_PHINode (PN);
			}
			else if (Argument * agu = dyn_cast < Argument > (Incoming))
			{
				_Argument (agu);
			}
		}
	}

	void _Argument (Argument * agu)
	{
		unsigned index = agu->getArgNo ();
		Function *paraP = agu->getParent ();
	  for (User * U:paraP->users ())
		{
			if (CallInst * callInstCall = dyn_cast < CallInst > (U))
			{
				Value *v = callInstCall->getArgOperand (index);
				if (Function * func = dyn_cast < Function > (v))
				{
					fnList.insert (func->getName ());
					_fnList.insert (v);
					//print(callInst->getDebugLoc().getLine(), func->getName());
				}
				else if (PHINode * PN = dyn_cast < PHINode > (v))
				{
					// fnList.clear();
				  for (User * U:PN->users ())
					{
						if (CallInst * callInstCall = dyn_cast < CallInst > (U))
						{
							Value *v = callInstCall->getArgOperand (index);
							if (Function * func = dyn_cast < Function > (v))
							{
								fnList.insert (func->getName ());
								_fnList.insert (v);
							}
						}
					}
					_PHINode (PN);
					//print_vec(callInst->getDebugLoc().getLine());
				}
				else if (LoadInst * loadInst = dyn_cast < LoadInst > (v))
				{
					_LoadInst (loadInst);
					//print_vec(callInst->getDebugLoc().getLine());
				}
				else if (Argument * _agu = dyn_cast < Argument > (v))
				{
					_Argument (_agu);
				}
			}
			else if (PHINode * PN = dyn_cast < PHINode > (U))
			{
			  for (User * U:PN->users ())
				{
					if (CallInst * callInst = dyn_cast < CallInst > (U))
					{
						Value *v = callInst->getOperand (index);
						if (Function * func = dyn_cast < Function > (v))
						{
							fnList.insert (func->getName ());
							_fnList.insert (v);
						}
					}
				}
			}
		}
	}

	void _LoadInst (LoadInst * loadInst)
	{
		LoadInst *_loadInst;
		Value *va = loadInst->getPointerOperand ();;
	  for (User * U:va->users ())
		{
			if (StoreInst * st = dyn_cast < StoreInst > (U))
			{
				Value *vl = st->getOperand (0);
				if (Function * func = dyn_cast < Function > (vl))
				{
					fnList.insert (func->getName ());
					_fnList.insert (vl);
				}
				else if (Argument * agu = dyn_cast < Argument > (vl))
				{
					_Argument (agu);
				}
				else if (loadInst = dyn_cast < LoadInst > (vl))
				{
					_LoadInst (loadInst);
				}
			}
		}
		if (GetElementPtrInst * GEP = dyn_cast < GetElementPtrInst > (va))
		{
			_GetElementPtrInst (GEP);
		}
	}

	void _GetElementPtrInst (GetElementPtrInst * GEP)
	{
		Value *v = GEP->getOperand (0);
	  for (User * U:v->users ())
		{
			if (GetElementPtrInst * GEPL = dyn_cast < GetElementPtrInst > (U))
			{
				if (cmpGEPs (GEP, GEPL) == 1)
				{
				  for (User * UL:GEPL->users ())
					{
						if (StoreInst * storeInst = dyn_cast < StoreInst > (UL))
						{
							Value *vl = storeInst->getOperand (0);
							if (Function * func = dyn_cast < Function > (vl))
							{
								fnList.insert (func->getName ());
								_fnList.insert (vl);
							}
						}
					}
				}
				else
				{

				}
			}
		}
	}

	int cmpGEPs (GetElementPtrInst * GEPL, GetElementPtrInst * GEPR)
	{

		unsigned int ASL = GEPL->getPointerAddressSpace ();
		unsigned int ASR = GEPR->getPointerAddressSpace ();

		if (ASL != ASR)
			return 0;

		Type *ETL = GEPL->getSourceElementType ();
		Type *ETR = GEPR->getSourceElementType ();

		string bufferL;
		raw_string_ostream osL (bufferL);
		osL << *ETL;
		string strETL = osL.str ();

		string bufferR;
		raw_string_ostream osR (bufferR);
		osR << *ETL;
		string strETR = osR.str ();

		if (strETL != strETR)
			return 0;

		unsigned int NPL = GEPL->getNumOperands ();
		unsigned int NPR = GEPR->getNumOperands ();

		if (NPL != NPR)
			return 0;

		for (unsigned i = 0, e = GEPL->getNumOperands (); i != e; ++i)
		{
			Value *vL = GEPL->getOperand (i);
			Value *vR = GEPR->getOperand (i);
			if (cmpValues (vL, vR) == 0)
				return 0;
		}

		return 1;

	}

	int cmpValues (Value * L, Value * R)
	{
		string bufferL;
		raw_string_ostream osL (bufferL);
		osL << *L;
		string strVL = osL.str ();

		string bufferR;
		raw_string_ostream osR (bufferR);
		osR << *R;
		string strVR = osR.str ();

		if (strVL != strVR)
			return 0;

		return 1;
	}

	/* int cmpGEPs(GetElementPtrInst* GepL, GetElementPtrInst* GepR){

	   unsigned int ASL = GepL->getPointerAddressSpace();
	   unsigned int ASR = GepR->getPointerAddressSpace();

	   errs()<<ASL<<" ASL\n";
	   if( ASL != ASR ) return 0;

	   Type* ETL = GepL->getSourceElementType();
	   Type* ETR = GepR->getSourceElementType();
	   errs() << *ETL << "Type\n";

	   unsigned int NPL = GepL->getNumOperands();
	   unsigned int NPR = GepR->getNumOperands();

	   if(NPL != NPR) return 0;

	   for(unsigned i=0, e = GepL->getNumOperands(); i!=e; ++i){
	   Value* vL = GepL->getOperand(i);
	   Value* vR = GepR->getOperand(i);

	   } 

	   return 1;

	   } */

	void print_vec (unsigned line)
	{
		errs () << line << " : ";
		it = fnList.begin ();
		if (it != fnList.end ())
		{
			errs () << *it;
			++it;
		}
		for (; it != fnList.end (); ++it)
		{
			errs () << ", " << *it;
		}
		errs () << "\n";
	}

	void print (unsigned line, StringRef fnName)
	{
		if (fnName.str () == "llvm.dbg.value" || fnName.str () == "llvm.dbg.declare")
			return;
		errs () << line << " : " << fnName << "\n";
	}

};

/*struct FuncPtrPass : public CallGraphSCCPass {

  bool runOnSCC(CallGraphSCC &SCC) override;
    static char ID; // Pass identification, replacement for typeid
    explicit ArgPromotion(unsigned maxElements = 3)
        : CallGraphSCCPass(ID), maxElements(maxElements) {
      initializeArgPromotionPass(*PassRegistry::getPassRegistry());
  }

   /*for(llvm::User::op_iterator i = callInst->arg_begin(), ie = callInst->arg_end(); i!=ie;++i){
              Argument* args = dyn_cast<Argument>(*i);
              errs()<<*args<<"args\n";
            }*/

  /*if(F.getName() == "main"){
     for (inst_iterator I=inst_begin(F), E=inst_end(F); I != E; ++I) {
     if (CallInst* callInst = dyn_cast<CallInst>(&*I)) {
     Value * v = callInst->getCalledValue();
     Value * sv = v->stripPointerCasts();
     StringRef fname = sv->getName();
     errs()<<fname<<"\n";
     }
     }
     } */

  /*for( Value::use_iterator i = inst->use_begin(), ie = inst->use_end(); i != ie; ++i){
     Value* v = *i;
     Instruction* vi = dyn_cast<Instruction>(v);
     errs() << "\t\t" <<  *vi << "\n";

     } */

   /*for( User::op_iterator i = inst->op_begin(), e = inst->op_end(); i != e; ++i ){
      Value *v = *i;
      Instruction *vi = dyn_cast<Instruction>(v);
      errs() << "\t\t" << *vi << "\n";

      } */
/*for( inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I ){
        Instruction* inst = &*I;
        errs() << "def: " << *inst << "\n";
        for (User * U : I->users()){
            if(Instruction* ist = dyn_cast<Instruction>(U)){
              errs() << "\t\t" << *ist << "\n";
            }
        }
        

    }

    for( inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I ){
        Instruction* inst = &*I;
        errs() << "use: " << *inst << "\n";
        for( Use &U : inst->operands() ){
          Value* v = U.get();
          errs() << "\t\t" << *v << "\n";
        }
    }  */

		/*const DebugLoc& Loc = callInst->getDebugLoc();
		   unsigned Line = Loc.getLine(); */

char FuncPtrPass::ID = 0;
static RegisterPass < FuncPtrPass > X ("funcptrpass", "Print function call instruction");

static cl::opt < std::string > InputFilename (cl::Positional, cl::desc ("<filename>.bc"), cl::init (""));

int main (int argc, char **argv)
{
	//LLVMContext &Context = getGlobalContext();
	static LLVMContext Context;
	SMDiagnostic Err;
	// Parse the command line to read the Inputfilename
	cl::ParseCommandLineOptions (argc, argv, "FuncPtrPass \n My first LLVM too which does not do much.\n");

	// Load the input module
	std::unique_ptr < Module > M = parseIRFile (InputFilename, Err, Context);

	//std::unique_ptr<tool_output_file> Out(new tool_output_file(InputFilename,EC,sys::fs::F_None));

	if (!M)
	{
		Err.print (argv[0], errs ());
		return 1;
	}

	llvm::legacy::PassManager Passes;

	///Transform it to SSA
	Passes.add (llvm::createPromoteMemoryToRegisterPass ());

	/// Your pass to print Function and Call Instructions
	Passes.add (new FuncPtrPass ());

	bool modified = Passes.run (*M.get ());
	if (modified)
	{
		string suffix (".modified");
		string inputName (InputFilename.c_str ());
		string outputName = inputName + suffix;

		error_code ec;
		raw_fd_ostream OS (outputName, ec, (sys::fs::OpenFlags) 0);
		M->print (OS, nullptr);

		string ll2bcCmd = "llvm-as " + outputName;
		string rmOutput = "rm " + outputName;
		string mvOutput = "mv " + outputName + ".bc" + " " + inputName;

		system (ll2bcCmd.c_str ());
		system (rmOutput.c_str ());
		system (mvOutput.c_str ());

	}

	// WriteBitcodeToFile(M.get(),Out->os());
	// Out->keep();
}
