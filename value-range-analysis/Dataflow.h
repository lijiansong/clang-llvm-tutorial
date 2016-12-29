/************************************************************************
 *
 * @file Dataflow.h
 *
 * General dataflow framework
 *
 ***********************************************************************/

#ifndef _DATAFLOW_H_
#define _DATAFLOW_H_

#include <llvm/Support/raw_ostream.h>
#include <llvm/Analysis/LoopInfo.h>
#include <map>
#include <set>

using namespace std;

using namespace llvm;

///
/// Dummy class to provide a typedef for the detailed result set
/// For each basicblock, we compute its input dataflow val and its output dataflow val
///

template<class T>
struct DataflowResult
{
    typedef typename std::map<BasicBlock *, std::pair<T, T> > Type;
};

//template<class T>
// struct DataflowBackEdge
// {
//     typedef typename std::map<BasicBlock *, BasicBlock * > BackEdge;
// };

///Base dataflow visitor class, defines the dataflow function


template <class T>
class DataflowVisitor 
{
public:
    virtual ~DataflowVisitor() { }

    /// Dataflow Function invoked for each basic block 
    /// 
    /// @block the Basic Block
    /// @dfval the input dataflow value
    /// @isforward true to compute dfval forward, otherwise backward
    //bb, &((*result)[bb].second),result,back_edge,loop_list, true
    virtual void compDFVal(BasicBlock *block, T *dfval,typename DataflowResult<T>::Type * result,map<BasicBlock *, BasicBlock * > &back_edge,set<BasicBlock *> &loop_list, bool isforward) 
    {
        if (isforward == true) 
        {
           for (BasicBlock::iterator ii=block->begin(), ie=block->end(); 
                ii!=ie; ++ii) 
           {
                Instruction * inst = &*ii;
                compDFVal(inst, dfval,result,back_edge,loop_list);
           }
        }
        else 
        {
           for (BasicBlock::reverse_iterator ii=block->rbegin(), ie=block->rend();
                ii != ie; ++ii) 
           {
                Instruction * inst = &*ii;
                compDFVal(inst, dfval,result,back_edge,loop_list);
           }
        }
    }

    ///
    /// Dataflow Function invoked for each instruction
    ///
    /// @inst the Instruction
    /// @dfval the input dataflow value
    /// @return true if dfval changed
    virtual void compDFVal(Instruction *inst, T *dfval,typename DataflowResult<T>::Type * result,map<BasicBlock *, BasicBlock * > &back_edge,set<BasicBlock *> &loop_list ) = 0;

    ///
    /// Merge of two dfvals, dest will be ther merged result
    /// @return true if dest changed
    ///
    virtual void merge( T *dest, const T &src,BasicBlock *bb,typename DataflowResult<T>::Type * result ) = 0;

    //todo: add process functions
    //virtual void merge( T *dest, const T &src ) = 0;
    //virtual void merge( T *dest, const T &src ) = 0;
    //virtual void merge( T *dest, const T &src ) = 0;
    //virtual void handlePredIcmp()
    virtual void handlePredIcmp(BasicBlock* pred_bb, typename DataflowResult<T>::Type * result) = 0;
};



/// 
/// Compute a forward iterated fixedpoint dataflow function, using a user-supplied
/// visitor function. Note that the caller must ensure that the function is
/// in fact a monotone function, as otherwise the fixedpoint may not terminate.
/// 
/// @param fn The function
/// @param visitor A function to compute dataflow vals
/// @param result The results of the dataflow 
/// @initval the Initial dataflow value
template<class T>
void compForwardDataflow( Function *fn,
                          DataflowVisitor<T> *visitor,
                          typename DataflowResult<T>::Type *result,
                          map<BasicBlock *, BasicBlock * > &back_edge,
                          LoopInfo * loopInfo,
                          set<BasicBlock *> &loop_list,
                          const T & initval);
/// 
/// Compute a backward iterated fixedpoint dataflow function, using a user-supplied
/// visitor function. Note that the caller must ensure that the function is
/// in fact a monotone function, as otherwise the fixedpoint may not terminate.
/// 
/// @param fn The function
/// @param visitor A function to compute dataflow vals
/// @param result The results of the dataflow 
/// @initval The initial dataflow value
template<class T>
void compBackwardDataflow( Function *fn, 
                           DataflowVisitor<T> *visitor,
                           typename DataflowResult<T>::Type *result,
                           map<BasicBlock *, BasicBlock * > &back_edge,
                           set<BasicBlock *> &loop_list,
                           const T &initval);

template<class T>
void printDataflowResult(raw_ostream &out,
                         const typename DataflowResult<T>::Type &dfresult) 
{
    for ( typename DataflowResult<T>::Type::const_iterator it = dfresult.begin();
            it != dfresult.end(); ++it ) 
    {
        if (it->first == NULL) out << "*";
        else it->first->dump();
        out << "\n\tin : "
            << it->second.first 
            << "\n\tout :  "
            << it->second.second
            << "\n";
    }
}

#include "Dataflow_.h"

#endif /* !_DATAFLOW_H_ */
