#include "Expression.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"


using namespace std;

#include <sstream>

template <typename T>
string NumberToString ( T Number ) {
    ostringstream ss;
    ss << Number;
    return ss.str();
}

string Expression::getNameOf(Value * value) {
    ConstantInt * CI = dyn_cast<ConstantInt>(value);
    if (CI && CI->getBitWidth() <= 32) {
        return NumberToString(CI->getSExtValue());
    }

    return value->getName().str();
}

Expression::Expression(Instruction * instruction) {
    _instruction = instruction;
}

void Expression::dump() {
    errs() << operand1Name() << " " << _instruction->getOpcodeName() << " " << operand2Name();
}

Value * Expression::getOperand1() {
    return _instruction->getOperand(0);
}

Value * Expression::getOperand2() {
    return _instruction->getOperand(1);
}

string Expression::operand1Name() {
    return getNameOf(getOperand1());
}

string Expression::operand2Name() {
    return getNameOf(getOperand2());
}

bool Expression::isEqual(Expression *  expression) {
        if (_instruction->isCommutative() && 
           (_instruction->getOpcode() == expression->_instruction->getOpcode()) &&
          ((operand1Name() == expression->operand1Name() && operand2Name() == expression->operand2Name()) ||
           (operand1Name() == expression->operand2Name() && operand2Name() == expression->operand1Name()))) {
            return true;
        }
        else if (!_instruction->isCommutative() && 
            (_instruction->getOpcode() == expression->_instruction->getOpcode()) &&
            operand1Name() == expression->operand1Name() &&
                                operand2Name() == expression->operand2Name()) {
                                        return true;
                                            }

                                                return false;
}

bool Expression::isEqualTo(Expression * expression) {
    if (_instruction->isCommutative() && _instruction->getOpcode() == expression->_instruction->getOpcode()) {
        if ((operand1Name() == expression->operand1Name() &&
             operand2Name() == expression->operand2Name()) ||
            (operand1Name() == expression->operand2Name() &&
             operand2Name() == expression->operand1Name())) {
            return false;
        }
        else {
        }
    }

    if (_instruction->getOpcode() == expression->_instruction->getOpcode()) {
        return (operand1Name() > expression->operand1Name() || operand2Name() > expression->operand2Name());
    }

    return _instruction->getOpcode() < expression->_instruction->getOpcode();
}

