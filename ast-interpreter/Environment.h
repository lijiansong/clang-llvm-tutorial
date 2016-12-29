//==--- tools/clang-check/ClangInterpreter.cpp - Clang Interpreter tool --------------===//
//===----------------------------------------------------------------------===//
#include <stdio.h>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include <iostream>
#include <string>

using namespace clang;
using namespace std;
#define INF 0x7fffffffffffffff

/// Heap maps address to a value,
//the address in the memory is unsigned long, 
//here we uniformly take it as long type for the sake of simplicity
//and compatible with int type
class Heap {
   /// The allocated bufs, key is the address, val is its size
   std::map<long, long> mBufs;
   /// The map that maps address to value
   std::map<long, long> mContents;
public:
	Heap() : mBufs(), mContents(){
   }

   //allocate a buffer with the size of size and return the start pointer of the buffer
   long Malloc(int size) {
      //assert (mBufs.find(addr) == mHeap.end());
      /// Allocate the buf
      int * buf = (int *)malloc(size * sizeof(int) );
      mBufs.insert(std::make_pair((long)buf, size));

      /// Initialize the Content
      for (int i=0; i<size; i ++) {
         mContents.insert(std::make_pair((long)(buf+i), 0));
      }
      return (long)buf;
   }

   //release the buffer
   void Free (long addr) {
      /// Clear the contents;
      //assert (mHeap.find(addr) != mHeap.end());
   	  assert (mBufs.find(addr) != mBufs.end());
      int * buf = (int *)addr;
      long size = mBufs.find(addr)->second;
      mBufs.erase(mBufs.find(addr));

      for (int i = 0; i < size; i++) {
      	assert (mContents.find((long)(buf+i)) != mContents.end());
        mContents.erase((long)(buf+i));
      }
        /// Free the allocated buf
      //free(mHeap.find(addr)->second);
      free(buf);
   }

   //update the value of address in the buf
   void Update(long addr, long val) {
      assert (mContents.find(addr) != mContents.end());
      mContents[addr] = val;
   }

   //get the value of addr in the buf
   long Get(long addr) {
      assert (mContents.find(addr) != mContents.end());
      return mContents.find(addr)->second;
    }
};

class StackFrame {
   /// StackFrame maps Variable Declaration to Value
   /// Which are either integer or addresses (also represented using an Integer value)
   //to be compatible with the addr,we change the type from int to long
   std::map<Decl*, long> mVars;
   std::map<Stmt*, long> mExprs;
   //Heap * mHeap;
   /// The current stmt
   Stmt * mPC;
public:
   StackFrame() : mVars(), mExprs(), /*mHeap(),*/ mPC() {
   }

   // void bindHeapStmt(Stmt * stmt,int size)
   // {
   // 	  long addr=mHeap->Malloc(size);
   // 	  mExprs[stmt]=addr;
   // }
   // void freeHeap(long addr)
   // {
   // 	mHeap->Free(addr);
   // }
   // void setHeap()
   // {

   // }
   // long getHeapStmt(Stmt *stmt)
   // {
   // 	  assert (mExprs.find(stmt) != mExprs.end());
	  // return mExprs[stmt];
   // }

   //bind the value of Decl
   void bindDecl(Decl* decl, int val) {
      mVars[decl] = val;
   }

   //get the value of Decl
   long getDeclVal(Decl * decl) {
      assert (mVars.find(decl) != mVars.end());
      return mVars.find(decl)->second;
   }

   //bind the value of Stmt
   void bindStmt(Stmt * stmt, int val) {
	   mExprs[stmt] = val;
   }

   //get the value of Stmt
   long getStmtVal(Stmt * stmt) {
	   assert (mExprs.find(stmt) != mExprs.end());
	   return mExprs[stmt];
   }

   //set current instruction
   void setPC(Stmt * stmt) {
	   mPC = stmt;
   }

   //get current instruction
   Stmt * getPC() {
	   return mPC;
   }
};

class Environment {
   //mStack to store current StackFrame
   std::vector<StackFrame> mStack;
   //mHeap to process the mem allocation
   Heap mHeap;
   std::vector<StackFrame> mVarGlobal;

   /// Declartions to the built-in functions
   FunctionDecl * mFree;
   FunctionDecl * mMalloc;
   FunctionDecl * mInput;
   FunctionDecl * mOutput;

   //fuction process entry,here is main()
   FunctionDecl * mEntry;
public:
   /// Get the declartions to the built-in functions
   Environment() : mStack(), mVarGlobal(),mFree(NULL), mMalloc(NULL), mInput(NULL), mOutput(NULL), mEntry(NULL) {
   }


   /// Initialize the Environment
   void init(TranslationUnitDecl * unit) 
   {
	   for (TranslationUnitDecl::decl_iterator i =unit->decls_begin(), e = unit->decls_end(); i != e; ++ i) 
     {
		   if (FunctionDecl * fdecl = dyn_cast<FunctionDecl>(*i) ) 
       {
			   if (fdecl->getName().equals("FREE")) mFree = fdecl;
			   else if (fdecl->getName().equals("MALLOC")) mMalloc = fdecl;
			   else if (fdecl->getName().equals("GET")) mInput = fdecl;
			   else if (fdecl->getName().equals("PRINT")) mOutput = fdecl;
			   else if (fdecl->getName().equals("main")) mEntry = fdecl;
		   }
       //process global varible
       if(VarDecl *vardecl=dyn_cast<VarDecl>(*i))
       {
          if(!( vardecl->hasInit()))
          {
              StackFrame stack;
              stack.bindDecl(vardecl, 0);
              mVarGlobal.push_back(stack);
              //mVarGlobal.back().bindDecl(vardecl,0);
              //mStack.back().bindDecl(vardecl,0);
          }
          else if( vardecl->hasInit() )
          {
              //now only process global varible with integer
              //int val=mStack.back().getStmtVal(vardecl->getInit());
              if(isa<IntegerLiteral>(vardecl->getInit()))
              {
                  StackFrame stack;
                  IntegerLiteral *integer=dyn_cast<IntegerLiteral>(vardecl->getInit());
                  int val=integer->getValue().getSExtValue();
                  stack.bindDecl(vardecl, val);
                  mVarGlobal.push_back(stack);
                  //mStack.push_back(stack);
              }
          }
       }

	   }
	   mStack.push_back(StackFrame());
   }

   //get the entry
   FunctionDecl * getEntry() {
	   return mEntry;
   }

   //process BinaryOperator
   void binop(BinaryOperator *bop) {
   	   //get the right and left expr of BinaryOperator
	   auto * left = bop->getLHS();
	   auto * right = bop->getRHS();

	   //get the value of right and left expr
	   int valLeft=mStack.back().getStmtVal(left);
	   int valRight=mStack.back().getStmtVal(right);

	   //assignment operation
	   if (bop->isAssignmentOp()) {
	   	//process pointer type, e.g. *dptr = ptr;
	   	if(isa<UnaryOperator>(left))
	   	{
	   		UnaryOperator *uop = dyn_cast<UnaryOperator>(left);
	   		//left opcode is UO_Deref, which is *
	   		if( (uop->getOpcode()) == UO_Deref)
	   		{
	   			Expr *expr = uop->getSubExpr();
                int addr = mStack.back().getStmtVal(expr);
                //update the value of addr in buf
                mHeap.Update(addr, valRight);
	   		}
	   	}
	   	//process the array type, e.g. int [n]
	   	if(isa<ArraySubscriptExpr>(left))
	   	{
	   		ArraySubscriptExpr *array=dyn_cast<ArraySubscriptExpr>(left);
	   		Expr *leftexpr=array->getLHS();
	   		//get the base of the array
   	        long base=mStack.back().getStmtVal(leftexpr);
   	        Expr *rightexpr=array->getRHS();
   	        //get the offset index of the array, here is an integerliteral
   	        long offset=mStack.back().getStmtVal(rightexpr);
   	        mHeap.Update(base + offset*sizeof(int), valRight);
	   		//mStack.back().bindStmt(left)
	   	}

		   //int val = mStack.back().getStmtVal(right);
	   	   //bind the right value to the left expr
		   mStack.back().bindStmt(left, valRight);
		   //cout<<left->getStmtClassName()<<endl;

		   //if left expr is a refered expr, bind the right value to it
		   if (DeclRefExpr * declexpr = dyn_cast<DeclRefExpr>(left)) {
			   Decl * decl = declexpr->getFoundDecl();
			   mStack.back().bindDecl(decl, valRight);
		   }
	   }
	   
	   //comparison operation
	   if(bop->isComparisonOp())
	   {
	   	switch(bop->getOpcode())
	   	{
	   		//<
	   		case BO_LT:
	   		if( valLeft < valRight )
	   			mStack.back().bindStmt(bop,true);
	   		else
	   			mStack.back().bindStmt(bop,false);
	   		break;

	   		//>
	   		case BO_GT:
	   		if( valLeft > valRight )
	   			mStack.back().bindStmt(bop,true);
	   		else
	   			mStack.back().bindStmt(bop,false);
	   		break;

	   		//>=
	   		case BO_GE:
	   		if( valLeft >= valRight )
	   			mStack.back().bindStmt(bop,true);
	   		else
	   			mStack.back().bindStmt(bop,false);
	   		break;

	   		//<=
	   		case BO_LE:
	   		if( valLeft <= valRight )
	   			mStack.back().bindStmt(bop,true);
	   		else
	   			mStack.back().bindStmt(bop,false);
	   		break;

	   		//==
	   		case BO_EQ:
	   		if( valLeft == valRight )
	   			mStack.back().bindStmt(bop,true);
	   		else
	   			mStack.back().bindStmt(bop,false);
	   		break;

	   		//!=
	   		case BO_NE:
	   		if( valLeft != valRight )
	   			mStack.back().bindStmt(bop,true);
	   		else
	   			mStack.back().bindStmt(bop,false);
	   		break;
	   		default:
	   		cout<<" invalid input comparisons! "<<endl;
	   		break;
	   	}
	   }

	   //Additive operation,e.g. add and sub
	   if(bop->isAdditiveOp())
	   {
	   	switch(bop->getOpcode())
	   	{
	   		//+
	   		case BO_Add:
	   		mStack.back().bindStmt(bop,valLeft+valRight);
	   		break;

	   		//-
	   		case BO_Sub:
	   		mStack.back().bindStmt(bop,valLeft-valRight);
	   		break;
	   	}
	   }

	   //Multiplicative operation, e.g. * , /
	   if(bop->isMultiplicativeOp())
	   {
	   	switch(bop->getOpcode())
	   	{
	   		//*
	   		case BO_Mul:
	   		mStack.back().bindStmt(bop,valLeft * valRight);
	   		break;
	   	}
	   }
   }

   //process UnaryOperator, e.g. +,-,* and etc.
   void unaryop(UnaryOperator* uop)
   {
   	//get the sub-expr of UnaryOperator
   	Expr *expr=uop->getSubExpr();
   	//get the value of the sub-expr
   	int val=mStack.back().getStmtVal(expr);
	switch(uop->getOpcode())
	{
		//+,plus
		case UO_Plus:
		mStack.back().bindStmt(uop,val);
		break;

		//-,minus
		case UO_Minus:
		mStack.back().bindStmt(uop,-val);
		break;

		//*,pointer 
		case UO_Deref:
		//int *val=(int *)mStack.back().getStmtVal(expr);
		mStack.back().bindStmt(uop, mHeap.Get(val));
		break;

		//&,deref,bind the address of expr to UnaryOperator
		case UO_AddrOf: mStack.back().bindStmt(uop,(long)expr);
		cout<<long(expr)<<endl;
		//mStack.back().bindStmt(uop, mHeap.Get(val));
		break;

		//other operator
		// case UO_PostInc:   break;
  //       case UO_PostDec:   break;
  //       case UO_PreInc:    break;
  //       case UO_PreDec:    break;
  //       case UO_AddrOf:    break;
  //       case UO_Plus:      break;
  //       case UO_Minus:     break;
  //       case UO_Not:       break;
  //       case UO_LNot:      break;
  //       case UO_Real:      break;
  //       case UO_Imag:      break;
  //       case UO_Extension: break;
  //       case UO_Coawait:   break;
	}
   }

   //process ArraySubscriptExpr, e.g. int [n]
   void array(ArraySubscriptExpr *arrayexpr)
   {
   	//get the base and the offset index of the array
   	Expr *leftexpr=arrayexpr->getLHS();
   	//cout<<leftexpr->getStmtClassName()<<endl;
   	int base=mStack.back().getStmtVal(leftexpr);
   	Expr *rightexpr=arrayexpr->getRHS();
   	//cout<<rightexpr->getStmtClassName()<<endl;
   	int offset=mStack.back().getStmtVal(rightexpr);
   	//cout<<valRight<<endl;

   	//by mHeap,we can get the value of addr in buf,we bind the value to ArraySubscriptExpr
   	mStack.back().bindStmt(arrayexpr,mHeap.Get(base + offset*sizeof(int)));
   }

   //process IntegerLiteral, e.g. integer number node in ast
   //we can simply get the value and bind it to IntegerLiteral
   void integerliteral(IntegerLiteral* integer)
   {
   	//int val=integer->getValue().bitsToDouble();
   	int val=integer->getValue().getSExtValue();
   	mStack.back().bindStmt(integer,val);
   }

   //get the condition value of IfStmt and WhileStmt
   bool getcond(/*BinaryOperator *bop*/Expr *expr)
   {
   	return mStack.back().getStmtVal(expr);
   }

   //process DeclStmt, e.g. int a; int a=c+d;
   void decl(DeclStmt * declstmt) 
   {
   	   //we may declare many variables in one stmt, e.g. int a,b,c,d
	   for (DeclStmt::decl_iterator it = declstmt->decl_begin(), ie = declstmt->decl_end(); it != ie; ++ it) 
	   {
		Decl * decl = *it;
		//in ast, the sub-node is usually VarDecl
		if (VarDecl * vardecl = dyn_cast<VarDecl>(decl)) 
		{
			//string type=(vardecl->getType()).getAsString();
			// if VarDecl doesn't have init expr, e.g. int a,b;
			if( !( vardecl->hasInit() ) /*&& (type.compare("int *")) && (type.compare("int **"))*/)
			 {
			 	//whether VarDecl is an array type
			 	if( !( vardecl->getType()->isArrayType() ) )
			 		mStack.back().bindDecl(vardecl, 0);
			 	else
			 	{
			 		//get the type string, e.g. int [2]
			 		string type=(vardecl->getType()).getAsString();
			 		//we use the naive method to get size of the array by string match
			 		//this method may be unsafe and unreusable
			 		int size=0;
			 		int indexLeft=type.find("[");
			 		int indexRight=type.find("]");
			 		if((indexLeft!=string::npos) && (indexRight!=string::npos))
			 		{
			 			string num=type.substr(indexLeft+1,indexRight-indexLeft-1);
    					size=atoi(num.c_str());
    					//cout<<num<<endl<<size<<endl;
			 		}
			 		
			 		//for the array type, when we declear it we will allocate buf for it meanwhile
			 		long buf=mHeap.Malloc(size);
			 		//bind the start pointer of the buf to VarDecl
			 		mStack.back().bindDecl(vardecl, buf);
			 	}
			 }
			//pointer type,e.g. int *, int **
			 // else if(!( vardecl->hasInit() ) && ( !(type.compare("int *")) || !(type.compare("int **")) ))
			 // {
			 // 	mStack.back().bindDecl(vardecl, INF);
			 // }

			 //if VarDecl has init expr, e.g. int a=b+1; and etc.
			 //we can get the value of the init expr and bind it to VarDecl
			 else if( vardecl->hasInit() )
			 {
			 	int val=mStack.back().getStmtVal(vardecl->getInit());
		    	mStack.back().bindDecl(vardecl, val);
			 }
	    	}
	    }
   }

   //process DeclRefExpr
   void declref(DeclRefExpr * declref) 
   {
	   mStack.back().setPC(declref);
	   //make sure DeclRefExpr has an initial value
	   mStack.back().bindStmt(declref, 0);

	   //if DeclRefExpr is Integer type
	   if (declref->getType()->isIntegerType()) 
	   {
		   Decl* decl = declref->getFoundDecl();
		   int val = mStack.back().getDeclVal(decl);
		   mStack.back().bindStmt(declref, val);
	   }
	   //if DeclRefExpr is pointer type
	   else if(declref->getType()->isPointerType()) 
	   {
           Decl* decl = declref->getFoundDecl();
           int val = mStack.back().getDeclVal(decl);
           mStack.back().bindStmt(declref, val);
       }
       //if DeclRefExpr is array type
       else if(declref->getType()->isArrayType())
       {
       	   Decl* decl = declref->getFoundDecl();
           int val = mStack.back().getDeclVal(decl);
           mStack.back().bindStmt(declref, val);
       }
   }

   //process CastExpr
   void cast(CastExpr * castexpr) 
   {
	   mStack.back().setPC(castexpr);
	   Expr * expr = castexpr->getSubExpr();
	   if (castexpr->getType()->isIntegerType()) 
	   {
		   
		   int val = mStack.back().getStmtVal(expr);
		   //cout<<"------CastExpr expr val: "<<val<<" getSubExpr expr:"<<expr->getStmtClassName()<<endl;
		   mStack.back().bindStmt(castexpr, val );
	   }
	   else
	   {
         int val = mStack.back().getStmtVal(expr);
         mStack.back().bindStmt(castexpr, val );
       }
   }

   //process UnaryExprOrTypeTraitExpr, e.g. sizeof
   void unarysizeof(UnaryExprOrTypeTraitExpr *uop)
   {
   	// auto *expr=uop->getArgumentExpr();
   	// int val =sizeof(expr);
   	  int val;
   	  //if UnaryExprOrTypeTraitExpr is sizeof,
      if(uop->getKind() == UETT_SizeOf )
      {
      	 //if the arg type is integer type, we bind sizeof(long) to UnaryExprOrTypeTraitExpr
         if(uop->getArgumentType()->isIntegerType())
         {
            val = sizeof(long);
         }
         //if the arg type is pointer type, we bind sizeof(int *) to UnaryExprOrTypeTraitExpr
         else if(uop->getArgumentType()->isPointerType())
         {
            val = sizeof(int *);
         }
      }    
   	  mStack.back().bindStmt(uop,val);
   }

   //process return stmt
   //when a function call take place, we push the current environment to a stack,
   //so to process the return stmt, we ought to pop it out
   void ret(ReturnStmt *retstmt)
   {
   	//get the return expr and its value,and bind it to call function
   	Expr *expr=retstmt->getRetValue();
   	//cout<<expr->getStmtClassName()<<endl;
   	int val=0;
   	val=mStack.back().getStmtVal(expr);
   	mStack.back().bindStmt(retstmt,val);

   	//pop back from the stack to get the instruction before the function call
   	mStack.pop_back();
   	Stmt *stmt=mStack.back().getPC();
   	//cout<<stmt->getStmtClassName()<<endl;
   	//CallExpr * callexpr = dyn_cast<CallExpr>(stmt);
   	mStack.back().bindStmt(stmt,val);
   }

   /// process Function Call, e.g. built-in function and other self-define functions
   void call(CallExpr * callexpr) 
   {
	   mStack.back().setPC(callexpr);
	   int val = 0;
	   //get the direct function 
	   FunctionDecl * callee = callexpr->getDirectCallee();

	   //built-in function GET
	   if (callee == mInput) 
	   {
		  llvm::errs() << "Please Input an Integer Value : ";
		  scanf("%d", &val);

		  mStack.back().bindStmt(callexpr, val);
	   }
	   //built-in function PRINT
	   else if (callee == mOutput) 
	   {
		   Expr * decl = callexpr->getArg(0);
		   val = mStack.back().getStmtVal(decl);
		   llvm::errs() << val;
	   }
	   //built-in function Malloc
	   else if ( callee == mMalloc )
	   {
	   	//child_iterator
	   	// auto param=callexpr->child_end();
	   	// //Expr * param = callexpr->getArg(0);
	   	// if(isa<UnaryExprOrTypeTraitExpr>(*param))
	   	// {
   		// 	int size =(int)(mStack.back().getStmtVal(*param));
   		// 	mStack.back().bindHeapStmt(callexpr,size);
	   	// }

	   	 //get the sizeof expr and its value, and allocate relevant buf in the memory,
	   	 //bind the start address of the buf to CallExpr
	   	 Expr * unaryExprOrTypeTraitExpr = callexpr->getArg(0);
         val = mStack.back().getStmtVal(unaryExprOrTypeTraitExpr);
         long buf = mHeap.Malloc(val);
         mStack.back().bindStmt(callexpr, buf);
	   }
	   //built-in function Free
	   else if (callee == mFree )
	   {
	   	// auto param=callexpr->child_end();
	   	// long addr=mStack.back().getStmtVal(*param);
	   	// mStack.back().freeHeap(addr);

	   	 //get the addr of the buf, using Free method of mHeap to release the buf
	   	 Expr * unaryExprOrTypeTraitExpr = callexpr->getArg(0);
         val = mStack.back().getStmtVal(unaryExprOrTypeTraitExpr);
         mHeap.Free(val);
         mStack.back().bindStmt(callexpr, 0);
	   }
	   //self-define function
	   else 
	   {
		   /// You could add your code here for Function call Return
		   //llvm::errs() << callee->getName();
	   	//Expr **args=callexpr.getArgs();
	   	//bind the args
	   	// vector<Expr*> args;
	   	// for(CallExpr::arg_iterator it=callexpr->arg_begin(), ie=callexpr->arg_end();it!=ie;++it)
	   	// {
	   	// 	args.push_back(*it);
	   	// }
	   	// int argc=callee->getNumParams();
	   	// for(int i=0;i<argc;++i)
	   	// {
	   	// 	int val=mStack.front().getStmtVal(args[i]);
	   	// 	mStack.front().bindDecl(callee->parameters()[i], val);
	   	// }
	   	// mStack.front().setPC(callee->getBody());

	   	//mStack.front().bindDecl(callee,getret())
	   	//mStack.front().bindStmt(callexpr,10+val);

	   	//preserve current instruction to the stack,
	   	//when the fucntion call take place, we bind the param value to the self-define function
	   	StackFrame stack;
	   	auto pit=callee->param_begin();
	   	for(auto it=callexpr->arg_begin(), ie=callexpr->arg_end();it!=ie;++it,++pit)
	   	{
	   		int val=mStack.back().getStmtVal(*it);
	   		stack.bindDecl(*pit,val);
	   	}
	   	mStack.push_back(stack);
	   }
   }

};
