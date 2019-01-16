/************************************************************************
 *
 * @file LiveVariable.cpp
 *
 * Live variable analysis in backward flow direction.
 *
 * Liveness analysis data flow equations:
 *  out[B] = U{S \in succ[B]} in[S]
 *  in[B] = use[B] U (out[B] - def[B])
 *
 * A BitVector is stored at each basic block for the in and out sets.
 * It contains an entry for all the values
 * Corner case: empty set for flow value, identified
 * by no successors.
 *
 ***********************************************************************/

#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/IR/DebugInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/SourceMgr.h>

#include "Dataflow.h"

using namespace llvm;

#if LLVM_VERSION_MAJOR >= 4
static ManagedStatic<LLVMContext> GlobalContext;
static LLVMContext &getGlobalContext() { return *GlobalContext; }
#endif

/* In LLVM 5.0, when -O0 passed to clang, the functions generated with clang
 * will have optnone attribute which would lead to some transform passes
 * disabled, like mem2reg. Therefore, we have to remove this attribute from
 * these functions.
 */
#if LLVM_VERSION_MAJOR == 5
struct EnableFunctionOptPass : public FunctionPass {
  static char ID;
  EnableFunctionOptPass() : FunctionPass(ID) {}
  bool runOnFunction(Function &F) override {
    if (F.hasFnAttribute(Attribute::OptimizeNone)) {
      F.removeFnAttr(Attribute::OptimizeNone);
    }
    return true;
  }
};

char EnableFunctionOptPass::ID = 0;
#endif

class LiveVariable : public ModulePass,
                     public DataFlow<BitVector>,
                     public AssemblyAnnotationWriter {

public:
  static char ID;

  // domain vector to store all definitions and function arguments
  std::vector<Value *> domain;
  // map values to their bitvector
  std::vector<Value *> *bvIndexToInstrArg;
  // map values (args and variables) to their bit vector index
  ValueMap<Value *, int> *valueToBitVectorIndex;
  // IN set for an instruction inside basic block
  ValueMap<const Instruction *, BitVector *> *instrInSet;
  int domainSize;
  int numArgs;
  int numInstr;
  // live info dump, live_dump[i] indicates the number of
  // program points with i live variables
  int live_dump[100];
  size_t n = 0;

  // mark forward false as a backward analysis pass
  LiveVariable() : ModulePass(ID), DataFlow<BitVector>(false) {
    bvIndexToInstrArg = new std::vector<Value *>();
    valueToBitVectorIndex = new ValueMap<Value *, int>();
    instrInSet = new ValueMap<const Instruction *, BitVector *>();
  }

  // result dump
  // live variables before each basic block
  virtual void emitBasicBlockStartAnnot(const BasicBlock *bb,
                                        formatted_raw_ostream &os) {
    os << "; ";
    if (!isa<PHINode>(*(bb))) {
      const BitVector *bv = (*in)[&*bb];

      for (size_t i = 0; i < bv->size(); i++) {
        if ((*bv)[i]) {
          os << (*bvIndexToInstrArg)[i]->getName();
          os << ", ";
        }
      }
    }
    os << '\n';
  }

  // result dump
  // live variables before each instruction
  virtual void emitInstructionAnnot(const Instruction *i,
                                    formatted_raw_ostream &os) {
    os << "; ";
    if (!isa<PHINode>(*(i))) {
      const BitVector *bv = (*instrInSet)[&*i];
      for (size_t i = 0; i < bv->size(); i++) {
        if ((*bv)[i]) {
          os << (*bvIndexToInstrArg)[i]->getName();
          os << ", ";
        }
      }
    }
    os << '\n';
  }

  // set the boundary condition for block
  // explicit constructor of BitVector
  virtual void setBoundaryCondition(BitVector *blockBoundary) {
    *blockBoundary = BitVector(domainSize, false);
  }

  // union (bitwise OR) operator '|=' overriden in BitVector class
  virtual void meetOp(BitVector *lhs, const BitVector *rhs) { *lhs |= *rhs; }

  // initially empty set, each bit represent a value
  virtual BitVector *initFlowValue(BasicBlock &b, SetType setType) {
    return new BitVector(domainSize, false);
  }

  // transfer function:
  // in[n] = use[n] U (out[n] - def[n])
  virtual BitVector *transferFunc(BasicBlock &bb) {
    BitVector *outNowIn = new BitVector(*((*out)[&bb]));
    // for empty blocks
    BitVector *immIn = outNowIn;
    Instruction *tempInst;

    // go through instructions in reverse
    // FIXME: reverse iterator
    BasicBlock::iterator ii = --(bb.end()), ib = bb.begin();
    while (true) {
      // inherit data from next instruction
      tempInst = &*ii;
      immIn = (*instrInSet)[tempInst];
      *immIn = *outNowIn;

      // if this instruction is a new definition, remove it
      if (isDefinition(tempInst)) {
        (*immIn)[(*valueToBitVectorIndex)[tempInst]] = false;
      }

      // add the arguments, unless it is a phi node
      if (!isa<PHINode>(*ii)) {
        User::op_iterator OI, OE;
        for (OI = tempInst->op_begin(), OE = tempInst->op_end(); OI != OE;
             ++OI) {
          if (isa<Instruction>(*OI) || isa<Argument>(*OI)) {
            (*immIn)[(*valueToBitVectorIndex)[*OI]] = true;
          }
        }
      } else if (isa<PHINode>(*ii)) {
        PHINode *phiNode = cast<PHINode>(&*ii);
        for (size_t incomingIdx = 0;
             incomingIdx < phiNode->getNumIncomingValues(); incomingIdx++) {
          Value *val = phiNode->getIncomingValue(incomingIdx);
          if (isa<Instruction>(val) || isa<Argument>(val)) {
            int valIdx = (*valueToBitVectorIndex)[val];
            BasicBlock *incomingBlock = phiNode->getIncomingBlock(incomingIdx);
            if ((*neighbourSpecificValues).find(incomingBlock) ==
                (*neighbourSpecificValues).end())
              (*neighbourSpecificValues)[incomingBlock] =
                  new BitVector(domainSize);
            (*(*neighbourSpecificValues)[incomingBlock]).set(valIdx);
          }
        }
      }
      outNowIn = immIn;
      if (ii == ib)
        break;

      --ii;
    }

    return immIn;
  }

  bool isDefinition(Instruction *ii) { return !(isa<TerminatorInst>(ii)); }

  // for result dump
  void calculate(const Instruction *i) {
    size_t count = 0;
    if (!isa<PHINode>(*(i))) {
      const BitVector *bv = (*instrInSet)[&*i];
      for (size_t i = 0; i < bv->size(); i++) {
        if ((*bv)[i])
          count++;
      }
      if (count > n) {
        n = count + 1;
      }
      live_dump[count]++;
    }
  }

  // evaluate each function
  virtual bool evalFunc(Function &F) {
    domain.clear();
    bvIndexToInstrArg = new std::vector<Value *>();
    valueToBitVectorIndex = new ValueMap<Value *, int>();
    instrInSet = new ValueMap<const Instruction *, BitVector *>();

    int index = 0;
    Function::arg_iterator arg_it = F.arg_begin(), arg_ie = F.arg_end();
    for (; arg_it != arg_ie; ++arg_it) {
      domain.push_back(&*arg_it);
      bvIndexToInstrArg->push_back(&*arg_it);
      (*valueToBitVectorIndex)[&*arg_it] = index;
      index++;
    }

    inst_iterator inst_it = inst_begin(F), inst_ie = inst_end(F);
    for (; inst_it != inst_ie; ++inst_it) {
      domain.push_back(&*inst_it);
      bvIndexToInstrArg->push_back(&*inst_it);
      (*valueToBitVectorIndex)[&*inst_it] = index;
      index++;
    }

    domainSize = domain.size();

    // initialize the IN sets inside the basic block for each instruction.
    for (inst_iterator inst_it = inst_begin(F), inst_ie = inst_end(F);
         inst_it != inst_ie; ++inst_it) {
      (*instrInSet)[&*inst_it] = new BitVector(domainSize, false);
    }
    // call the backward analysis method in dataflow
    DataFlow<BitVector>::runOnFunction(F);
    F.print(errs(), this);

    for (inst_iterator inst_it = inst_begin(F), inst_ie = inst_end(F);
         inst_it != inst_ie; ++inst_it) {
      calculate(&*inst_it);
    }

    return false;
  }

  virtual bool runOnModule(Module &M) {
    std::fill_n(live_dump, 50, 0);

    auto MIT = M.begin(), MIE = M.end();
    for (; MIT != MIE; ++MIT) {
      evalFunc(*MIT);
    }

    errs() << "===--- Live Variable Analysis Statistic Info ---===\n";
    for (unsigned int i = 0; i < n; i++) {
      if (live_dump[i] >= 0)
        errs() << i << " : " << live_dump[i] << "\n";
    }
    return false;
  }

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
  }
};

char LiveVariable::ID = 0;
RegisterPass<LiveVariable> X("liveness", "live variable analysis");

static cl::opt<std::string>
    InputFilename(cl::Positional, cl::desc("<filename>.bc"), cl::init(""));

int main(int argc, const char *argv[]) {
  LLVMContext &Context = getGlobalContext();
  // static LLVMContext Context;
  SMDiagnostic Err;
  // Parse the command line to read the Inputfilename
  cl::ParseCommandLineOptions(argc, argv, "Live variable analysis...\n");

  // load the input module
  std::unique_ptr<Module> M = parseIRFile(InputFilename, Err, Context);
  if (!M) {
    Err.print(argv[0], errs());
    return -1;
  }

  llvm::legacy::PassManager Passes;

#if LLVM_VERSION_MAJOR == 5
  Passes.add(new EnableFunctionOptPass());
#endif
  /// Transform it to SSA
  // Passes.add(llvm::createPromoteMemoryToRegisterPass());
  // Passes.add(new LoopInfoWrapperPass());

  Passes.add(new LiveVariable());
  Passes.run(*M.get());
  return 0;
}
