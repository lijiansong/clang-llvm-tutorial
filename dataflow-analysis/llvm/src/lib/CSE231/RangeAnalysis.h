#include "Lattice.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include <algorithm>    // std::max and min


using namespace llvm;
using namespace std;

class RangeAnalysis {
public:
	RangeAnalysis(Instruction * inst, Lattice< map<string,vector<int> > > * incoming);
    Instruction * getInstruction();
    Lattice< map<string,vector<int> > > * getOutgoingEdge();
	Lattice< map<string,vector<int> > > * getOutgoingEdge(BasicBlock * toSuccessor);
    void setIncomingEdge(Lattice< map<string,vector<int> > > * incoming);
	bool isConditionalBranch();
    void applyFlowFunction();
    void dump();
    static void dump(Lattice<map<string,vector<int> > > * lattice);

	static Lattice< map<string,vector<int> > > * join(Lattice< map<string,vector<int> > > * edge_1, Lattice< map<string,vector<int> > > * edge_2);
    static Lattice< map<string,vector<int> > > * merge(Lattice< map<string,vector<int> > > * edge_1, Lattice< map<string,vector<int> > > * edge_2);		
    static bool equal(Lattice< map<string,vector<int> > > * edge1, Lattice< map<string,vector<int> > > * edge2);
private:
	Instruction * _instruction;
	Lattice< map<string,vector<int> > > * _incomingEdge;
	Lattice< map<string,vector<int> > > * _outgoingEdge;
	Lattice< map<string,vector<int> > > * _outgoingTrueEdge;
	Lattice< map<string,vector<int> > > * _outgoingFalseEdge;
	bool _isConditionalBranch;
	void handleAllocaInst(AllocaInst * allocaInst);
    void handleStoreInst(StoreInst * storeInst);
    void handleLoadInst(LoadInst * loadInst);
    void handleBinaryOp(Instruction * inst);
	void handleConditionalBranchInst(BranchInst * inst);
	ConstantInt * tryGetConstantValue(Value * value);
	vector<int> tryGetRange(Value * value);
	vector<int> intersect(vector<int> left, vector<int> right);
};

