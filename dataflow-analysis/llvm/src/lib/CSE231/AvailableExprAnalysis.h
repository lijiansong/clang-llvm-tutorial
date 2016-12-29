#include "Lattice.h"
#include "ExpressionContainer.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"


using namespace llvm;
using namespace std;

class AvailableExprAnalysis {
public:
	AvailableExprAnalysis(Instruction * inst, Lattice<ExpressionContainer> * incoming);
    Instruction * getInstruction();
    Lattice<ExpressionContainer> * getOutgoingEdge();
    Lattice<ExpressionContainer> * getOutgoingEdge(BasicBlock * toSuccessor);
    void setIncomingEdge(Lattice<ExpressionContainer> * incoming);
    void applyFlowFunction();
    void dump();
    static void dump(Lattice<ExpressionContainer> * container);

    static Lattice<ExpressionContainer> * merge(Lattice<ExpressionContainer> * edge_1, Lattice<ExpressionContainer> * edge_2);
    static bool equal(Lattice<ExpressionContainer> * edge1, Lattice<ExpressionContainer> * edge2);
private:
	Instruction * _instruction;
	Lattice<ExpressionContainer> * _incomingEdge;
	Lattice<ExpressionContainer> * _outgoingEdge;
    void handleBinaryOp(Instruction * inst);
    ConstantInt * tryGetConstantValue(Value * value);
};

