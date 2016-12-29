//===- test.cpp - 				    ---------------===//
//
// Sample pass to understand basic block behavior
// 
// 		
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "AvailableExpr"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/User.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Format.h"

#include <llvm/ADT/StringRef.h>
#include "llvm/Support/CFG.h"
#include "AnalysisPass.h"
#include "AvailableExprAnalysis.h"
#include "ConstantPropAnalysis.h"
#include "RangeAnalysis.h"
#include "PointerAnalysis.h"

#include <stdio.h>

using namespace llvm;
using namespace std;

template<> char AnalysisPass<AvailableExprAnalysis,ExpressionContainer>::ID = 0;
template<> char AnalysisPass<ConstantPropAnalysis,map<string,ConstantInt*> >::ID = 1;
template<> char AnalysisPass<RangeAnalysis,map<string,vector<int> > >::ID = 2;
template<> char AnalysisPass<MustPointerAnalysis,map<string,set<Value*,valueComp> > >::ID = 3;
template<> char AnalysisPass<MayPointerAnalysis,map<string,set<Value*,valueComp> > >::ID = 4;

static RegisterPass<AnalysisPass<AvailableExprAnalysis,ExpressionContainer> > X("avail_expr", "AvailableExpr Pass");
static RegisterPass<AnalysisPass<ConstantPropAnalysis,map<string,ConstantInt*> > > Y("block_order","Block Order Pass");
static RegisterPass<AnalysisPass<RangeAnalysis,map<string,vector<int> > > > Z("RangePass","RangePass");
static RegisterPass<AnalysisPass<MustPointerAnalysis,map<string,set<Value*,valueComp> > > > A("must_pointer","Must Pointer Pass");
static RegisterPass<AnalysisPass<MayPointerAnalysis,map<string,set<Value*,valueComp> > > > B("may_pointer","May Pointer Pass");

