#ifndef _LATTICE_T_H
#define _LATTICE_T_H

#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/InstIterator.h"

#include <set>
#include <map>
#include <string>

using namespace llvm;
using namespace std;
    
struct valueComp {
    bool operator()(Value* const a, Value* const b) {
        return a->getName() < b->getName();
    }
};

template <class T>
class Lattice {
public:
	Lattice();
	Lattice(bool isTop, bool isBottom, T facts);
	Lattice(Lattice& other);
	Lattice& operator=(const Lattice& other);
	~Lattice();
	T getFacts();
	void setNewFacts(bool isTop, bool isBottom, T facts);
	bool isTop();
	bool isBottom();

private:
	T _facts;
	bool _isTop;
	bool _isBottom;

};

template <class T>
Lattice<T>::Lattice() {}

template <class T>
Lattice<T>::Lattice(bool isTop, bool isBottom, T facts) {
	// Must Analysis, bottom is Full set, top is empty set
	_facts = facts;
	_isTop = isTop;
	_isBottom = isBottom;
}

template <class T>
Lattice<T>::Lattice(Lattice& other) {
	_facts = other._facts;
	_isTop = other._isTop;
	_isBottom = other._isBottom;
}

template <class T>
Lattice<T>& Lattice<T>::operator=( const Lattice& other ) {
	_facts = other._facts;
	_isTop = other._isTop;
	_isBottom = other._isBottom;
	return *this;
}

template <class T>
Lattice<T>::~Lattice() {}

template <class T>
T Lattice<T>::getFacts() {
	return _facts; //returns a copy
}

template <class T>
void Lattice<T>::setNewFacts(bool isTop, bool isBottom, T facts) {
	_facts = facts;
	_isTop = isTop;
	_isBottom = isBottom;
}

template <class T>
bool Lattice<T>::isTop() {
	return _isTop;
}

template <class T>
bool Lattice<T>::isBottom() {
	return _isBottom;
}

#endif

