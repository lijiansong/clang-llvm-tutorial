#include "RangeAnalysis.h"
#include "llvm/Support/raw_ostream.h"


RangeAnalysis::RangeAnalysis(Instruction * inst, Lattice< map<string,vector<int> > > * incoming) {
	_instruction = inst;
	map<string, vector<int> > empty;
	_incomingEdge = new Lattice< map<string,vector<int> > >(false,true,empty);
	*_incomingEdge = *incoming;
	_outgoingEdge = new Lattice< map<string,vector<int> > >(false,true,empty);
	_outgoingTrueEdge = new Lattice< map<string,vector<int> > >(false,true,empty);
	_outgoingFalseEdge = new Lattice< map<string,vector<int> > >(false,true,empty);
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

bool RangeAnalysis::isConditionalBranch() {
	return _isConditionalBranch;
}

void RangeAnalysis::applyFlowFunction() {
	if (AllocaInst::classof(_instruction)) {
        handleAllocaInst((AllocaInst *) _instruction);
    }
    else if (StoreInst::classof(_instruction)) {
        handleStoreInst((StoreInst *) _instruction);
    }
    else if (LoadInst::classof(_instruction)) {
        handleLoadInst((LoadInst *) _instruction);
    }
    else if (_instruction->isBinaryOp()) {
        handleBinaryOp(_instruction);            
    }
	else if (_isConditionalBranch) {
		handleConditionalBranchInst((BranchInst *) _instruction);
	}
	else { //temp 
		*_outgoingEdge = *_incomingEdge;
	}
}

Instruction * RangeAnalysis::getInstruction() {
    return _instruction;
}

Lattice< map<string,vector<int> > > * RangeAnalysis::getOutgoingEdge() {
    return _outgoingEdge;
}

Lattice< map<string,vector<int> > > * RangeAnalysis::getOutgoingEdge(BasicBlock * toSuccessor) {
	if(!_isConditionalBranch) {
		//errs() << "[RangeAnalysis::getOutgoingEdge(BasicBlock * toSuccessor)] not a conditional branch predecessor, return normal outgoing edge\n";
		return _outgoingEdge;
	}
	BranchInst * branchInst = (BranchInst * )_instruction;
	BasicBlock * trueBlock = branchInst->getSuccessor(0);
	BasicBlock * falseBlock = branchInst->getSuccessor(1);
	if (toSuccessor == trueBlock) {
		return _outgoingTrueEdge;
	}
	else if(toSuccessor == falseBlock) {
		return _outgoingFalseEdge;
	}
	else {
		errs() << "[getOutgoingEdge(BasicBlock * toSuccessor)] the world is weird.\n";
	}
	return NULL;
}


void RangeAnalysis::setIncomingEdge(Lattice< map<string,vector<int> > > * incoming) {    
    *_incomingEdge = *incoming;
}

// will be a join
Lattice< map<string,vector<int> > > * RangeAnalysis::join(Lattice< map<string,vector<int> > > * edge_1, Lattice< map<string,vector<int> > > * edge_2) {
	map<string, vector<int> > outgoingEdge;
	if (edge_1->isTop() || edge_2->isTop()) {
		return new Lattice< map<string,vector<int> > >(true, false, outgoingEdge); // return Top
	}
	else if (edge_1->isBottom() && edge_2->isBottom()) { // both are bottom (empty) return bottom
		return new Lattice< map<string,vector<int> > >(false, true, outgoingEdge);
	}
	else if (edge_1->isBottom()) {
		return edge_2;
	}
	else if (edge_2->isBottom()) {
		return edge_1;
	}
	map<string, vector<int> > edge1 = edge_1->getFacts();
	map<string, vector<int> > edge2 = edge_2->getFacts();
	
    for (map<string, vector<int> >::iterator i = edge1.begin(); i != edge1.end(); i++) {
        bool isInBothEdges = false;
        for (map<string, vector<int> >::iterator j = edge2.begin(); j != edge2.end(); j++) {
            // If item is in both edges, add to outgoing edge
            if ( i->first == j->first ) {
                isInBothEdges = true;
                break;
            }
        }
        if (isInBothEdges) {
			int varmin = max(edge1[i->first][0], edge2[i->first][0]);
			int varmax = min(edge1[i->first][1], edge2[i->first][1]);
			if (varmin <= varmax) { // there is an intersection of values
            	outgoingEdge[i->first].push_back( varmax );
				outgoingEdge[i->first].push_back( varmin );
			}
			//edge2.erase(i->first); //erase...in order to get remainders later
        }
    }
	/*for (map<string, vector<int> >::iterator i = edge2.begin(); i != edge2.end(); i++) {
    	outgoingEdge[i->first] = i->second;
    }*/
    return new Lattice< map<string,vector<int> > >(false, false, outgoingEdge);
}

// will be a meet
Lattice< map<string,vector<int> > > * RangeAnalysis::merge(Lattice< map<string,vector<int> > > * edge_1, Lattice< map<string,vector<int> > > * edge_2) {
	map<string, vector<int> > outgoingEdge;
	if (edge_1->isTop() || edge_2->isTop()) {
		return new Lattice< map<string,vector<int> > >(true, false, outgoingEdge); // return Top
	}
	else if (edge_1->isBottom() && edge_2->isBottom()) { //both are bottom (empty) return bottom
		return new Lattice< map<string,vector<int> > >(false, true, outgoingEdge);
	}
	map<string, vector<int> > edge1 = edge_1->getFacts();
	map<string, vector<int> > edge2 = edge_2->getFacts();
	
    for (map<string, vector<int> >::iterator i = edge1.begin(); i != edge1.end(); i++) {
        bool isInBothEdges = false;
        for (map<string, vector<int> >::iterator j = edge2.begin(); j != edge2.end(); j++) {
            // If item is in both edges, add to outgoing edge
            if ( i->first == j->first ) {
                isInBothEdges = true;
                break;
            }
        }
        if (isInBothEdges) {
            outgoingEdge[i->first].push_back( min(edge1[i->first][0], edge2[i->first][0]) );
			outgoingEdge[i->first].push_back( max(edge1[i->first][1], edge2[i->first][1]) );
			edge2.erase(i->first); //erase...in order to get remainders later
        }
    }
	for (map<string, vector<int> >::iterator i = edge2.begin(); i != edge2.end(); i++) {
    	outgoingEdge[i->first] = i->second;
    }
    return new Lattice< map<string,vector<int> > >(false, false, outgoingEdge);

}

bool RangeAnalysis::equal(Lattice< map<string,vector<int> > > * edge_1, Lattice< map<string,vector<int> > > * edge_2) {
	return true;
}

void RangeAnalysis::dump() {
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

void RangeAnalysis::dump(Lattice<map<string,vector<int> > > * lattice) {
    if (lattice->isTop()) {
        errs() << "\t\t\tis Top\n";
    }
    else if (lattice->isBottom()) {
        errs() << "\t\t\tis Bottom\n";
    }
    else {
        map<string,vector<int> > edge = lattice->getFacts();

        for (map<string, vector<int> >::iterator i = edge.begin(); i != edge.end(); i++) {
            errs() << "\t\t\t\t" << i->first << " -> (" << i->second[0] << ", " << i->second[1] << ")\n";
        }
    }
}


void RangeAnalysis::handleAllocaInst(AllocaInst * allocaInst) {
	// we're gonna assume everything is an int
	map<string, vector<int> > edgeMap = _incomingEdge->getFacts();
	edgeMap.erase(allocaInst->getOperandUse(0).getUser()->getName().str());
	vector<int> intrange;
	intrange.push_back(APInt::getSignedMinValue(32).getSExtValue());
	intrange.push_back(APInt::getSignedMaxValue(32).getSExtValue());
	edgeMap[allocaInst->getOperandUse(0).getUser()->getName().str()] = intrange;
	_outgoingEdge->setNewFacts(false,false,edgeMap);
}

void RangeAnalysis::handleStoreInst(StoreInst * storeInst) {
	string name = storeInst->getPointerOperand()->getName().str();
	map<string, vector<int> > edgeMap = _incomingEdge->getFacts(); 
	edgeMap.erase(name);
    ConstantInt* CI = tryGetConstantValue(storeInst->getValueOperand()); 
	vector<int> storedRange = tryGetRange(storeInst->getValueOperand()); 
    if (CI == NULL && storedRange.empty()) { // X = unknown
		errs() << "[ConstantPropAnalysis::handleStoreInst] not a ConstantInt or range\n";
		_outgoingEdge->setNewFacts(_incomingEdge->isTop(), _incomingEdge->isBottom(), edgeMap);
    }
	else if (CI) { // X = C
    	edgeMap[name].push_back(CI->getSExtValue());
		edgeMap[name].push_back(CI->getSExtValue());
		_outgoingEdge->setNewFacts(false,false,edgeMap);
	}
	else { // X = Y where Y is in map
		edgeMap[name] = storedRange;
		_outgoingEdge->setNewFacts(false,false,edgeMap);
	}
}

void RangeAnalysis::handleLoadInst(LoadInst * loadInst) { //of the form X = Y
	map<string, vector<int> > edgeMap = _incomingEdge->getFacts();
	edgeMap.erase(loadInst->getOperandUse(0).getUser()->getName().str());
	if (edgeMap.count(loadInst->getOperand(0)->getName().str()) > 0)
    	edgeMap[loadInst->getOperandUse(0).getUser()->getName().str()] = edgeMap[loadInst->getOperand(0)->getName().str()];
    _outgoingEdge->setNewFacts(false,false,edgeMap);
}


void RangeAnalysis::handleBinaryOp(Instruction * inst) {
	map<string, vector<int> > rangeMap = _incomingEdge->getFacts();
	string dest = inst->getOperandUse(0).getUser()->getName().str();
	rangeMap.erase(dest);
	ConstantInt * operandConstant1 = tryGetConstantValue(inst->getOperand(0));
	ConstantInt * operandConstant2 = tryGetConstantValue(inst->getOperand(1));
    vector<int> operandRange1 = tryGetRange(inst->getOperand(0));
    vector<int> operandRange2 = tryGetRange(inst->getOperand(1));
	vector<int> resultRange;

    //errs() << "Operands:  " <<  inst->getOperand(0)->getName().str() << "\t" << inst->getOperand(1)->getName().str() <<
    //"\tdest" << inst->getOperandUse(0).getUser()->getName().str() << "\n";

    if ( (operandConstant1 == NULL && operandRange1.empty()) || (operandConstant2 == NULL && operandRange2.empty()) ) {
		_outgoingEdge->setNewFacts(false, false, rangeMap);
        errs() << "No Constant or in binary op or range is not in incoming edge.\n";
        return;
    }
	// Constant constant
	// range range
	// range constant
	if (operandConstant1 && operandConstant2) {
		int64_t operand1 = operandConstant1->getSExtValue();
		int64_t operand2 = operandConstant2->getSExtValue();
		int64_t result;

		switch (inst->getOpcode()) {
		    case Instruction::Add:
		        result = operand1 + operand2;
		        break;

		    case Instruction::Mul:
		        result = operand1 * operand2;
		        break;

		    case Instruction::Sub:
		        result = operand1 - operand2;
		        break;

		    case Instruction::SDiv:
		    case Instruction::UDiv:
		        result = operand1 / operand2;
		        break;

		    case Instruction::SRem:
		    case Instruction::URem:
		        result = operand1 % operand2;
		        break;

		    case Instruction::Or:
		        result = operand1 | operand2;
		        break;

		    case Instruction::And:
		        result = operand1 & operand2;
		        break;

		    case Instruction::Xor:
		        result = operand1 ^ operand2;
		        break;
		}
		resultRange.push_back(result); //min
		resultRange.push_back(result); //max
	}
	else if(!operandRange1.empty() && !operandRange2.empty()) {
		int resultMin;
		int resultMax;
		int result; // a temp to check for min versus max changes
		switch (inst->getOpcode()) {
		    case Instruction::Add:
		        resultMin = operandRange1[0] + operandRange2[0];
				resultMax = operandRange1[1] + operandRange2[1];
		        break;

		    case Instruction::Mul:
		       	resultMin = operandRange1[0] + operandRange2[0];
				resultMax = operandRange1[1] + operandRange2[1];
				if (resultMin > resultMax) { //swap if needed
					result = resultMin;
					resultMin = resultMax;
					resultMax = result;
				}
		        break;

		    case Instruction::Sub:
		        resultMin = operandRange1[0] - operandRange2[0];
				resultMax = operandRange1[1] - operandRange2[1];
		        break;

		    case Instruction::SDiv:
		    case Instruction::UDiv:
		        resultMin = operandRange1[0] / operandRange2[0];
				resultMax = operandRange1[1] / operandRange2[1];
				if (resultMin > resultMax) { //swap if needed
					result = resultMin;
					resultMin = resultMax;
					resultMax = result;
				}
		        break;

		    case Instruction::SRem:
		    case Instruction::URem:
		        resultMin = operandRange1[0] % operandRange2[0];
				resultMax = operandRange1[1] % operandRange2[1];
				if (resultMin > resultMax) { //swap if needed
					result = resultMin;
					resultMin = resultMax;
					resultMax = result;
				}
		        break;
		}
		resultRange.push_back(result); //min
		resultRange.push_back(result); //max
	}
	else if(!operandRange1.empty()) { // (min, max) op C
		int resultMin;
		int resultMax;
		int result; // a temp to check for min versus max changes
		int64_t operand2 = operandConstant2->getSExtValue();
		switch (inst->getOpcode()) {
		    case Instruction::Add:
		        resultMin = operandRange1[0] + operand2;
				resultMax = operandRange1[1] + operand2;
		        break;

		    case Instruction::Mul:
		       	resultMin = operandRange1[0] + operand2;
				resultMax = operandRange1[1] + operand2;
				if (resultMin > resultMax) { //swap if needed
					result = resultMin;
					resultMin = resultMax;
					resultMax = result;
				}
		        break;

		    case Instruction::Sub:
		        resultMin = operandRange1[0] - operand2;
				resultMax = operandRange1[1] - operand2;
		        break;

		    case Instruction::SDiv:
		    case Instruction::UDiv:
		        resultMin = operandRange1[0] / operand2;
				resultMax = operandRange1[1] / operand2;
				if (resultMin > resultMax) { //swap if needed
					result = resultMin;
					resultMin = resultMax;
					resultMax = result;
				}
		        break;

		    case Instruction::SRem:
		    case Instruction::URem:
		        resultMin = operandRange1[0] % operand2;
				resultMax = operandRange1[1] % operand2;
				if (resultMin > resultMax) { //swap if needed
					result = resultMin;
					resultMin = resultMax;
					resultMax = result;
				}
		        break;
		}
		resultRange.push_back(resultMin); //min
		resultRange.push_back(resultMax); //max
	}
	else if(!operandRange2.empty()) { // C op (min, max)
		int resultMin;
		int resultMax;
		int result; // a temp to check for min versus max changes
		int64_t operand1 = operandConstant1->getSExtValue();
		switch (inst->getOpcode()) {
		    case Instruction::Add:
		        resultMin = operand1 + operandRange2[0];
				resultMax = operand1 + operandRange2[1];
		        break;

		    case Instruction::Mul:
		       	resultMin = operand1 + operandRange2[0];
				resultMax = operand1 + operandRange2[1];
				if (resultMin > resultMax) { //swap if needed
					result = resultMin;
					resultMin = resultMax;
					resultMax = result;
				}
		        break;

		    case Instruction::Sub:
		        resultMin = operand1 - operandRange2[0];
				resultMax = operand1 - operandRange2[1];
		        break;

		    case Instruction::SDiv:
		    case Instruction::UDiv:
		        resultMin = operand1 / operandRange2[0];
				resultMax = operand1 / operandRange2[1];
				if (resultMin > resultMax) { //swap if needed
					result = resultMin;
					resultMin = resultMax;
					resultMax = result;
				}
		        break;

		    case Instruction::SRem:
		    case Instruction::URem:
		        resultMin = operand1 % operandRange2[0];
				resultMax = operand1 % operandRange2[1];
				if (resultMin > resultMax) { //swap if needed
					result = resultMin;
					resultMin = resultMax;
					resultMax = result;
				}
		        break;
		}
		resultRange.push_back(result); //min
		resultRange.push_back(result); //max
	}
    
    rangeMap[dest] = resultRange;
    _outgoingEdge->setNewFacts(false, false, rangeMap);

}

void RangeAnalysis::handleConditionalBranchInst(BranchInst * inst) {
	// Branch flow: 
	// X == 0
	// true will change, false will not
	// X != 0
	// false will change, true will not
	*_outgoingEdge = *_incomingEdge;//temp may remove later
	map<string,vector<int> > trueMap = _incomingEdge->getFacts();
	map<string,vector<int> > falseMap = _incomingEdge->getFacts();

	Value * condition = inst->getCondition();
	if (!ICmpInst::classof(condition)) { // no information, just copy and exit
		*_outgoingTrueEdge  = *_incomingEdge;
		*_outgoingFalseEdge = *_incomingEdge;
		return;
	}
	else {
		ICmpInst * cmpInst = (ICmpInst *)condition; 
		int predicate = cmpInst->isSigned() ? cmpInst->getSignedPredicate() : cmpInst->getUnsignedPredicate();
		Value * lhs = cmpInst->getOperand(0);
		Value * rhs = cmpInst->getOperand(1);
		ConstantInt * rhsConstant = tryGetConstantValue(rhs);
		ConstantInt * lhsConstant = tryGetConstantValue(lhs);
		vector<int> lhsRange = tryGetRange(lhs);
		vector<int> rhsRange = tryGetRange(rhs); 
		Value * variable = NULL;
		ConstantInt * constant = NULL;
		vector<int> range; //empty
		if ((lhsConstant && rhsConstant) || (lhsRange.empty() && rhsRange.empty())) { //both constants or both , no information,just copy and exit
			*_outgoingTrueEdge  = *_incomingEdge;
			*_outgoingFalseEdge = *_incomingEdge;
			return;
		}
		else if(rhsConstant)  { //rhs is a constant int, but lhs is not
			variable = lhs;
			constant = rhsConstant;
		}
		else if(lhsConstant) {  //lhs is a constant int, but rhs is not
			variable = rhs;
			constant = lhsConstant;
		}
		else { // both variables, trick situation
			if (!lhsRange.empty() && !rhsRange.empty()) {
				variable = NULL; //signal that both have ranges, we have to assign new may ranges to both variables!
			}
			else if (!lhsRange.empty()) { // assign to rhs, since it has no range
				range = lhsRange;
				variable = rhs;
			}
			else if (!rhsRange.empty()) { // assign to lhs, since it has no range
				range = rhsRange;
				variable = lhs;
			}
		}
		if (variable && constant) {
			vector<int> variableRange = tryGetRange(variable);
			
			if(predicate == CmpInst::ICMP_EQ) { // X == C
				vector<int> constantRange;
				trueMap.erase(variable->getName().str());
				constantRange.push_back(constant->getSExtValue()); // min
				constantRange.push_back(constant->getSExtValue()); // max
				vector<int> intersection = intersect(variableRange, constantRange);
				if (!intersection.empty()) {
					trueMap[variable->getName().str()] = intersection;
				}
				_outgoingTrueEdge->setNewFacts(false, false, trueMap);
				*_outgoingFalseEdge = *_incomingEdge;
			}
			else if(predicate == CmpInst::ICMP_NE) { // X != C
				vector<int> constantRange;
				falseMap.erase(variable->getName().str());
				constantRange.push_back(constant->getSExtValue()); // min
				constantRange.push_back(constant->getSExtValue()); // max
				vector<int> intersection = intersect(variableRange, constantRange);
				if (!intersection.empty()) {
					falseMap[variable->getName().str()] = intersection;
				}
				_outgoingFalseEdge->setNewFacts(false, false, falseMap);
				*_outgoingTrueEdge = *_incomingEdge;
			}
			else if( ((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == lhs) || ((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == rhs) ) { // X > C
				vector<int> constantRangeTrue;
				vector<int> constantRangeFalse;
						
				trueMap.erase(variable->getName().str());				
				constantRangeTrue.push_back(constant->getSExtValue()+1); // min
				constantRangeTrue.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max

				falseMap.erase(variable->getName().str());				
				constantRangeFalse.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
				constantRangeFalse.push_back(constant->getSExtValue()); // max
				
				vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
				vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
				if (!intersectTrue.empty()) {
					trueMap[variable->getName().str()] = intersectTrue;
				}
				if (!intersectFalse.empty()) {
					falseMap[variable->getName().str()] = intersectFalse;
				}
				_outgoingTrueEdge->setNewFacts(false, false, trueMap);
				_outgoingFalseEdge->setNewFacts(false, false, falseMap);
			}
			else if( ((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == lhs) || ((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == rhs) ) { // X < C
				vector<int> constantRangeTrue;
				vector<int> constantRangeFalse;
						
				trueMap.erase(variable->getName().str());				
				constantRangeTrue.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
				constantRangeTrue.push_back(constant->getSExtValue()-1); // max

				falseMap.erase(variable->getName().str());				
				constantRangeFalse.push_back(constant->getSExtValue()); // min
				constantRangeFalse.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max
				
				vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
				vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
				if (!intersectTrue.empty()) {
					trueMap[variable->getName().str()] = intersectTrue;
				}
				if (!intersectFalse.empty()) {
					falseMap[variable->getName().str()] = intersectFalse;
				}
				_outgoingTrueEdge->setNewFacts(false, false, trueMap);
				_outgoingFalseEdge->setNewFacts(false, false, falseMap);
			}
			else if( ((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == lhs) || ((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == rhs) ) { // X >= C
				vector<int> constantRangeTrue;
				vector<int> constantRangeFalse;
						
				trueMap.erase(variable->getName().str());				
				constantRangeTrue.push_back(constant->getSExtValue()); // min
				constantRangeTrue.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max

				falseMap.erase(variable->getName().str());				
				constantRangeFalse.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
				constantRangeFalse.push_back(constant->getSExtValue()-1); // max
				
				vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
				vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
				if (!intersectTrue.empty()) {
					trueMap[variable->getName().str()] = intersectTrue;
				}
				if (!intersectFalse.empty()) {
					falseMap[variable->getName().str()] = intersectFalse;
				}
				_outgoingTrueEdge->setNewFacts(false, false, trueMap);
				_outgoingFalseEdge->setNewFacts(false, false, falseMap);
			}
			else if( ((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == lhs) || ((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == rhs) ) { // X <= C
				vector<int> constantRangeTrue;
				vector<int> constantRangeFalse;
						
				trueMap.erase(variable->getName().str());				
				constantRangeTrue.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
				constantRangeTrue.push_back(constant->getSExtValue()); // max

				falseMap.erase(variable->getName().str());				
				constantRangeFalse.push_back(constant->getSExtValue()+1); // min
				constantRangeFalse.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max
				
				vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
				vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
				if (!intersectTrue.empty()) {
					trueMap[variable->getName().str()] = intersectTrue;
				}
				if (!intersectFalse.empty()) {
					falseMap[variable->getName().str()] = intersectFalse;
				}
				_outgoingTrueEdge->setNewFacts(false, false, trueMap);
				_outgoingFalseEdge->setNewFacts(false, false, falseMap);
			}
		}
		else if (variable && !range.empty()) {
			vector<int> variableRange = tryGetRange(variable);
			if(predicate == CmpInst::ICMP_EQ) { // X == Y, Y->(min,max)
				trueMap.erase(variable->getName().str());
				vector<int> intersection = intersect(variableRange, range);
				if (!intersection.empty()) {
					trueMap[variable->getName().str()] = intersection;
				}
				_outgoingTrueEdge->setNewFacts(false, false, trueMap);
				*_outgoingFalseEdge = *_incomingEdge;
			}
			else if(predicate == CmpInst::ICMP_NE) { // X != Y, Y->(min,max)
				falseMap.erase(variable->getName().str());
				vector<int> intersection = intersect(variableRange, range);
				if (!intersection.empty()) {
					falseMap[variable->getName().str()] = intersection;
				}
				_outgoingFalseEdge->setNewFacts(false, false, falseMap);
				*_outgoingTrueEdge = *_incomingEdge;
			}
			else if( ((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == lhs) || ((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == rhs) ) { // X > Y, Y->(min,max)
				vector<int> constantRangeTrue;
				vector<int> constantRangeFalse;
						
				trueMap.erase(variable->getName().str());				
				constantRangeTrue.push_back(range[0]+1); // min
				constantRangeTrue.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max

				falseMap.erase(variable->getName().str());				
				constantRangeFalse.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
				constantRangeFalse.push_back(range[1]); // max
				
				vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
				vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
				if (!intersectTrue.empty()) {
					trueMap[variable->getName().str()] = intersectTrue;
				}
				if (!intersectFalse.empty()) {
					falseMap[variable->getName().str()] = intersectFalse;
				}
				_outgoingTrueEdge->setNewFacts(false, false, trueMap);
				_outgoingFalseEdge->setNewFacts(false, false, falseMap);
			}
			else if( ((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == lhs) || ((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == rhs) ) { // X < Y, Y->(min,max)
				vector<int> constantRangeTrue;
				vector<int> constantRangeFalse;
						
				trueMap.erase(variable->getName().str());				
				constantRangeTrue.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
				constantRangeTrue.push_back(range[1]-1); // max

				falseMap.erase(variable->getName().str());				
				constantRangeFalse.push_back(range[0]); // min
				constantRangeFalse.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max
				
				vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
				vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
				if (!intersectTrue.empty()) {
					trueMap[variable->getName().str()] = intersectTrue;
				}
				if (!intersectFalse.empty()) {
					falseMap[variable->getName().str()] = intersectFalse;
				}
				_outgoingTrueEdge->setNewFacts(false, false, trueMap);
				_outgoingFalseEdge->setNewFacts(false, false, falseMap);	
			}
			else if( ((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == lhs) || ((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == rhs) ) { // X >= Y, Y->(min,max)
				vector<int> constantRangeTrue;
				vector<int> constantRangeFalse;
						
				trueMap.erase(variable->getName().str());				
				constantRangeTrue.push_back(range[0]); // min
				constantRangeTrue.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max

				falseMap.erase(variable->getName().str());				
				constantRangeFalse.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
				constantRangeFalse.push_back(range[1]-1); // max
				
				vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
				vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
				if (!intersectTrue.empty()) {
					trueMap[variable->getName().str()] = intersectTrue;
				}
				if (!intersectFalse.empty()) {
					falseMap[variable->getName().str()] = intersectFalse;
				}
				_outgoingTrueEdge->setNewFacts(false, false, trueMap);
				_outgoingFalseEdge->setNewFacts(false, false, falseMap);	
			}
			else if( ((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == lhs) || ((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == rhs) ) { // X <= Y, Y->(min,max)
				vector<int> constantRangeTrue;
				vector<int> constantRangeFalse;
						
				trueMap.erase(variable->getName().str());				
				constantRangeTrue.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
				constantRangeTrue.push_back(range[1]); // max

				falseMap.erase(variable->getName().str());				
				constantRangeFalse.push_back(range[0]+1); // min
				constantRangeFalse.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max
				
				vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
				vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
				if (!intersectTrue.empty()) {
					trueMap[variable->getName().str()] = intersectTrue;
				}
				if (!intersectFalse.empty()) {
					falseMap[variable->getName().str()] = intersectFalse;
				}
				_outgoingTrueEdge->setNewFacts(false, false, trueMap);
				_outgoingFalseEdge->setNewFacts(false, false, falseMap);	
			}
		}
		else { //both operands are ranges
			range = rhsRange;
			variable = lhs; // assign LHS first
			for (int i=0; i < 2; i++) { //complete utter hack, just perform the variable and range code twice with interchanged assignment to lhs/rhs
				vector<int> variableRange = tryGetRange(variable);
				if(predicate == CmpInst::ICMP_EQ) { // X == Y, Y->(min,max)
					trueMap.erase(variable->getName().str());
					vector<int> intersection = intersect(variableRange, range);
					if (!intersection.empty()) {
						trueMap[variable->getName().str()] = intersection;
					}
					_outgoingTrueEdge->setNewFacts(false, false, trueMap);
					*_outgoingFalseEdge = *_incomingEdge;
				}
				else if(predicate == CmpInst::ICMP_NE) { // X != Y, Y->(min,max)
					falseMap.erase(variable->getName().str());
					vector<int> intersection = intersect(variableRange, range);
					if (!intersection.empty()) {
						falseMap[variable->getName().str()] = intersection;
					}
					_outgoingFalseEdge->setNewFacts(false, false, falseMap);
					*_outgoingTrueEdge = *_incomingEdge;
				}
				else if( ((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == lhs) || ((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == rhs) ) { // X > Y, Y->(min,max)
					vector<int> constantRangeTrue;
					vector<int> constantRangeFalse;
						
					trueMap.erase(variable->getName().str());				
					constantRangeTrue.push_back(range[0]+1); // min
					constantRangeTrue.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max

					falseMap.erase(variable->getName().str());				
					constantRangeFalse.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
					constantRangeFalse.push_back(range[1]); // max
				
					vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
					vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
					if (!intersectTrue.empty()) {
						trueMap[variable->getName().str()] = intersectTrue;
					}
					if (!intersectFalse.empty()) {
						falseMap[variable->getName().str()] = intersectFalse;
					}
					_outgoingTrueEdge->setNewFacts(false, false, trueMap);
					_outgoingFalseEdge->setNewFacts(false, false, falseMap);
				}
				else if( ((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == lhs) || ((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == rhs) ) { // X < Y, Y->(min,max)
					vector<int> constantRangeTrue;
					vector<int> constantRangeFalse;
						
					trueMap.erase(variable->getName().str());				
					constantRangeTrue.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
					constantRangeTrue.push_back(range[1]-1); // max

					falseMap.erase(variable->getName().str());				
					constantRangeFalse.push_back(range[0]); // min
					constantRangeFalse.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max
				
					vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
					vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
					if (!intersectTrue.empty()) {
						trueMap[variable->getName().str()] = intersectTrue;
					}
					if (!intersectFalse.empty()) {
						falseMap[variable->getName().str()] = intersectFalse;
					}
					_outgoingTrueEdge->setNewFacts(false, false, trueMap);
					_outgoingFalseEdge->setNewFacts(false, false, falseMap);	
				}
				else if( ((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == lhs) || ((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == rhs) ) { // X >= Y, Y->(min,max)
					vector<int> constantRangeTrue;
					vector<int> constantRangeFalse;
						
					trueMap.erase(variable->getName().str());				
					constantRangeTrue.push_back(range[0]); // min
					constantRangeTrue.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max

					falseMap.erase(variable->getName().str());				
					constantRangeFalse.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
					constantRangeFalse.push_back(range[1]-1); // max
				
					vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
					vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
					if (!intersectTrue.empty()) {
						trueMap[variable->getName().str()] = intersectTrue;
					}
					if (!intersectFalse.empty()) {
						falseMap[variable->getName().str()] = intersectFalse;
					}
					_outgoingTrueEdge->setNewFacts(false, false, trueMap);
					_outgoingFalseEdge->setNewFacts(false, false, falseMap);	
				}
				else if( ((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == lhs) || ((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == rhs) ) { // X <= Y, Y->(min,max)
					vector<int> constantRangeTrue;
					vector<int> constantRangeFalse;
						
					trueMap.erase(variable->getName().str());				
					constantRangeTrue.push_back(APInt::getSignedMinValue(32).getSExtValue()); // min
					constantRangeTrue.push_back(range[1]); // max

					falseMap.erase(variable->getName().str());				
					constantRangeFalse.push_back(range[0]+1); // min
					constantRangeFalse.push_back(APInt::getSignedMaxValue(32).getSExtValue()); // max
				
					vector<int> intersectTrue = intersect(constantRangeTrue, variableRange);				
					vector<int> intersectFalse = intersect(constantRangeFalse, variableRange);			
					if (!intersectTrue.empty()) {
						trueMap[variable->getName().str()] = intersectTrue;
					}
					if (!intersectFalse.empty()) {
						falseMap[variable->getName().str()] = intersectFalse;
					}
					_outgoingTrueEdge->setNewFacts(false, false, trueMap);
					_outgoingFalseEdge->setNewFacts(false, false, falseMap);	
				}
				range = lhsRange;
				variable = rhs; // assign rhs side on next pass
			}
		}
	}

}

ConstantInt * RangeAnalysis::tryGetConstantValue(Value * value) {
    ConstantInt * CI = dyn_cast<ConstantInt>(value);
    if (CI && CI->getBitWidth() <= 32) {
        return CI;
    }
    return NULL;
}

vector<int> RangeAnalysis::tryGetRange(Value * value) {
    map<string,vector<int> > edgeMap = _incomingEdge->getFacts();
    if( edgeMap.count(value->getName().str()) > 0 )
		return edgeMap[value->getName().str()];
	vector<int> empty;
    return empty;
}

vector<int> RangeAnalysis::intersect(vector<int> left, vector<int> right) {
	vector<int> result;
	if (left.empty())
		result = left;
	else if (right.empty())
		result = right;
	else {
		int varmin = max(left[0], right[0]);
		int varmax = min(left[1], right[1]);
		if (varmin <= varmax) { // there is an intersection of values
        	result.push_back(varmin);
			result.push_back(varmax);
		}
	}
	return result;
		
}


