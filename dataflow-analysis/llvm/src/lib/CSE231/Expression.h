#include "llvm/IR/Instruction.h"
#include "llvm/IR/Value.h"

using namespace llvm;
using namespace std;

class Expression {
public:
    Expression(Instruction * instruction);
    Value * getOperand1();
    Value * getOperand2();
    string operand1Name();
    string operand2Name();
    bool isEqualTo(Expression * expression);

    void dump();

    bool isEqual(Expression * expression);

private:
    string getNameOf(Value * value);

    Instruction * _instruction;
};

/*
class UnaryExpression : public Expression {
public:
    UnaryExpression(Instruction * instruction);

    Value * getOperand();
    void dump();
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(Instruction * instruction);

    Value * getOperand1();
    Value * getOperand2();
    string operand1Name();
    string operand2Name();
    bool isEqualTo(BinaryExpression * expression);

    void dump();
};
*/

