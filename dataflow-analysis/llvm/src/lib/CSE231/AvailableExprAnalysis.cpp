#include "AvailableExprAnalysis.h"
#include "llvm/Support/raw_ostream.h"

AvailableExprAnalysis::AvailableExprAnalysis(Instruction * inst, Lattice<ExpressionContainer> * incoming) {
	_instruction = inst;
	ExpressionContainer empty;
	_incomingEdge = new Lattice<ExpressionContainer>(false,true,empty);
	*_incomingEdge = *incoming;
	_outgoingEdge = new Lattice<ExpressionContainer>(false,true,empty);
}

void AvailableExprAnalysis::applyFlowFunction() {
    if (_instruction->isBinaryOp()) {
        handleBinaryOp(_instruction);            
    }
	else {//temp 
		*_outgoingEdge = *_incomingEdge;
	}
}

Instruction * AvailableExprAnalysis::getInstruction() {
    return _instruction;
}

Lattice<ExpressionContainer> * AvailableExprAnalysis::getOutgoingEdge() {
    return _outgoingEdge;
}

Lattice<ExpressionContainer> * AvailableExprAnalysis::getOutgoingEdge(BasicBlock * toSucessor) {
    return _outgoingEdge;
}

void AvailableExprAnalysis::setIncomingEdge(Lattice<ExpressionContainer> * incoming) {    
    *_incomingEdge = *incoming;
}

// will be a join
Lattice<ExpressionContainer> * AvailableExprAnalysis::merge(Lattice<ExpressionContainer> * edge_1, Lattice<ExpressionContainer> * edge_2) {
	ExpressionContainer outgoingEdge;
	if (edge_1->isTop() || edge_2->isTop()) {
		return new Lattice<ExpressionContainer>(true, false, outgoingEdge); // return Top
	}
	else if (edge_1->isBottom() && edge_2->isBottom()) {
		return new Lattice<ExpressionContainer>(false, true, outgoingEdge);
	}
	else if (edge_1->isBottom()) {
		return new Lattice<ExpressionContainer>(false, false, edge_2->getFacts());
	}
	else if (edge_2->isBottom()) {
		return new Lattice<ExpressionContainer>(false, false, edge_1->getFacts());
	}
	map<string,Expression*> edge1 = edge_1->getFacts().getMap();
	map<string,Expression*> edge2 = edge_2->getFacts().getMap();

    for (map<string,Expression*>::iterator i = edge1.begin(); i != edge1.end(); i++) {
        for (map<string,Expression*>::iterator j = edge2.begin(); j != edge2.end(); j++) {
            // If item is in both edges and are equal, add to outgoing edge
            if (i->first == j->first && i->second->isEqual(j->second)) {
                errs() << "ADD TO OUTGOING\n";
                outgoingEdge.addExpression(i->first,i->second);
                break;
            }
        }
    }
    
    return new Lattice<ExpressionContainer>(false, false, outgoingEdge);
}

bool AvailableExprAnalysis::equal(Lattice<ExpressionContainer> * edge_1, Lattice<ExpressionContainer> * edge_2) {
	if(edge_1->isTop() && edge_2->isTop()) 
		return true;
	else if(edge_1->isBottom() && edge_2->isBottom())
		return true;
	map<string,Expression*> edge1 = edge_1->getFacts().getMap();
	map<string,Expression*> edge2 = edge_2->getFacts().getMap();
    if(edge1.size() != edge2.size())
        return false;
    for (map<string,Expression*>::iterator i = edge1.begin(); i != edge1.end(); i++) {
        if (edge1[i->first] != edge2[i->first])
            return false;
    }

    return true;
}

void AvailableExprAnalysis::dump() {
    errs() << "\t\t\tINCOMING:\n";
    dump(_incomingEdge);

    errs() << "\t\t\tOUTGOING:\n";
    dump(_outgoingEdge);

    errs() << "\t\t--------------------------------------------------------\n";
}

void AvailableExprAnalysis::dump(Lattice<ExpressionContainer> * lattice) {
    if (lattice->isTop()) {
        errs() << "\t\t\tis Top\n";
    }
    else if (lattice->isBottom()) {
        errs() << "\t\t\tis Bottom\n";
    }
    else {
        lattice->getFacts().dump();
    }
}

void AvailableExprAnalysis::handleBinaryOp(Instruction * inst) {
    ExpressionContainer edgeMap = _incomingEdge->getFacts();

    edgeMap.addExpression(inst->getOperandUse(0).getUser()->getName().str(), new Expression(inst));
    //edgeMap[new Expression(inst)].push_back(inst->getOperandUse(0).getUser()->getName().str());

    _outgoingEdge->setNewFacts(false,false,edgeMap);
}

