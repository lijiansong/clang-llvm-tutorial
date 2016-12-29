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
#include "llvm/IR/InstIterator.h"

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
struct FuncPtrPass:public FunctionPass
{
	static char ID;				// Pass identification, replacement for typeid
	  FuncPtrPass ():FunctionPass (ID){}

  set < StringRef > call_list;
  set < Value * >_call_list;

	//type str match
	StringRef getTypeString (Type * type)
	{
		StringRef param_type = "";
		switch (type->getTypeID ())
		{
		case 0:
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
			if (type->getIntegerBitWidth () == 8)
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

		default:
			param_type = "default";
			break;
		}
		return param_type;
	}
	//todo: arg num match

	//process the function pointer that is passed by argument
	void getArgFuncptr (Argument * argument)
	{
		unsigned offset = argument->getArgNo ();
		//get its parent caller
		Function *parent = argument->getParent ();
	    for (User * U:parent->users ())
		{
			if (CallInst * callInstCall = dyn_cast < CallInst > (U))
			{
				Value *v = callInstCall->getArgOperand (offset);
				if (Function * func = dyn_cast < Function > (v))
				{
					//errs()<<func->getName ()<<"\n";
					call_list.insert (func->getName ());
					_call_list.insert (v);
				}
				else if (PHINode * phi_node = dyn_cast < PHINode > (v))
				{
				  for (User * U:phi_node->users ())
					{
						if (CallInst * callInstCall = dyn_cast < CallInst > (U))
						{
							Value *v = callInstCall->getArgOperand (offset);
							if (Function * func = dyn_cast < Function > (v))
							{
								call_list.insert (func->getName ());
								_call_list.insert (v);
							}
						}
					}
					getPhiNode (phi_node);
				}
				else if (LoadInst * loadInst = dyn_cast < LoadInst > (v))
				{
					getLoadInst (loadInst);
				}
				else if (Argument * _argument = dyn_cast < Argument > (v))
				{
					getArgFuncptr (_argument);
				}
			}
			else if (PHINode * phi_node = dyn_cast < PHINode > (U))
			{
			  for (User * U:phi_node->users ())
				{
					if (CallInst * call_inst = dyn_cast < CallInst > (U))
					{
						Value *v = call_inst->getOperand (offset);
						if (Function * func = dyn_cast < Function > (v))
						{
							call_list.insert (func->getName ());
							_call_list.insert (v);
						}
					}
				}
			}
		}
	}

	void getCallInst (CallInst * call_inst)
	{
		Function *func = call_inst->getCalledFunction ();
		//funcptr
		if (func != NULL)
		{
			for (inst_iterator inst_it = inst_begin (func), inst_ie = inst_end (func); inst_it != inst_ie; ++inst_it)
			{
				if (ReturnInst * ret = dyn_cast < ReturnInst > (&*inst_it))
				{
					Value *v = ret->getReturnValue ();
					if (Argument * argument = dyn_cast < Argument > (v))
					{
						getArgFuncptr (argument);
					}
				}
			}
		}
		else
		{
			Value *funcptr = call_inst->getCalledValue ();
			if (PHINode * phi_node = dyn_cast < PHINode > (funcptr))
			{
			  for (Value * Incoming:phi_node->incoming_values ())
				{
					if (Function * func = dyn_cast < Function > (Incoming))
					{
						for (inst_iterator inst_it = inst_begin (func), inst_ie = inst_end (func); inst_it != inst_ie; ++inst_it)
						{
							if (ReturnInst * ret = dyn_cast < ReturnInst > (&*inst_it))
							{
								Value *v = ret->getReturnValue ();
								if (Argument * argument = dyn_cast < Argument > (v))
								{
									getArgFuncptr (argument);
								}
							}
						}
					}
				}
			}
		}
	}

	//process the function pointer
	void getFunc (CallInst * callInst)
	{
		Value *funcptr = callInst->getCalledValue ();

		//todo:corner case-bonus,load instruction
		if (LoadInst * load_inst = dyn_cast < LoadInst > (funcptr))
		{
			getLoadInst (load_inst);
		}
		//the function pointer may be passed by the arg, get the use of the argument
		else if (Argument * argument = dyn_cast < Argument > (funcptr))
		{
			getArgFuncptr (argument);
		}
		else if (PHINode * phi_node = dyn_cast < PHINode > (funcptr))
		{
			getPhiNode (phi_node);
		}
		//the function pointer is a call intruction,recursively process
		else if (CallInst * call_inst = dyn_cast < CallInst > (funcptr))
		{
			getCallInst (call_inst);
		}
	}

	void getPhiNode (PHINode * phi_node)
	{
	  for (Value * Incoming:phi_node->incoming_values ())
		{
			if (Function * func = dyn_cast < Function > (Incoming))
			{
				//errs()<<func->getName()<<"\n";
				call_list.insert (func->getName ());
				_call_list.insert (Incoming);
			}
			//the phi node may contains null
			else if (Constant * cons = dyn_cast < Constant > (Incoming))
			{
				if (ConstantPointerNull * CPN = dyn_cast < ConstantPointerNull > (cons))
				{

					call_list.insert (StringRef ("NULL"));
					Function *func;
					_call_list.insert (func);
				}
			}
			else if (phi_node = dyn_cast < PHINode > (Incoming))
			{
				getPhiNode (phi_node);
			}
			else if (Argument * argument = dyn_cast < Argument > (Incoming))
			{
				getArgFuncptr (argument);
			}
		}
	}

	//corner case-bonus,load instruction
	void getLoadInst (LoadInst * load_inst)
	{
		Value *v = load_inst->getPointerOperand ();
	    for (User * U:v->users ())
		{
			if (StoreInst *st = dyn_cast < StoreInst > (U))
			{
				Value *value = st->getOperand (0);
				if (Function * func = dyn_cast < Function > (value))
				{
					call_list.insert (func->getName ());
					_call_list.insert (value);
				}
				else if (Argument *argument = dyn_cast < Argument > (value))
				{
					getArgFuncptr (argument);
				}
				else if (load_inst = dyn_cast < LoadInst > (value))
				{
					getLoadInst (load_inst);
				}
			}
		}
		if (GetElementPtrInst * getElementPtrInst = dyn_cast < GetElementPtrInst > (v))
		{
			getGetElementPtrInst (getElementPtrInst);
		}
	}

	void getGetElementPtrInst (GetElementPtrInst * getElementPtrInst)
	{
		Value *v = getElementPtrInst->getOperand (0);
	    for (User * U:v->users ())
		{
			if (GetElementPtrInst * gepi = dyn_cast < GetElementPtrInst > (U))
			{
				if (cmpGetElementPtrInst (getElementPtrInst, gepi) == true)
				{
				  for (User * UL:gepi->users ())
					{
						if (StoreInst * storeInst = dyn_cast < StoreInst > (UL))
						{
							Value *vl = storeInst->getOperand (0);
							if (Function * func = dyn_cast < Function > (vl))
							{
								call_list.insert (func->getName ());
								_call_list.insert (vl);
							}
						}
					}
				}
				
			}
		}
	}

	bool cmpGetElementPtrInst (GetElementPtrInst * gep_left, GetElementPtrInst * gep_right)
	{
		unsigned int ASL = gep_left->getPointerAddressSpace ();
		unsigned int ASR = gep_right->getPointerAddressSpace ();
		if (ASL != ASR)
			return false;

		Type *ETL = gep_left->getSourceElementType ();
		Type *ETR = gep_right->getSourceElementType ();

		string bufferL;
		raw_string_ostream osL (bufferL);
		osL << *ETL;
		string strETL = osL.str ();

		string bufferR;
		raw_string_ostream osR (bufferR);
		osR << *ETL;
		string strETR = osR.str ();

		if (strETL != strETR)
			return false;

		unsigned int NPL = gep_left->getNumOperands ();
		unsigned int NPR = gep_right->getNumOperands ();

		if (NPL != NPR)
			return false;

		for (unsigned i = 0, e = gep_left->getNumOperands (); i != e; ++i)
		{
			Value *vL = gep_left->getOperand (i);
			Value *vR = gep_right->getOperand (i);
			if (cmpValue (vL, vR) == false)
				return false;
		}
		return true;
	}

	bool cmpValue (Value * L, Value * R)
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
			return false;

		return true;
	}

	//show the result
	void display (unsigned line)
	{
		errs () << line << " : ";
		auto it = call_list.begin ();
		auto ie =call_list.end();
		if (it != ie)
		{
			errs () << *it;
			++it;
		}
		for (; it != ie; ++it)
		{
			errs () << ", " << *it;
		}
		errs () << "\n";
	}

	bool runOnFunction (Function & F) override
	{
		bool updated = false;
		//for each basic block in the function
		Function::iterator bb_it = F.begin (), bb_ie = F.end ();
		for (; bb_it != bb_ie; ++bb_it)
		{
			//for each intruction in the basic block
			BasicBlock::iterator ii = bb_it->begin (), ie = bb_it->end ();
			for (; ii != ie; ++ii)
			{
				Instruction *inst = dyn_cast < Instruction > (ii);
				//only process the call instruction to get the possible function call
				if (isa < CallInst > (ii))
				{
					CallInst *call = dyn_cast < CallInst > (inst);
					//errs()<<*call<<"\n";
					Function *func = call->getCalledFunction ();
					//FunctionType *type = call->getFunctionType();
					//function pointer
					if (func == NULL)
					{
						//process the funcptr
						getFunc (call);
						display (call->getDebugLoc ().getLine ());
						if (_call_list.size () == 1)
						{
							call->setCalledFunction (*(_call_list.begin ()));
							//errs()<<*callInst<<" getCalledFunction Modified\n";
							updated = true;
						}
					}//end of func==NULL
					//debug call,e.g. llvm.dbg.value, ignore these info
					else if (func->isIntrinsic ())
					{
						continue;
					}
					//func is not null, so it is direct function call
					else
					{
						//output the call info directly
						errs () << call->getDebugLoc ().getLine () << " : " << func->getName () << "\n";
					}

				}//end of if()

			}//end of for(ii)

		}//end of outer loop
		if (updated)
			return true;
		return false;
	}
};

char FuncPtrPass::ID = 0;
static RegisterPass < FuncPtrPass > X ("funcptrpass", "Print function call instruction");

static cl::opt < std::string > InputFilename (cl::Positional, cl::desc ("<filename>.bc"), cl::init (""));

int main (int argc, char **argv)
{
	LLVMContext & Context = getGlobalContext ();
	//static LLVMContext Context;
	SMDiagnostic Err;
	// Parse the command line to read the Inputfilename
	cl::ParseCommandLineOptions (argc, argv, "FuncPtrPass \n My first LLVM too which does not do much.\n");

	// Load the input module
	std::unique_ptr < Module > M = parseIRFile (InputFilename, Err, Context);
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
	bool updated = Passes.run (*M.get ());

	//write the changed call instruction to file
	if(updated)
	{
		//errs()<<"changed\n";
		std::error_code EC;
		std::unique_ptr<tool_output_file> Out(new tool_output_file(InputFilename, EC, sys::fs::F_None));
		WriteBitcodeToFile(M.get(), Out->os());
		Out->keep();
	}
	
}
