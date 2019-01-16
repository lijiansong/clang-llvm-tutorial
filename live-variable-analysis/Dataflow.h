/************************************************************************
 *
 * @file Dataflow.h
 *
 * General dataflow framework
 *
 ***********************************************************************/

#ifndef _DATAFLOW_H_
#define _DATAFLOW_H_

#include <llvm/ADT/BitVector.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include <list>

//#define DEBUG

using namespace llvm;

using Worklist = std::list<BasicBlock *>;

// Template parameter:
// - FVT = type of data flow value. e.g. it can be a BitVector
template <typename FVT> class DataFlow {
  // direction: true forward, false backward
  bool forward;

public:
  enum SetType { IN, OUT };

  DataFlow(bool forward) : forward(forward) {
    in = new InOutBBFVMap();
    out = new InOutBBFVMap();
    neighbourSpecificValues = new InOutBBFVMap();
    visited = new ValueMap<BasicBlock *, bool>();
  }

  ~DataFlow() {
    delete in;
    delete out;
    delete neighbourSpecificValues;
    delete visited;
  }

  // type alias for IN and OUT flow value sets of a basic block
  using InOutBBFVMap = ValueMap<const BasicBlock *, FVT *>;
  // in and out sets for a basic block
  InOutBBFVMap *in;
  InOutBBFVMap *out;
  InOutBBFVMap *neighbourSpecificValues;
  ValueMap<BasicBlock *, bool> *visited;

  // forward analysis
  void performForwardAnalysis(Worklist &w) {
    BasicBlock *curBlock = *w.begin();
    w.pop_front();
    (*visited)[curBlock] = true;

#ifdef DEBUG
    errs() << "===> Enter Baisc Block: " << curBlock->getName() << '\n';
#endif

    int numPred = 0;
    pred_iterator PIT = pred_begin(curBlock), PIE = pred_end(curBlock);
    for (; PIT != PIE; ++PIT) {
      ++numPred;
      if (PIT == pred_begin(curBlock)) {
        *(*in)[curBlock] = *(*out)[*PIT];
      } else {
        meetOp((*in)[curBlock], (*out)[*PIT]);
      }
    }

    if (numPred == 0) {
      setBoundaryCondition((*in)[curBlock]);
    }

    FVT *newOut = transferFunc(*curBlock);
    bool changed = false;
    changed = (*newOut != *(*out)[curBlock]);

    if (changed) {
      *(*out)[curBlock] = *newOut;
    }

    succ_iterator SIT = succ_begin(curBlock), SIE = succ_end(curBlock);
    for (; SIT != SIE; ++SIT) {
      if (changed || !(*visited)[*SIT]) {
        w.push_back(*SIT);
      }
    }
  }

  // backward analysis
  void performBackwardAnalysis(Worklist &w) {
    BasicBlock *curBlock = *w.begin();
    w.pop_front();
    (*visited)[curBlock] = true;

#ifdef DEBUG
    errs() << "===> Enter Basic Block: " << curBlock->getName() << '\n';
#endif

    // to check for the exit node
    int numSucc = 0;

    // OUT of this basic block is equivalent to IN of its successor
    succ_iterator SIT = succ_begin(curBlock), SIE = succ_end(curBlock);
    for (; SIT != SIE; ++SIT) {
      numSucc++;
      if (SIT == succ_begin(curBlock)) {
        // copy the first IN set values
        *(*out)[curBlock] = *(*in)[*SIT];
      } else {
        // call the meet operator
        meetOp((*out)[curBlock], (*in)[*SIT]);
      }
    }

    if ((*neighbourSpecificValues).find(curBlock) !=
        (*neighbourSpecificValues).end()) {
      // for phi node. meet the variables that are live from this specific block
      meetOp((*out)[curBlock], (*neighbourSpecificValues)[curBlock]);
    }

    // set boundary condition for the exit node.
    if (numSucc == 0) {
      setBoundaryCondition((*out)[curBlock]);
    }

    FVT *newIn = transferFunc(*curBlock);

    bool changed = false;
    changed = ((*newIn) != (*(*in)[curBlock]));

    if (changed)
      *(*in)[curBlock] = *newIn;
    pred_iterator PIT = pred_begin(curBlock), PIE = pred_end(curBlock);
    for (; PIT != PIE; ++PIT) {
      if (changed || !(*visited)[*PIT]) {
        w.push_back(*PIT);
      }
    }
  }

  void finalizeBackwardAnalysis(Function &func) {
    for (Function::iterator i = func.begin(), e = func.end(); i != e; ++i) {
      // int numSucc = 0;
      BasicBlock *curBlock = &*i;
      if ((*neighbourSpecificValues).find(curBlock) !=
          (*neighbourSpecificValues).end()) {
        for (succ_iterator SI = succ_begin(&*i), SE = succ_end(&*i); SI != SE;
             SI++) {
          meetOp((*in)[*SI], (*neighbourSpecificValues)[curBlock]);
        }
      }
    }
  }

  // add just the last block in case of backward analysis
  void initializeWorklist(Function &func, Worklist &worklist) {
    if (forward) {
      BasicBlock &entry = func.getEntryBlock();
      worklist.push_back(&entry);
      return;
    }

    Function::iterator BBIT = func.begin(), BBIE = func.end();
    for (; BBIT != BBIE; ++BBIT) {
      int numSucc = 0;
      succ_iterator SIT = succ_begin(&*BBIT), SIE = succ_end(&*BBIT);
      for (; SIT != SIE; SIT++) {
        numSucc++;
      }

      if (numSucc == 0) {
        worklist.push_back(&*BBIT);
      }
    }
  }

  bool runOnFunction(Function &F) {
    bool changed = false;
    visited = new ValueMap<BasicBlock *, bool>();
    Function::iterator BBIT = F.begin(), BBIE = F.end();
    for (; BBIT != BBIE; ++BBIT) {
      BasicBlock *B = &*BBIT;

      (*in)[B] = initFlowValue(*BBIT, IN);
      (*out)[B] = initFlowValue(*BBIT, OUT);
    }

    Worklist *worklist = new Worklist();
    initializeWorklist(F, *worklist);
    while (!worklist->empty()) {
      if (forward)
        performForwardAnalysis(*worklist);
      else
        performBackwardAnalysis(*worklist);
    }
    finalizeBackwardAnalysis(F);
    return changed;
  }

protected:
  virtual void setBoundaryCondition(FVT *) = 0;

  virtual void meetOp(FVT *lhs, const FVT *rhs) = 0;

  virtual FVT *initFlowValue(BasicBlock &b, SetType setType) = 0;

  virtual FVT *transferFunc(BasicBlock &b) = 0;
};

#endif /* !_DATAFLOW_H_ */
