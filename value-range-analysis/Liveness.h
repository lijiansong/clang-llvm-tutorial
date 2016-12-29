//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IntrinsicInst.h"

#include "Dataflow.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <map>
#include <set>
using namespace llvm;
using namespace std;
#define MINUS_INF -0x7ffffff
#define INF 0x7ffffff

typedef pair < map < string, vector < int >>, map < string, vector < int >>>true_false_out_pair;

struct LivenessInfo
{
	std::set < Instruction * >LiveVars;	/// Set of variables which are live

	// interval of each vars,name of each vars is the key, and vector<int> is its range 
	map < string, vector < int >>VarRanges;
	//whether this block is reacheable by judging its range
	bool isReachable;

	//map < BasicBlock *, BasicBlock * > BackEdge;
	// map<string,vector<int> > TrueOut;
	// map<string,vector<int> > FalseOut;

	  LivenessInfo ():LiveVars (), VarRanges ()	,isReachable(true) /*,TrueOut(),FalseOut() */
	{
	}
	LivenessInfo (const LivenessInfo & info):LiveVars (info.LiveVars)
	{
	}

	bool operator == (const LivenessInfo & info) const
	{
		return LiveVars == info.LiveVars;
	}
};

inline raw_ostream & operator<< (raw_ostream & out, const LivenessInfo & info)
{
	for (std::set < Instruction * >::iterator ii = info.LiveVars.begin (), ie = info.LiveVars.end (); ii != ie; ++ii)
	{
		const Instruction *inst = *ii;
		out << inst->getName ();
		out << " ";
	}
	return out;
}

class LivenessVisitor:public DataflowVisitor < struct LivenessInfo >
{
  public:
	LivenessVisitor ()
	{
	}

	void merge (LivenessInfo * dest, const LivenessInfo & src,BasicBlock *current_bb,typename DataflowResult<LivenessInfo>::Type * result) override
	{
		//merge
		auto it = src.VarRanges.begin ();
		auto ie = src.VarRanges.end ();
		for (; it != ie; ++it)
		{
			//this means the basic block already get the condition range from its pred block
			if((*result)[current_bb].first.VarRanges.count((*it).first)>0)
			{
				continue;
			}
			else
			{
				if (dest->VarRanges.count ((*it).first) > 0)
				{
					join (dest->VarRanges[(*it).first], (*it).second);
				}
				else
				{
					dest->VarRanges.insert (pair < string, vector < int > >((*it).first, (*it).second));
				}
			}
		}
	}

	vector < int >join (vector < int >left, vector < int >right)
	{
		vector < int >result;
		if (left.empty ())
			result = right;
		else if (right.empty ())
			result = left;
		else if (!left.empty () && !right.empty ())
		{
			if (left.size () == 2 && right.size () == 2)
			{
				//merge
				if (!intersect (left, right).empty ())
				{
					int resultMin = min (left[0], right[0]);
					int resultMax = max (left[1], right[1]);
					result.push_back (resultMin);
					result.push_back (resultMax);
				}
				else if (left[1] + 1 == right[0])
				{
					result.push_back (left[0]);
					result.push_back (right[1]);
				}
				else if (right[1] + 1 == left[0])
				{
					result.push_back (right[0]);
					result.push_back (left[1]);
				}
				else
				{
					result.push_back (left[0]);
					result.push_back (left[1]);
					result.push_back (right[0]);
					result.push_back (right[1]);
				}
			}
			else if (left.size () == 4 && right.size () == 2)
			{
				//todo:
				vector < int >tmp_left;
				tmp_left.push_back (left[0]);
				tmp_left.push_back (left[1]);
				vector < int >tmp_result = join (tmp_left, right);
				tmp_left.clear ();
				tmp_left.push_back (left[2]);
				tmp_left.push_back (left[3]);
				result = join (tmp_left, tmp_result);
			}
			else if (left.size () == 2 && right.size () == 4)
			{
				//todo
				vector < int >tmp_right;
				tmp_right.push_back (right[0]);
				tmp_right.push_back (right[1]);
				vector < int >tmp_result = join (tmp_right, left);
				tmp_right.clear ();
				tmp_right.push_back (right[2]);
				tmp_right.push_back (right[3]);
				result = join (tmp_right, tmp_result);
			}
			else
			{
				//todo: other cases
			}
		}
		return result;
	}

	ConstantInt *tryGetConstantValue (Value * value)
	{
		ConstantInt *CI = dyn_cast < ConstantInt > (value);
		if (CI && CI->getBitWidth () <= 32)
		{
			return CI;
		}
		return NULL;
	}

	vector < int >tryGetRange (Value * value, LivenessInfo * dfval)
	{
		map < string, vector < int >>varRanges = dfval->VarRanges;
		if (varRanges.count (value->getName ().str ()) > 0)
			return varRanges[value->getName ().str ()];
		vector < int >empty;
		// //assign [-inf,inf]
		empty.push_back (MINUS_INF);
		empty.push_back (INF);
		return empty;
	}

	//intersect operation 
	vector < int >intersect (vector < int >left, vector < int >right)
	{
		vector < int >result;
		if (left.empty ())
			result = left;
		else if (right.empty ())
			result = right;
		else
		{
			int varmin = max (left[0], right[0]);
			int varmax = min (left[1], right[1]);
			if (varmin <= varmax)
			{
				result.push_back (varmin);
				result.push_back (varmax);
			}
		}
		return result;
	}

	//cut right from left
	vector < int >cut (vector < int >left, vector < int >right)
	{
		vector < int >result;
		vector <int> empty;
		if(!intersect(left,right).empty())
		{
			if (left[0] <= right[0] && right[1] <= left[1])
			{
				int tmp_left = right[0] - left[0];
				int tmp_right = left[1] - right[1];
				if (tmp_left >= 1 && tmp_right >= 1)
				{
					result.push_back (left[0]);
					result.push_back (right[0] - 1);
					result.push_back (right[1] + 1);
					result.push_back (left[1]);
				}
				else if(tmp_left==0 && tmp_right>=1)
				{
					result.push_back (right[1] + 1);
					result.push_back (left[1]);
				}
				else if(tmp_left>=1 && tmp_right==0)
				{
					result.push_back (left[0]);
					result.push_back (right[0] - 1);
				}
				else if(tmp_left==0 && tmp_right==0)
				{
					result=empty;
				}
			}
			else
			{
				errs () << "*****cut: other cases" << "\n";
			}
		}
		else
		{
			result=empty;
		}
		
		return result;
	}

	//handle the icmpinst of the pred_bb's pred basic block
	void handlePredIcmp ( /*const */ BasicBlock * pred_bb,
		/*BasicBlock *bb, */
		typename DataflowResult < LivenessInfo >::Type * result) override
	{
		//for each intruction in the basic block
		BasicBlock::iterator ii = pred_bb->begin (), ie = pred_bb->end ();
		for (; ii != ie; ++ii)
		{
			Instruction *inst = dyn_cast < Instruction > (ii);
			if (isa < BranchInst > (inst))
			{
				BranchInst *br_inst = dyn_cast < BranchInst > (inst);
				if (br_inst->isConditional ())
				{
					Value *condition = br_inst->getCondition ();
					//ICmpInst * cmpInst = (ICmpInst *)condition;
					ICmpInst *cmpInst = dyn_cast < ICmpInst > (condition);

					int predicate = cmpInst->isSigned ()? cmpInst->getSignedPredicate () : cmpInst->getUnsignedPredicate ();
					//get the lhs and the rhs
					Value *lhs = cmpInst->getOperand (0);
					Value *rhs = cmpInst->getOperand (1);
					ConstantInt *rhsConstant = tryGetConstantValue (rhs);
					ConstantInt *lhsConstant = tryGetConstantValue (lhs);
					

					//range is empty
					vector < int >range;

					Value *variable = NULL;
					ConstantInt *constant = NULL;

					//both lhs and rhs is constant
					if ((lhsConstant && rhsConstant) /*|| (lhsRange.empty() && rhsRange.empty()) */ )
					{
						errs () << "both lhs and rhs is constant!" << "\n";
						return;
					}
					//rhs is a constant int, but lhs is not
					else if (rhsConstant)
					{
						variable = lhs;
						constant = rhsConstant;
					}
					//lhs is a constant int, but rhs is not
					else if (lhsConstant)
					{
						variable = rhs;
						constant = lhsConstant;
					}
					//both variables
					else
					{
						variable = NULL;
					}
					// else
					// {
					//     //both have ranges, we have to assign new may ranges to both variables!
					//     if (!lhsRange.empty() && !rhsRange.empty()) 
					//     {
					//         variable = NULL;
					//     }
					//     // assign to rhs, since it has no range
					//     else if(!lhsRange.empty())
					//     {
					//         range=lhsRange;
					//         variable=rhs;
					//     }
					//     //assign to lhs, since it has no range
					//     else if(!rhsRange.empty())
					//     {
					//         range=rhsRange;
					//         variable=lhs;
					//     }
					// }//end of else

					BasicBlock *trueBlock = br_inst->getSuccessor (0);
					BasicBlock *falseBlock = br_inst->getSuccessor (1);
					//process each probably case
					if (variable && constant)
					{
						vector < int >variableRange = tryGetRange (variable, &((*result)[pred_bb].second));
						// X == C
						if (predicate == CmpInst::ICMP_EQ)
						{
							vector < int >constantRange;
							map < string, vector < int >>*trueMap = &(((*result)[pred_bb].second).VarRanges);
							if (trueMap->count (variable->getName ().str ()) > 0)
							{
								//trueMap->erase (variable->getName ().str ());
								constantRange.push_back (constant->getSExtValue ());
								constantRange.push_back (constant->getSExtValue ());
								vector < int >intersection = intersect (variableRange, constantRange);
								if (!intersection.empty ())
								{
									//(*result)[trueBlock].first.VarRanges[variable->getName().str()]=intersection;
									(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersection));
								}
								else
								{
									//unreachable
									(*result)[trueBlock].first.isReachable=false;
								}
								vector < int >_cut = cut (variableRange, constantRange);
								if (!_cut.empty())
								{
									(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), _cut));
								}
								else
								{
									(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), variableRange));
								}
								
								//(*result)[trueBlock].first.VarRanges
							}
							//[-inf,inf]
							else
							{
							}
							trueMap = NULL;
						}
						// X != C
						else if (predicate == CmpInst::ICMP_NE)
						{
							vector < int >constantRange;
							//map < string, vector < int >>*falseMap = &(((*result)[pred_bb].second).VarRanges);
							//falseMap->erase (variable->getName ().str ());
							constantRange.push_back (constant->getSExtValue ());
							constantRange.push_back (constant->getSExtValue ());
							vector < int >intersection = intersect (variableRange, constantRange);
							vector < int >_cut = cut (variableRange, constantRange);
							//todo: may be wrong
							if(!_cut.empty())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), _cut));
							}
							else
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), variableRange));
							}

							if (!intersection.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersection));
							}
							else
							{
								(*result)[falseBlock].first.isReachable=false;
							}
							//vector < int >_cut = cut (variableRange, constantRange);
							(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), _cut));
							//falseMap = NULL;

						}
						// X > C
						else if (((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == lhs)
							|| ((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == rhs))
						{
							vector < int >constantRangeTrue;
							vector < int >constantRangeFalse;
							//map < string, vector < int >>*falseMap = &(((*result)[pred_bb].second).VarRanges);
							//falseMap->erase (variable->getName ().str ());
							constantRangeTrue.push_back (constant->getSExtValue () + 1);	// min
							constantRangeTrue.push_back (INF /*APInt::getSignedMaxValue(32).getSExtValue() */ );	// max

							//falseMap.erase(variable->getName().str());        
							constantRangeFalse.push_back (MINUS_INF /*APInt::getSignedMinValue(32).getSExtValue() */ );	// min
							constantRangeFalse.push_back (constant->getSExtValue ());	// max

							vector < int >intersectTrue = intersect (constantRangeTrue, variableRange);
							vector < int >intersectFalse = intersect (constantRangeFalse, variableRange);
							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							else
							{
								//unreachable
								(*result)[trueBlock].first.isReachable=false;
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}
							else
							{
								//unreachable
								(*result)[falseBlock].first.isReachable=false;
							}
							//falseMap = NULL;

						}
						// X < C
						else if (((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == lhs)
							|| ((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == rhs))
						{
							vector < int >constantRangeTrue;
							vector < int >constantRangeFalse;
							//map < string, vector < int >>*falseMap = &(((*result)[pred_bb].second).VarRanges);
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (MINUS_INF /*APInt::getSignedMinValue(32).getSExtValue() */ );	// min
							constantRangeTrue.push_back (constant->getSExtValue () - 1);	// max

							constantRangeFalse.push_back (constant->getSExtValue ());	// min
							constantRangeFalse.push_back (INF /*APInt::getSignedMaxValue(32).getSExtValue() */ );	// max

							vector < int >intersectTrue = intersect (constantRangeTrue, variableRange);
							vector < int >intersectFalse = intersect (constantRangeFalse, variableRange);
							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							else
							{
								(*result)[trueBlock].first.isReachable=false;
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}
							else
							{
								(*result)[falseBlock].first.isReachable=false;
							}
							//falseMap = NULL;
						}
						// X >= C
						else if (((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == lhs)
							|| ((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == rhs))
						{
							vector < int >constantRangeTrue;
							vector < int >constantRangeFalse;

							//map < string, vector < int >>*falseMap = &(((*result)[pred_bb].second).VarRanges);
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (constant->getSExtValue ());	// min
							constantRangeTrue.push_back (INF /*APInt::getSignedMaxValue(32).getSExtValue() */ );	// max

							constantRangeFalse.push_back (MINUS_INF /*APInt::getSignedMinValue(32).getSExtValue() */ );	// min
							constantRangeFalse.push_back (constant->getSExtValue () - 1);	// max

							vector < int >intersectTrue = intersect (constantRangeTrue, variableRange);
							vector < int >intersectFalse = intersect (constantRangeFalse, variableRange);
							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							else
							{
								(*result)[trueBlock].first.isReachable=false;
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}
							else
							{
								(*result)[falseBlock].first.isReachable=false;
							}
							//falseMap = NULL;
						}
						// X <= C
						else if (((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == lhs)
							|| ((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == rhs))
						{
							vector < int >constantRangeTrue;
							vector < int >constantRangeFalse;

							//map < string, vector < int >>*falseMap = &(((*result)[pred_bb].second).VarRanges);
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (MINUS_INF /*APInt::getSignedMinValue(32).getSExtValue() */ );	// min
							constantRangeTrue.push_back (constant->getSExtValue ());	// max

							constantRangeFalse.push_back (constant->getSExtValue () + 1);	// min
							constantRangeFalse.push_back (INF /*APInt::getSignedMaxValue(32).getSExtValue() */ );	// max

							vector < int >intersectTrue = intersect (constantRangeTrue, variableRange);
							vector < int >intersectFalse = intersect (constantRangeFalse, variableRange);
							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							else
							{
								(*result)[trueBlock].first.isReachable=false;
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}
							else
							{
								(*result)[falseBlock].first.isReachable=false;
							}
							//falseMap = NULL;
						}
						
					}//end of if variable && constant
					// else if (variable && !range.empty())
					// {
					// }
					//both operands are ranges
					else
					{
						vector < int >lhsRange = tryGetRange (lhs, &((*result)[pred_bb].second));
						vector < int >rhsRange = tryGetRange (rhs, &((*result)[pred_bb].second));
						//assign LHS first
						range = rhsRange;
						variable = lhs;
						vector < int >variableRange = tryGetRange (variable, &((*result)[pred_bb].second));
						// X==Y
						if (predicate == CmpInst::ICMP_EQ)
						{
						}
						//X!=Y
						else if (predicate == CmpInst::ICMP_NE)
						{
						}
						// X > Y, Y->(min,max)
						else if (((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == lhs) || ((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT)
								&& variable == rhs))
						{
							vector < int >constantRangeTrue;
							vector < int >constantRangeFalse;

							//map < string, vector < int >>*falseMap = &(((*result)[pred_bb].second).VarRanges);
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (range[0] + 1);	// min
							constantRangeTrue.push_back (INF /*APInt::getSignedMaxValue(32).getSExtValue() */ );	// max

							constantRangeFalse.push_back (MINUS_INF /*APInt::getSignedMinValue(32).getSExtValue() */ );	// min
							constantRangeFalse.push_back (range[1]);	// max

							vector < int >intersectTrue = intersect (constantRangeTrue, variableRange);
							vector < int >intersectFalse = intersect (constantRangeFalse, variableRange);
							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}

							range = lhsRange;
							variable = rhs;
							variableRange.clear ();
							variableRange = tryGetRange (variable, &((*result)[pred_bb].second));
							constantRangeTrue.clear ();
							constantRangeFalse.clear ();
							intersectTrue.clear ();
							intersectFalse.clear ();
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (MINUS_INF /*APInt::getSignedMinValue(32).getSExtValue() */ );	// min
							constantRangeTrue.push_back (range[1] - 1);	// max

							constantRangeFalse.push_back (range[0]);	// min
							constantRangeFalse.push_back (INF /*APInt::getSignedMaxValue(32).getSExtValue() */ );	// max

							intersectTrue = intersect (constantRangeTrue, variableRange);
							intersectFalse = intersect (constantRangeFalse, variableRange);

							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}
							//falseMap = NULL;

						}
						// X < Y,
						else if (((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == lhs) || ((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT)
								&& variable == rhs))
						{
							vector < int >constantRangeTrue;
							vector < int >constantRangeFalse;

							//map < string, vector < int >>*falseMap = &(((*result)[pred_bb].second).VarRanges);
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (MINUS_INF /*APInt::getSignedMinValue(32).getSExtValue() */ );
							constantRangeTrue.push_back (range[1] - 1);	

							constantRangeFalse.push_back (range[0]);
							constantRangeFalse.push_back (INF /*APInt::getSignedMaxValue(32).getSExtValue() */ );     

							vector < int >intersectTrue = intersect (constantRangeTrue, variableRange);
							vector < int >intersectFalse = intersect (constantRangeFalse, variableRange);

							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}

							range = lhsRange;
							variable = rhs;
							variableRange.clear ();
							variableRange = tryGetRange (variable, &((*result)[pred_bb].second));
							constantRangeTrue.clear ();
							constantRangeFalse.clear ();
							intersectTrue.clear ();
							intersectFalse.clear ();
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (range[0] + 1);	// min
							constantRangeTrue.push_back (INF /*APInt::getSignedMaxValue(32).getSExtValue() */ );	// max

							constantRangeFalse.push_back (MINUS_INF /*APInt::getSignedMinValue(32).getSExtValue() */ );	// min
							constantRangeFalse.push_back (range[1]);	// max

							intersectTrue = intersect(constantRangeTrue, variableRange);        
							intersectFalse = intersect(constantRangeFalse, variableRange);      
							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}
							//falseMap = NULL;

						}
						// X >= Y, Y->(min,max)
						else if (((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == lhs) || ((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE)
								&& variable == rhs))
						{
							vector < int >constantRangeTrue;
							vector < int >constantRangeFalse;

							//map < string, vector < int >>*falseMap = &(((*result)[pred_bb].second).VarRanges);
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (range[0]);	// min
							constantRangeTrue.push_back (INF);	// max

							constantRangeFalse.push_back (MINUS_INF);	// min
							constantRangeFalse.push_back (range[1] - 1);	// max

							vector < int >intersectTrue = intersect (constantRangeTrue, variableRange);
							vector < int >intersectFalse = intersect (constantRangeFalse, variableRange);

							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}

							range = lhsRange;
							variable = rhs;
							variableRange.clear ();
							variableRange = tryGetRange (variable, &((*result)[pred_bb].second));
							constantRangeTrue.clear ();
							constantRangeFalse.clear ();
							intersectTrue.clear ();
							intersectFalse.clear ();
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (MINUS_INF);	// min
							constantRangeTrue.push_back (range[1]);	// max

							constantRangeFalse.push_back (range[0] + 1);	// min
							constantRangeFalse.push_back (INF);	// max

							intersectTrue = intersect (constantRangeTrue, variableRange);
							intersectFalse = intersect (constantRangeFalse, variableRange);
							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}
							//falseMap = NULL;
						}
						// X <= Y, Y->(min,max)
						else if (((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == lhs) || ((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE)
								&& variable == rhs))
						{
							vector < int >constantRangeTrue;
							vector < int >constantRangeFalse;

							//map < string, vector < int >>*falseMap = &(((*result)[pred_bb].second).VarRanges);
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (MINUS_INF);	// min
							constantRangeTrue.push_back (range[1]);	// max

							constantRangeFalse.push_back (range[0] + 1);	// min
							constantRangeFalse.push_back (INF);	// max

							vector < int >intersectTrue = intersect (constantRangeTrue, variableRange);
							vector < int >intersectFalse = intersect (constantRangeFalse, variableRange);

							if (!intersectTrue.empty ())
							{
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							if (!intersectFalse.empty ())
							{
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}

							range = lhsRange;
							variable = rhs;
							variableRange.clear ();
							variableRange = tryGetRange (variable, &((*result)[pred_bb].second));
							constantRangeTrue.clear ();
							constantRangeFalse.clear ();
							intersectTrue.clear ();
							intersectFalse.clear ();
							//falseMap->erase (variable->getName ().str ());

							constantRangeTrue.push_back (range[0]);	// min
							constantRangeTrue.push_back (INF);	// max

							constantRangeFalse.push_back (MINUS_INF);	// min
							constantRangeFalse.push_back (range[1] - 1);	// max

							intersectTrue = intersect (constantRangeTrue, variableRange);
							intersectFalse = intersect (constantRangeFalse, variableRange);
							if (!intersectTrue.empty ())
							{
								//trueMap[variable->getName().str()] = intersectTrue;
								(*result)[trueBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectTrue));
							}
							if (!intersectFalse.empty ())
							{
								//falseMap[variable->getName().str()] = intersectFalse;
								(*result)[falseBlock].first.VarRanges.insert (pair < string, vector < int > >(variable->getName ().str (), intersectFalse));
							}
							//falseMap = NULL;
						}//end of X <= Y

					}//end of else
				}//end of if br_inst->isConditional
			}

		}//end of for
	}

	//handle binary op,e.g.: + - * /
	void handleBinaryOp (Instruction * inst, LivenessInfo * dfval)
	{
		map < string, vector < int >>*varRanges = &(dfval->VarRanges);
		string result = inst->getOperandUse (0).getUser ()->getName ().str ();
		vector < int >resultRange;
		// if (varRanges->count (result) > 0)
		// 	varRanges->erase (result);

		ConstantInt *operandConstant1 = tryGetConstantValue (inst->getOperand (0));
		ConstantInt *operandConstant2 = tryGetConstantValue (inst->getOperand (1));

		//two constant
		if ((operandConstant1 != NULL) && (operandConstant2 != NULL))
		{
			resultRange.clear();
			int operand1 = operandConstant1->getSExtValue ();
			int operand2 = operandConstant2->getSExtValue ();
			int dest_result;
			switch (inst->getOpcode ())
			{
			case Instruction::Add:
				dest_result = operand1 + operand2;
				break;

			case Instruction::Mul:
				dest_result = operand1 * operand2;
				break;

			case Instruction::Sub:
				dest_result = operand1 - operand2;
				break;

			case Instruction::SDiv:
			case Instruction::UDiv:
				dest_result = operand1 / operand2;
				break;

			case Instruction::SRem:
			case Instruction::URem:
				break;

			case Instruction::Or:
				break;

			case Instruction::And:
				break;

			case Instruction::Xor:
				break;
			}
			resultRange.push_back (dest_result);
			resultRange.push_back (dest_result);
		}
		//left is range, right is a constant
		else if ((operandConstant1 == NULL) && (operandConstant2 != NULL))
		{
			vector < int >operandRange1 = tryGetRange (inst->getOperand (0), dfval);
			int operand2 = operandConstant2->getSExtValue ();
			switch (inst->getOpcode ())
			{
			case Instruction::Add:
				operandRange1[0] += operand2;
				operandRange1[1] += operand2;
				break;

			case Instruction::Mul:
				{
					int tmp1 = operandRange1[0] * operand2;
					int tmp2 = operandRange1[1] * operand2;
					operandRange1[0] = min (tmp1, tmp2);
					operandRange1[1] = max (tmp1, tmp2);
					break;
				}
			case Instruction::Sub:
				operandRange1[0] -= operand2;
				operandRange1[1] -= operand2;
				break;

			case Instruction::SDiv:
			case Instruction::UDiv:
				/*dest_result = operand1 / operand2; */
				break;

			case Instruction::SRem:
			case Instruction::URem:
				break;

			case Instruction::Or:
				break;

			case Instruction::And:
				break;

			case Instruction::Xor:
				break;
			}
			resultRange.clear();
			resultRange.push_back (operandRange1[0]);
			resultRange.push_back (operandRange1[1]);
		}
		//left is a constant,right is range
		else if ((operandConstant1 != NULL) && (operandConstant2 == NULL))
		{
			//vector < int >operandRange1 = tryGetRange (inst->getOperand (0), dfval);
			vector < int >operandRange2 = tryGetRange (inst->getOperand (1), dfval);
			int operand1 = operandConstant1->getSExtValue ();
			switch (inst->getOpcode ())
			{
			case Instruction::Add:
				operandRange2[0] += operand1;
				operandRange2[1] += operand1;
				break;

			case Instruction::Mul:
				{
					int tmp1 = operandRange2[0] * operand1;
					int tmp2 = operandRange2[1] * operand1;
					operandRange2[0] = min (tmp1, tmp2);
					operandRange2[1] = max (tmp1, tmp2);
					break;
				}
			case Instruction::Sub:
				operandRange2[0] -= operand1;
				operandRange2[1] -= operand1;
				break;

			case Instruction::SDiv:
			case Instruction::UDiv:
				/*dest_result = operand1 / operand2; */
				break;

			case Instruction::SRem:
			case Instruction::URem:
				break;

			case Instruction::Or:
				break;

			case Instruction::And:
				break;

			case Instruction::Xor:
				break;
			}
			resultRange.clear();
			resultRange.push_back (operandRange2[0]);
			resultRange.push_back (operandRange2[1]);
		}
		//two ranges
		else if ((operandConstant1 == NULL) && (operandConstant2 == NULL))
		{
			vector < int >operandRange1 = tryGetRange (inst->getOperand (0), dfval);
			vector < int >operandRange2 = tryGetRange (inst->getOperand (1), dfval);
			int resultMin;
			int resultMax;
			switch (inst->getOpcode ())
			{
			case Instruction::Add:
				resultMin = operandRange1[0] + operandRange2[0];
				resultMax = operandRange1[1] + operandRange2[1];
				break;

			case Instruction::Mul:
			{
				//the same operand
				string left=inst->getOperand(0)->getName().str();
				string right=inst->getOperand(1)->getName().str();
				if ( strcmp(left.c_str(),right.c_str())==0 )
				{
					if((operandRange1[0] <= 0) && (operandRange1[1] <= 0) && (operandRange2[0] <= 0) && (operandRange2[1] <= 0))
					{
						resultMin = operandRange1[1] * operandRange2[1];
						resultMax = operandRange1[0] * operandRange2[0];
					}
					else if (operandRange1[0] <= 0 && operandRange1[1] >= 0 && operandRange2[0] <= 0 && operandRange2[1] >= 0)
					{
						resultMin = 0;
						resultMax = max (operandRange1[1] * operandRange2[1], operandRange1[0] * operandRange2[0]);
					}
					else if (operandRange1[0] >= 0 && operandRange1[1] >= 0 && operandRange2[0] >= 0 && operandRange2[1] >= 0)
					{
						resultMin = operandRange1[0] * operandRange2[0];
						resultMax = operandRange1[1] * operandRange2[1];
					}
				}//end of if
				else
				{
					vector<int> tmp;
					tmp.push_back(operandRange1[0] * operandRange2[0]);
					tmp.push_back(operandRange1[0] * operandRange2[1]);
					tmp.push_back(operandRange1[1] * operandRange2[0]);
					tmp.push_back(operandRange1[1] * operandRange2[1]);
					sort(tmp.begin(),tmp.end());
					resultMin=*(tmp.begin());
					resultMax=*(tmp.end()-1);
				}//end of else
				break;
			}

			case Instruction::Sub:
				if ((inst->getOperand (0)) == (inst->getOperand (1)))
				{
					resultMin = operandRange1[0] - operandRange2[0];
					resultMax = operandRange1[1] - operandRange2[1];
				}
				else
				{
					resultMin = operandRange1[0] - operandRange2[1];
					resultMax = operandRange1[1] - operandRange2[0];
				}
				break;

			case Instruction::SDiv:
			case Instruction::UDiv:
				break;

			case Instruction::SRem:
			case Instruction::URem:
				break;

			case Instruction::Or:
				break;

			case Instruction::And:
				break;

			case Instruction::Xor:
				break;
			}//end of switch
			resultRange.clear();
			resultRange.push_back (resultMin);
			resultRange.push_back (resultMax);
		}//end of else if

		//varRanges[result]=resultRange;
		varRanges->insert (pair < string, vector < int > >(result, resultRange));
		varRanges = NULL;
	}

	vector<int> processLoop(map<BasicBlock *, BasicBlock * > &back_edge,set<BasicBlock *> &loop_list)
	{
		vector<int> result;
		int left,right;
		int bound=0;
		int symbol;
		auto it=loop_list.begin(),ie=loop_list.end();
		for(;it!=ie;++it)
		{
			BasicBlock *bb=*it;
			//for each intruction in the basic block
			BasicBlock::iterator inst_it = bb->begin (), inst_ie = bb->end ();
			for (; inst_it != inst_ie; ++inst_it)
			{
				Instruction *inst = dyn_cast < Instruction > (inst_it);
				if (isa < DbgInfoIntrinsic > (inst))
					continue;
				if (isa < BranchInst > (inst))
				{
					BranchInst *br_inst = dyn_cast < BranchInst > (inst);
					if (br_inst->isConditional ())
					{
						Value *condition = br_inst->getCondition ();
						ICmpInst *cmpInst = dyn_cast < ICmpInst > (condition);
						int predicate = cmpInst->isSigned ()? cmpInst->getSignedPredicate () : cmpInst->getUnsignedPredicate ();
						Value *lhs = cmpInst->getOperand (0);
						Value *rhs = cmpInst->getOperand (1);
						ConstantInt *rhsConstant = tryGetConstantValue (rhs);
						ConstantInt *lhsConstant = tryGetConstantValue (lhs);
						//range is empty
						vector < int >range;

						Value *variable = NULL;
						ConstantInt *constant = NULL;

						if ((lhsConstant && rhsConstant) /*|| (lhsRange.empty() && rhsRange.empty()) */ )
						{
						}
						else if (rhsConstant)
						{
							variable = lhs;
							constant = rhsConstant;
						}
						else if (lhsConstant)
						{
							variable = rhs;
							constant = lhsConstant;
						}
						else
						{
							variable = NULL;
						}
						bound=constant->getSExtValue();
						if (variable && constant)
						{
							// X == C
							if (predicate == CmpInst::ICMP_EQ)
							{}
							// X != C
							else if (predicate == CmpInst::ICMP_NE)
							{}
							// X > C
							else if (((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == lhs)
								|| ((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == rhs))
							{
								symbol=1;
							}
							// X < C
							else if (((predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_SLT) && variable == lhs)
								|| ((predicate == CmpInst::ICMP_UGT || predicate == CmpInst::ICMP_SGT) && variable == rhs))
							{
								symbol=2;
							}
							// X >= C
							else if (((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == lhs)
								|| ((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == rhs))
							{
								symbol=3;
							}
							// X <= C
							else if (((predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SLE) && variable == lhs)
								|| ((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_SGE) && variable == rhs))
							{
								symbol=4;
							}
						}
					}//end of if (br_inst->isConditional ())
					else
						continue;
				}
				if (inst->isBinaryOp ())
				{
					ConstantInt *operandConstant1 = tryGetConstantValue (inst->getOperand (0));
					ConstantInt *operandConstant2 = tryGetConstantValue (inst->getOperand (1));
					if ((operandConstant1 == NULL) && (operandConstant2 != NULL))
					{
						int operand2 = operandConstant2->getSExtValue ();
						switch (inst->getOpcode ())
						{
						case Instruction::Add:
						{
							if(symbol==2)
							{
								left=bound;
								right=bound+operand2-1;
							}
							else if(symbol==4)
							{
								left=bound;
								right=bound+operand2;
							}
							break;
						}
						case Instruction::Mul:
							break;
						case Instruction::Sub:
						{
							if(symbol==1)
							{
								right=bound;
								left=bound-operand2+1;
							}
							else if(symbol==3)
							{
								right=bound;
								left=bound-operand2;
							}
							break;
						}
						}		
					}
					else if ((operandConstant1 != NULL) && (operandConstant2 == NULL))
					{
						int operand1 = operandConstant1->getSExtValue ();
						switch (inst->getOpcode ())
						{
						case Instruction::Add:
						{
							if(symbol==2)
							{
								left=bound;
								right=bound+operand1-1;
							}
							else if(symbol==4)
							{
								left=bound;
								right=bound+operand1;
							}
							break;
						}

						case Instruction::Mul:
							break;
						case Instruction::Sub:
						{
							if(symbol==1)
							{
								right=bound;
								left=bound-operand1+1;
							}
							else if(symbol==3)
							{
								right=bound;
								left=bound-operand1;
							}
							break;
						}
						}
					}
				}
			}
		}
		result.push_back(left);
		result.push_back(right);
		return result;
	}
	//compute the interval of each inst
	void compDFVal (Instruction * inst, LivenessInfo * dfval, typename DataflowResult < LivenessInfo >::Type * result,map<BasicBlock *, BasicBlock * > &back_edge,set<BasicBlock *> &loop_list) override
	{
		if (isa < DbgInfoIntrinsic > (inst))
			return;
		if (isa < BranchInst > (inst))
			return;
		if (isa < ReturnInst > (inst))
		{
			ReturnInst *ret_inst = dyn_cast < ReturnInst > (inst);
			Value *ret_value = ret_inst->getReturnValue ();
			//loop
			if(back_edge.size()>0)
			{
				vector<int> ans=processLoop(back_edge,loop_list);
				if(!ans.empty())
				{
					/*errs()*/cout<<"["<<ans[0]<<".."<<ans[1]<<"]\n";
				}
			}
			else
			{
				if (dfval->VarRanges.count (ret_value->getName ().str ()) > 0)
				{
					int left = dfval->VarRanges[ret_value->getName ().str ()][0];
					int right = dfval->VarRanges[ret_value->getName ().str ()][1];
					if (left == MINUS_INF)
					{
						/*errs ()*/cout << "[min..";
					}
					else
					{
						/*errs ()*/cout << "[" << left << "..";
					}
					if (right == INF)
					{
						/*errs ()*/cout << "max]\n";
					}
					else
					{
						/*errs () */cout<< right << "]\n";
					}
				}
				else
				{
					/*errs ()*/cout << "Error: " << ret_value->getName ().str () << " doesn't exist!\n";
					/*errs ()*/cout << "************[min..max]\n";
				}
			}
		}
		
		if (inst->isBinaryOp ())
		{
			handleBinaryOp (inst, dfval);
		}
		if (isa < PHINode > (inst))
		{
			PHINode *phi_node = dyn_cast < PHINode > (inst);
			unsigned num = phi_node->getNumIncomingValues ();
			vector < int >range;
			for (unsigned i = 0; i < num; ++i)
			{
				Value *value = phi_node->getIncomingValue (i);
				BasicBlock *pred_bb = phi_node->getIncomingBlock (i);
				ConstantInt *constant = tryGetConstantValue (value);
				if (constant)
				{
					// if( ((*result)[pred_bb].first.isReachable) )
					// {
						//errs()<<constant->getSExtValue ()<<"\n";
						range.push_back (constant->getSExtValue ());
						range.push_back (constant->getSExtValue ());
					// }
					// else
					// {
					// 	continue;
					// }	
				}
				else
				{
					map < string, vector < int >>pred_map = (*result)[pred_bb].second.VarRanges;
					if (pred_map.count (value->getName ().str ()) > 0)
					{
						vector < int >tmp_range = pred_map[value->getName ().str ()];
						for (size_t i = 0; i < tmp_range.size (); ++i)
						{
							range.push_back (tmp_range[i]);
						}
					}
					else
					{
						//-inf ~ inf
						range.push_back (MINUS_INF);
						range.push_back (INF);
					}
				}//end of else
			}//end of for
			sort (range.begin (), range.end ());
			// for(size_t i=0;i<range.size();++i)
			// {
			//  errs()<<range[i]<<'\n';
			// }
			vector < int >tmp;
			tmp.push_back (*(range.begin ()));
			tmp.push_back (*(range.end () - 1));
			//errs()<<*(range.end ());
			//errs()<<phi_node->getName ().str ()<<"\n";
			// for(size_t i=0;i<tmp.size();++i)
			// {
			//  errs()<<tmp[i]<<'\n';
			// }
			dfval->VarRanges.insert (pair < string, vector < int > >(phi_node->getName ().str (), tmp));
		}						//end of if isa<PHINode>(inst)
	}
};

class Liveness:public FunctionPass
{
  public:
  	LoopInfo *loopInfo;
	static char ID;
	  Liveness ():FunctionPass (ID)
	{
	}
	virtual void getAnalysisUsage(AnalysisUsage &AU) const
	{
		AU.addRequired<LoopInfoWrapperPass>();
	}

	bool runOnFunction (Function & F) override
	{
		//F.dump();
		loopInfo = &(getAnalysis<LoopInfoWrapperPass>().getLoopInfo());
		LivenessVisitor visitor;
		DataflowResult < LivenessInfo >::Type result;
		map<BasicBlock *, BasicBlock * > back_edge;
		set<BasicBlock *> loop_list;
		LivenessInfo initval;

		//compBackwardDataflow(&F, &visitor, &result, initval);
		compForwardDataflow (&F, &visitor, &result, back_edge, loopInfo ,loop_list,initval);
		//printDataflowResult<LivenessInfo>(errs(), result);
		return false;
	}
};
