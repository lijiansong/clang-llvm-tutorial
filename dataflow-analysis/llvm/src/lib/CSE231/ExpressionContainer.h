#include "Expression.h"

#include <map>
#include <vector>
#include <string>

using namespace llvm;
using namespace std;

struct expressionComp : public binary_function<Expression*,Expression*,bool> {
     bool operator()(Expression * const a, Expression * const b) {
            return a->isEqualTo(b);
     }
};

class ExpressionContainer {
public:
    ExpressionContainer();

    void addExpression(string variable, Expression * expression);
    vector<string> getVariablesFor(Expression * expression);
    map<string,Expression *> getMap();
    map<Expression*,vector<string>,expressionComp> getExpressionMap();
    void dump();

private:
    map<string, Expression*> _expressionMap;
};

