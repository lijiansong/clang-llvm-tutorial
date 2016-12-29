#include "Lattice.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"


using namespace llvm;
using namespace std;

class PointerAnalysis {
public:
    Instruction * getInstruction();
    Lattice< map<string, set<Value*,valueComp> > > * getOutgoingEdge();
	Lattice< map<string, set<Value*,valueComp> > > * getOutgoingEdge(BasicBlock * toSuccessor);
    void setIncomingEdge(Lattice< map<string, set<Value*,valueComp> > > * incoming);
	bool isConditionalBranch();
    void applyFlowFunction();
    void dump();
    static void dump(Lattice<map<string,set<Value*,valueComp> > > * lattice);

    static bool equal(Lattice< map<string, set<Value*,valueComp> > > * edge1, Lattice< map<string, set<Value*,valueComp> > > * edge2);

protected:
	PointerAnalysis(Instruction * inst, Lattice< map<string, set<Value*,valueComp> > > * incoming);

	Instruction * _instruction;
	Lattice< map<string, set<Value*,valueComp> > > * _incomingEdge;
	Lattice< map<string, set<Value*,valueComp> > > * _outgoingEdge;
	Lattice< map<string, set<Value*,valueComp> > > * _outgoingTrueEdge;
	Lattice< map<string, set<Value*,valueComp> > > * _outgoingFalseEdge;
	bool _isConditionalBranch;
    void handleStoreInst(StoreInst * storeInst);
    void handleLoadInst(LoadInst * loadInst);
};

class MustPointerAnalysis : public PointerAnalysis {
public:
	MustPointerAnalysis(Instruction * inst, Lattice< map<string, set<Value*,valueComp> > > * incoming);
    
    static Lattice< map<string, set<Value*,valueComp> > > * merge(Lattice< map<string, set<Value*,valueComp> > > * edge_1, Lattice< map<string, set<Value*,valueComp> > > * edge_2);
};

class MayPointerAnalysis : public PointerAnalysis {
public:
	MayPointerAnalysis(Instruction * inst, Lattice< map<string, set<Value*,valueComp> > > * incoming);
    
    static Lattice< map<string, set<Value*,valueComp> > > * merge(Lattice< map<string, set<Value*,valueComp> > > * edge_1, Lattice< map<string, set<Value*,valueComp> > > * edge_2);
};

