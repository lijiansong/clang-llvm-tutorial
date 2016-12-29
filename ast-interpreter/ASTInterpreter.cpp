//==--- tools/clang-check/ClangInterpreter.cpp - Clang Interpreter tool --------------===//
//===----------------------------------------------------------------------===//

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/EvaluatedExprVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;

#include "Environment.h"

class InterpreterVisitor : 
   public EvaluatedExprVisitor<InterpreterVisitor> {
public:
   explicit InterpreterVisitor(const ASTContext &context, Environment * env)
   : EvaluatedExprVisitor(context), mEnv(env) {}
   virtual ~InterpreterVisitor() {}

   //process BinaryOperator,e.g. assignment, add and etc.
   virtual void VisitBinaryOperator (BinaryOperator * bop) {
     //visit the sub-node of BinaryOperator
	   VisitStmt(bop);
	   mEnv->binop(bop);
   }

   //process ArraySubscriptExpr, e.g. int [2]
   virtual void VisitArraySubscriptExpr(ArraySubscriptExpr *arrayexpr)
   {
    VisitStmt(arrayexpr);
    mEnv->array(arrayexpr);
   }

   //process DeclRefExpr, e.g. refered decl expr
   virtual void VisitDeclRefExpr(DeclRefExpr * expr) {
	   VisitStmt(expr);
	   mEnv->declref(expr);
   }

   //process CastExpr
   virtual void VisitCastExpr(CastExpr * expr) {
	   VisitStmt(expr);
	   mEnv->cast(expr);
   }

   //process CallExpr,e.g. function call
   virtual void VisitCallExpr(CallExpr * call) {

     //process the sub-node of CallExpr first
	   VisitStmt(call);
	   mEnv->call(call);
     //get direct function call
     FunctionDecl * callee = call->getDirectCallee();
     Stmt *body=callee->getBody();
     if(body && isa<CompoundStmt>(body) )
     {
          //visit the function body
          VisitStmt(body);
     }
   }

  //process DeclStmt,e.g. int a; int a=c+d; and etc.
  virtual void VisitDeclStmt(DeclStmt * declstmt) {
     VisitStmt(declstmt);
	   mEnv->decl(declstmt);
   }
   
   //process IfStmt
   virtual void VisitIfStmt(IfStmt *ifstmt) {
      //get the condition expr and visit relevant node in ast
      Expr *expr=ifstmt->getCond();
      Visit(expr);
      //cout<<expr->getStmtClassName()<<endl;
      //BinaryOperator * bop = dyn_cast<BinaryOperator>(expr);
      //get the bool value of condition expr
      bool cond=mEnv->getcond(expr);
      //if condition value is true, visit then block,else visit else block
      if(cond)
      {
        VisitStmt(ifstmt->getThen());
      }
      else
      {
        Stmt *else_block=ifstmt->getElse();
        //if else block really exists
        if(else_block)
          VisitStmt(else_block);
      }
   }
   
   //process WhileStmt
   virtual void VisitWhileStmt(WhileStmt *whilestmt) {
      //get the condition expr of WhileStmt in ast,and visit relevant node
      Expr *expr = whilestmt->getCond();
      Visit(expr);
      //BinaryOperator *bop = dyn_cast<BinaryOperator>(expr);
      //get the condition value of WhileStmt,if it is true, visit the body of WhileStmt
      bool cond=mEnv->getcond(expr);
      Stmt *body=whilestmt->getBody();
      while(cond)
      {
        if( body && isa<CompoundStmt>(body) )
        {
          VisitStmt(whilestmt->getBody());
        }
        //update the condition value
        Visit(expr);
        cond=mEnv->getcond(expr);
      }
   }

   //process UnaryOperator, e.g. -, * and etc.
   virtual void VisitUnaryOperator(UnaryOperator *uop)
   {
        VisitStmt(uop);
        mEnv->unaryop(uop);
   }

   //process UnaryExprOrTypeTraitExpr, e.g. sizeof and etc.
   virtual void VisitUnaryExprOrTypeTraitExpr(UnaryExprOrTypeTraitExpr *uop)
   {
            VisitStmt(uop);
            mEnv->unarysizeof(uop);
   }

   //process ReturnStmt
   virtual void VisitReturnStmt(ReturnStmt *retstmt) {
           VisitStmt(retstmt);
           //Visit(retstmt);
           mEnv->ret(retstmt);
   }

   // virtual void VisitParmVarDecl(ParmVarDecl *parm)
   // {
   //         VisitDecl(parm);
   //         mEnv->parmdecl(parm);
   // }

   // virtual void VisitFunctionDecl(FunctionDecl *func) {
   //         // VisitDecl(func);
   //         // mEnv->funcdecl(func);
   // }

   //process IntegerLiteral, e.g. 1,13 and etc
   virtual void VisitIntegerLiteral(IntegerLiteral *integer)
   {
      //VisitStmt(integer);
      mEnv->integerliteral(integer);
   }
   
private:
   Environment * mEnv;
};

class InterpreterConsumer : public ASTConsumer {
public:
   explicit InterpreterConsumer(const ASTContext& context) : mEnv(),
   	   mVisitor(context, &mEnv) {
   }
   virtual ~InterpreterConsumer() {}

   virtual void HandleTranslationUnit(clang::ASTContext &Context) {
	   TranslationUnitDecl * decl = Context.getTranslationUnitDecl();
	   mEnv.init(decl);

	   FunctionDecl * entry = mEnv.getEntry();
	   mVisitor.VisitStmt(entry->getBody());
  }
private:
   Environment mEnv;
   InterpreterVisitor mVisitor;
};

class InterpreterClassAction : public ASTFrontendAction {
public: 
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    return std::unique_ptr<clang::ASTConsumer>(
        new InterpreterConsumer(Compiler.getASTContext()));
  }
};
//static cl::opt<std::string> FileName(cl::Positional ,cl::desc("Input file"),cl::Required);

int main (int argc, char ** argv) {
   if (argc > 1) {
       clang::tooling::runToolOnCode(new InterpreterClassAction, argv[1]);
   }
   
   /*cl::ParseCommandLineOptions(argc, argv, "cmd line \n");
   
   new InterpreterClassAction;
   return 0;*/
}
