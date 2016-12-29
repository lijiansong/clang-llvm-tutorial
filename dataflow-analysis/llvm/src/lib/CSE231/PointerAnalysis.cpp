#include "PointerAnalysis.h"
#include "llvm/Support/raw_ostream.h"

PointerAnalysis::PointerAnalysis(Instruction * inst, Lattice< map<string, set<Value*,valueComp> > > * incoming) {
	_instruction = inst;
	map<string,set<Value*,valueComp> > empty;
	_incomingEdge = new Lattice< map<string, set<Value*,valueComp> > >(false,true,empty);
	*_incomingEdge = *incoming;
	_outgoingEdge = new Lattice< map<string, set<Value*,valueComp> > >(false,true,empty);
	_outgoingTrueEdge = new Lattice< map<string, set<Value*,valueComp> > >(false,true,empty);
	_outgoingFalseEdge = new Lattice< map<string, set<Value*,valueComp> > >(false,true,empty);
	if(BranchInst::classof(_instruction)) {
		BranchInst * branchInst = (BranchInst *)_instruction;
		if (branchInst->isConditional()) 
			_isConditionalBranch = true;
		else 
			_isConditionalBranch = false;
	}
	else
		_isConditionalBranch = false;
}

bool PointerAnalysis::isConditionalBranch() {
	return _isConditionalBranch;
}

void PointerAnalysis::applyFlowFunction() {
    if (StoreInst::classof(_instruction)) {
        handleStoreInst((StoreInst *) _instruction);
    }
    else if (LoadInst::classof(_instruction)) {
        handleLoadInst((LoadInst *) _instruction);
    }
	else { //temp 
		*_outgoingEdge = *_incomingEdge;
		*_outgoingTrueEdge = *_incomingEdge;
		*_outgoingFalseEdge = *_incomingEdge;
	}
}

Instruction * PointerAnalysis::getInstruction() {
    return _instruction;
}

Lattice< map<string, set<Value*,valueComp> > > * PointerAnalysis::getOutgoingEdge() {
    return _outgoingEdge;
}

Lattice< map<string, set<Value*,valueComp> > > * PointerAnalysis::getOutgoingEdge(BasicBlock * toSuccessor) {
    return _outgoingEdge;
}

void PointerAnalysis::setIncomingEdge(Lattice< map<string, set<Value*,valueComp> > > * incoming) {    
    *_incomingEdge = *incoming;
}

bool PointerAnalysis::equal(Lattice< map<string, set<Value*,valueComp> > > * edge_1, Lattice< map<string, set<Value*,valueComp> > > * edge_2) {
	if(edge_1->isTop() && edge_2->isTop()) 
		return true;
	else if(edge_1->isBottom() && edge_2->isBottom())
		return true;
	map<string,set<Value*,valueComp> > edge1 = edge_1->getFacts();
	map<string,set<Value*,valueComp> > edge2 = edge_2->getFacts();
    if(edge1.size() != edge2.size())
        return false;
    for (map<string,set<Value*,valueComp> >::iterator i = edge1.begin(); i != edge1.end(); i++) {
        if (edge1[i->first] != edge2[i->first])
            return false;
    }

    return true;
}

void PointerAnalysis::dump() {
    errs() << "\t\t\tINCOMING:\n";
    dump(_incomingEdge);

	if (!_isConditionalBranch) {
		errs() << "\t\t\tOUTGOING:\n";
        dump(_outgoingEdge);
    }
	else {
		errs() << "\t\t\tOUTGOING (TRUE):\n";
        dump(_outgoingTrueEdge);
		errs() << "\t\t\tOUTGOING (FALSE):\n";
        dump(_outgoingFalseEdge);
	}
    errs() << "\t\t--------------------------------------------------------\n";
}

void PointerAnalysis::dump(Lattice<map<string,set<Value*,valueComp> > > * lattice) {
    if (lattice->isTop()) {
        errs() << "\t\t\tis Top\n";
    }
    else if (lattice->isBottom()) {
        errs() << "\t\t\tis Bottom\n";
    }
    else {
        map<string,set<Value*,valueComp> > edge = lattice->getFacts();
        for (map<string, set<Value*,valueComp> >::iterator i = edge.begin(); i != edge.end(); i++) {
            errs() << "\t\t\t\t" << i->first << " -> ";

            set<Value*,valueComp>::iterator j = i->second.begin();
            while(j != i->second.end()) {
                errs() << (*j)->getName().str();
                                                                
                if (++j != i->second.end()) {
                    errs() << ", ";
                }
            }
			errs() << "\n";
        }
    }
}

void PointerAnalysis::handleStoreInst(StoreInst * storeInst) {
	string name = storeInst->getPointerOperand()->getName().str();
	map<string,set<Value*,valueComp> > edgeMap = _incomingEdge->getFacts(); 
	edgeMap.erase(name);

    const Type* type = storeInst->getPointerOperand()->getType();

    if (type->isPointerTy() && type->getContainedType(0)->isPointerTy()) {
        edgeMap[name].clear();
        edgeMap[name].insert(storeInst->getValueOperand());
    }

    _outgoingEdge->setNewFacts(false,false,edgeMap);
}

void PointerAnalysis::handleLoadInst(LoadInst * loadInst) {
    map<string,set<Value*,valueComp> > edgeMap = _incomingEdge->getFacts();
	edgeMap.erase(loadInst->getOperandUse(0).getUser()->getName().str());
	
    const Type* type = loadInst->getOperand(0)->getType();
    if (type->isPointerTy() && type->getContainedType(0)->isPointerTy()) {
        edgeMap.erase(loadInst->getOperandUse(0).getUser()->getName().str());
		if (edgeMap.count(loadInst->getOperand(0)->getName().str()) > 0) {
        	edgeMap[loadInst->getOperandUse(0).getUser()->getName().str()].insert(edgeMap[loadInst->getOperand(0)->getName().str()].begin(), edgeMap[loadInst->getOperand(0)->getName().str()].end());
		}
    }
    
    _outgoingEdge->setNewFacts(false,false,edgeMap);
}

MustPointerAnalysis::MustPointerAnalysis(Instruction * inst, Lattice< map<string, set<Value*,valueComp> > > * incoming) : PointerAnalysis(inst, incoming) { }

// will be a join
Lattice< map<string, set<Value*,valueComp> > > * MustPointerAnalysis::merge(Lattice< map<string, set<Value*,valueComp> > > * edge_1, Lattice< map<string, set<Value*,valueComp> > > * edge_2) {
	map<string,set<Value*,valueComp> > outgoingEdge;
	if (edge_1->isTop() || edge_2->isTop()) {
		return new Lattice< map<string, set<Value*,valueComp> > >(true, false, outgoingEdge); // return Top
	}
	else if (edge_1->isBottom() && edge_2->isBottom()) {
		return new Lattice< map<string, set<Value*,valueComp> > >(false, true, outgoingEdge);
	}
	else if (edge_1->isBottom()) {
		return new Lattice< map<string, set<Value*,valueComp> > >(false, false, edge_2->getFacts());
	}
	else if (edge_2->isBottom()) {
		return new Lattice< map<string, set<Value*,valueComp> > >(false, false, edge_1->getFacts());
	}

	map<string, set<Value*,valueComp> > edge1 = edge_1->getFacts();
	map<string, set<Value*,valueComp> > edge2 = edge_2->getFacts();
	
    for (map<string, set<Value*,valueComp> >::iterator i = edge1.begin(); i != edge1.end(); i++) {
        for (map<string, set<Value*,valueComp> >::iterator j = edge2.begin(); j != edge2.end(); j++) {
            // If item is in both edges and are equal, add to outgoing edge
            if ( (i->first == j->first) && (i->second == j->second) ) {
                outgoingEdge[i->first] = i->second;
                break;
            }
        }
    }

    return new Lattice< map<string, set<Value*,valueComp> > >(false, false, outgoingEdge);
}

MayPointerAnalysis::MayPointerAnalysis(Instruction * inst, Lattice< map<string, set<Value*,valueComp> > > * incoming) : PointerAnalysis(inst, incoming) { } 

// will be a join
Lattice< map<string, set<Value*,valueComp> > > * MayPointerAnalysis::merge(Lattice< map<string, set<Value*,valueComp> > > * edge_1, Lattice< map<string, set<Value*,valueComp> > > * edge_2) {
	map<string,set<Value*,valueComp> > outgoingEdge;
	if (edge_1->isTop() || edge_2->isTop()) {
		return new Lattice< map<string, set<Value*,valueComp> > >(true, false, outgoingEdge); // return Top
	}
	else if (edge_1->isBottom() && edge_2->isBottom()) {
		return new Lattice< map<string, set<Value*,valueComp> > >(false, true, outgoingEdge);
	}
	else if (edge_1->isBottom()) {
		return new Lattice< map<string, set<Value*,valueComp> > >(false, false, edge_2->getFacts());
	}
	else if (edge_2->isBottom()) {
		return new Lattice< map<string, set<Value*,valueComp> > >(false, false, edge_1->getFacts());
	}

	map<string, set<Value*,valueComp> > edge1 = edge_1->getFacts();
	map<string, set<Value*,valueComp> > edge2 = edge_2->getFacts();
	
    for (map<string, set<Value*,valueComp> >::iterator i = edge1.begin(); i != edge1.end(); i++) {
        outgoingEdge[i->first].insert(i->second.begin(), i->second.end());
    }
    for (map<string, set<Value*,valueComp> >::iterator j = edge2.begin(); j != edge2.end(); j++) {
        outgoingEdge[j->first].insert(j->second.begin(), j->second.end());
    }

    return new Lattice< map<string, set<Value*,valueComp> > >(false, false, outgoingEdge);
}



