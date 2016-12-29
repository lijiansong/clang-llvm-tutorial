#include "Lattice.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"


using namespace llvm;
using namespace std;

class ConstantPropAnalysis {
public:
	ConstantPropAnalysis(Instruction * inst, Lattice< map<string,ConstantInt*> > * incoming);
    Instruction * getInstruction();
    Lattice< map<string,ConstantInt*> > * getOutgoingEdge();
	Lattice< map<string,ConstantInt*> > * getOutgoingEdge(BasicBlock * toSuccessor);
    void setIncomingEdge(Lattice< map<string,ConstantInt*> > * incoming);
	bool isConditionalBranch();
    void applyFlowFunction();
    void dump();
    static void dump(Lattice<map<string,ConstantInt*> > * lattice);

    static Lattice< map<string,ConstantInt*> > * merge(Lattice< map<string,ConstantInt*> > * edge_1, Lattice< map<string,ConstantInt*> > * edge_2);
    static bool equal(Lattice< map<string,ConstantInt*> > * edge1, Lattice< map<string,ConstantInt*> > * edge2);

private:
	Instruction * _instruction;
	Lattice< map<string,ConstantInt*> > * _incomingEdge;
	Lattice< map<string,ConstantInt*> > * _outgoingEdge;
	Lattice< map<string,ConstantInt*> > * _outgoingTrueEdge;
	Lattice< map<string,ConstantInt*> > * _outgoingFalseEdge;
	bool _isConditionalBranch;
    void handleStoreInst(StoreInst * storeInst);
    void handleLoadInst(LoadInst * loadInst);
    void handleBinaryOp(Instruction * inst);
	void handleConditionalBranchInst(BranchInst * inst);
    ConstantInt * tryGetConstantValue(Value * value);
};

