#include "ExpressionContainer.h"
#include "llvm/Support/raw_ostream.h"

ExpressionContainer::ExpressionContainer() {
}

void ExpressionContainer::addExpression(string variable, Expression * expression) {
    _expressionMap[variable] = expression;    
}

map<string,Expression*> ExpressionContainer::getMap() {
    return _expressionMap;
}

vector<string> ExpressionContainer::getVariablesFor(Expression * expression) {
    vector<string> v;

    for (map<string,Expression*>::iterator i = _expressionMap.begin(); i != _expressionMap.end(); i++) {
        if (expression->isEqual(i->second)) {
            v.push_back(i->first);
        }
    }

    return v;
}

map<Expression*,vector<string>,expressionComp> ExpressionContainer::getExpressionMap() {
    map<Expression*,vector<string>,expressionComp> expMap;

    for (map<string,Expression*>::iterator i = _expressionMap.begin(); i != _expressionMap.end(); i++) {

        expMap[i->second].push_back(i->first);

        /*
        for (map<string,Expression*>::iterator j = _expressionMap.begin(); j != _expressionMap.end(); j++) {
            if (i->second->isEqualTo(j->) {
                expMap[i->second].push_back(i->first);
            }
        }
        */
    }

    return expMap;
}

void ExpressionContainer::dump() {
    map<Expression*,vector<string>,expressionComp> expMap = getExpressionMap();
    
    for (map<Expression*,vector<string>,expressionComp>::iterator i = expMap.begin(); i != expMap.end(); i++) {
        errs() << "\t\t\t\t";
        i->first->dump();
        errs() << " -> ";
        
        for (unsigned int j = 0; j < i->second.size(); j++) {
            errs() << i->second[j] << ",";
        }
        
        errs() << "\n";
    }
}

