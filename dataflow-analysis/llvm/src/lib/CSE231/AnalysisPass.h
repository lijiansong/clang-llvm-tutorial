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
#include "Lattice.h"

#include <stdio.h>

using namespace llvm;
using namespace std;

    template<class T,class U>
	struct AnalysisPass : public FunctionPass {
	
        //template<>
        static char ID; // Pass identification, replacement for typeid
		vector<BasicBlock*> blockOrder; // for use in remembering order of traversed blocks

		map<StringRef, vector<T *> > blockInstAnalysis; //for use propagating dataflow facts

		AnalysisPass<T,U>() : FunctionPass(ID) {}

		virtual bool runOnFunction(Function &F) {
            map<StringRef, BasicBlock*> visitedBlocks; //for use in detecting loop
			errs() << "BEGIN FUNCTION: ";
			errs().write_escaped(F.getName()) << "\n";
			for (Function::iterator B = F.begin(); B != F.end(); ++B) { 
				errs() << "Begin block with name: ";
				errs().write_escaped(B->getName()) << "\n";
				
				// check predecessors to perform merge
				vector< Lattice<U>* > predecessorEdges;
				errs() << "\tPredecessors:\n";
				for (pred_iterator PI = pred_begin(B); PI != pred_end(B); ++PI) {
  					BasicBlock *pred = *PI;
					errs() << "\t\t";
					errs().write_escaped(pred->getName());
					errs() << "\n";
					// get final instruction's outgoing edge from each predecessor block
					if (!blockInstAnalysis[pred->getName()].empty()) {// some predecessors have not been visited (a loop edge from a future block)
						Lattice<U>* singleEdge = blockInstAnalysis[pred->getName()].back()->getOutgoingEdge( &(*B) );
						predecessorEdges.push_back(singleEdge);
						T::dump(singleEdge);
						
					}
					else {
						errs() << "\t\t\tNo incoming edge from this, pushing bottom\n";
						U empty;
						predecessorEdges.push_back(new Lattice<U>(false,true,empty)); //mem leak here
					}
				}
				// perform mergings	
				// if no predecessors: incomingEdge will be bottom
				U empty;		
				Lattice<U> * incomingEdge = new Lattice<U>(false,true,empty);
				if (predecessorEdges.size() == 1)
					incomingEdge = predecessorEdges.front();
                else if (predecessorEdges.size() >= 2) {
                    incomingEdge = T::merge(predecessorEdges[0], predecessorEdges[1]);
                    for (unsigned int i = 2; i < predecessorEdges.size(); i++) {
                        incomingEdge = T::merge(incomingEdge, predecessorEdges[2]);
                    }
                }

				// record block visit
				visitedBlocks[B->getName()] = &(*B);
				blockOrder.push_back(&(*B));
				
				// collect dataflow facts
				errs() << "\tInstructions:\n";
				for (BasicBlock::iterator I = B->begin(); I != B->end(); ++I) { 
					errs() << "\t\t";
                    T * analysis = new T(&(*I), incomingEdge);
					blockInstAnalysis[B->getName()].push_back(analysis);
					analysis->applyFlowFunction();
                   	I->dump();
					analysis->dump();
					incomingEdge = analysis->getOutgoingEdge(); // for next analysis
				}

				// check successors for presence of a loop
				errs() << "\tSucessors:\n";
				for (succ_iterator SI = succ_begin(B); SI != succ_end(B); ++SI) {
					BasicBlock *successor = *SI;
					errs() << "\t\t";
					errs().write_escaped(successor->getName());
					BasicBlock* blockIfVisited = visitedBlocks[successor->getName()];
					if (blockIfVisited) {
						errs() << " !!!VISITED BEFORE!!!\n";
						blockLoop(blockIfVisited, &(*B)); // handle loop
					}
					else
						errs() << "\n";
				}
			}
			return false; // NOTE WE WILL HAVE TO CHANGE THIS
		}

		void blockLoop(BasicBlock *start, BasicBlock *end) {
			unsigned int startIndex;
			unsigned int endIndex;
			for (unsigned int i = 0; i < blockOrder.size(); i++) {
				if (blockOrder[i] == start) 
					startIndex = i;
				if (blockOrder[i] == end)
					endIndex = i;
			}
			map<StringRef, BasicBlock*> visitedBlocks; //for use in detecting nested loop
			while (true) {
				for (unsigned int i = startIndex; i <= endIndex; i++) {
					BasicBlock * currentBlock = blockOrder[i];
					errs() << "Begin !!!LOOP!!! block with name: ";
					errs().write_escaped(currentBlock->getName()) << "\n";
					// check predecessors to perform merge
					vector< Lattice<U> * > predecessorEdges;
					errs() << "\tPredecessors:\n";
					for (pred_iterator PI = pred_begin(currentBlock); PI != pred_end(currentBlock); ++PI) {
						BasicBlock *pred = *PI;
						errs() << "\t\t";
						errs().write_escaped(pred->getName());
						errs() << "\n";
						// get final instruction's outgoing edge from each predecessor block
						if (!blockInstAnalysis[pred->getName()].empty())  {// some predecessors have not been visited (a loop edge from a future block)
							Lattice<U>* singleEdge = blockInstAnalysis[pred->getName()].back()->getOutgoingEdge( currentBlock );
							predecessorEdges.push_back(singleEdge);
							T::dump(singleEdge);
						}
						else {
							errs() << "\t\t\tNo incoming edge from this, pushing bottom\n";
							U empty;
							predecessorEdges.push_back(new Lattice<U>(false,true,empty)); //mem leak here
						}
					}
					// perform mergings			
					Lattice<U> * incomingEdge;
					if (predecessorEdges.size() == 1)
						incomingEdge = predecessorEdges.front();
					else if (predecessorEdges.size() >= 2) {
						incomingEdge = T::merge(predecessorEdges[0], predecessorEdges[1]);
						for (unsigned int i = 2; i < predecessorEdges.size(); i++) {
							incomingEdge = T::merge(incomingEdge, predecessorEdges[2]);
						}
					}
					// record block visit
					visitedBlocks[currentBlock->getName()] = currentBlock;
					// collect dataflow facts
					errs() << "\tInstructions:\n";
					for (unsigned int j = 0; j < blockInstAnalysis[currentBlock->getName()].size(); j++) { 
						errs() << "\t\t";
						T * analysis = blockInstAnalysis[currentBlock->getName()][j];
						Lattice<U> * originalOut = analysis->getOutgoingEdge();
						analysis->setIncomingEdge(incomingEdge);
						analysis->applyFlowFunction();
						analysis->getInstruction()->dump();
						analysis->dump();
						if(T::equal(originalOut, analysis->getOutgoingEdge())) {
							errs() << "Encountered fixed dataflow fact --- !!!ENDING LOOP!!!\n";
							return; //we are done
						}
						incomingEdge = analysis->getOutgoingEdge(); // for next analysis
					}

					// check successors for presence of a loop
					errs() << "\tSucessors:\n";
					for (succ_iterator SI = succ_begin(currentBlock); SI != succ_end(currentBlock); ++SI) {
						BasicBlock *successor = *SI;
						errs() << "\t\t";
						errs().write_escaped(successor->getName());
						BasicBlock* blockIfVisited = visitedBlocks[successor->getName()];
						if (blockIfVisited) {
							errs() << " !!!VISITED BEFORE!!!\n";
							blockLoop(blockIfVisited, currentBlock); // handle loop
						}
						else
							errs() << "\n";
					}
				}
				visitedBlocks.clear(); //IMPORTANT, otherwise detect nested loops on subsequent loops
			}

		}
	};

