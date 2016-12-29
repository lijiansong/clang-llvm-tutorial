/************************************************************************
 *
 * @file Dataflow.tpp
 *
 * General dataflow framework - template implementation
 *
 ***********************************************************************/

#include <map>
#include <set>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/Analysis/LoopInfo.h>
#include <vector>

//todo: all process logic to be written here
using namespace std;
void printBasicBlock(BasicBlock *bb)
{
	BasicBlock::iterator bb_it=bb->begin(),bb_ie=bb->end();
	for(;bb_it!=bb_ie;++bb_it)
	{
		Instruction *inst = dyn_cast < Instruction > (bb_it);
		errs()<<*inst<<"\n";
	}
}

//find loop header list
vector<BasicBlock*> getLoopHeaderList(LoopInfo * loopInfo,vector<BasicBlock *> worklist)
{
	vector<BasicBlock*> loopHeaderList;
	auto it=worklist.begin();
    auto ie=worklist.end();
    for( ;it != ie; ++it )
    {
    	if(loopInfo->isLoopHeader(*it))
    	{
    		loopHeaderList.push_back(*it);
    	}
    }
    return loopHeaderList;
}

// void getBackEdge(vector<BasicBlock *> worklist,BasicBlock *loopheader, map<BasicBlock *, BasicBlock * > &back_edge)
// {
// 	auto it=worklist.begin();
//     auto ie=worklist.end();
//     for( ;it != ie; ++it )
//     {
//     	BasicBlock *bb = *it;
//     	for (succ_iterator si = succ_begin(bb), se = succ_end(bb); si != se; ++si) 
// 	    {
// 	        BasicBlock *succ = *si;
// 	        if(succ==loopheader)
// 	        {
// 	        	back_edge.insert(make_pair(bb,loopheader));
// 	        }
// 	    }
//     }
// }

template<class T>
void compForwardDataflow (Function *fn,
                          DataflowVisitor<T> *visitor,
                          typename DataflowResult<T>::Type *result,
                          map<BasicBlock *, BasicBlock * > &back_edge,
                          LoopInfo * loopInfo,
                          set<BasicBlock *> &loop_list,
                          const T & initval)
{
    std::/*set*/vector<BasicBlock *> worklist;
    
    map<BasicBlock *,bool> visited;
    bool hasLoop=false;
    // Initialize the worklist with all exit blocks
    for(Function::iterator bi=fn->begin(),be=fn->end();bi!=be;++bi)
    {
        BasicBlock *bb=&*bi;
        result->insert(std::make_pair(bb,std::make_pair(initval,initval)));
        worklist.push_back(bb);
        visited[bb]=false;
        //find loop
        if(loopInfo->isLoopHeader(bb))
        {
        	hasLoop=true;
        }
        
    }

    //get the loop list
    vector<BasicBlock *> loopHeaderList=getLoopHeaderList(loopInfo,worklist);
    if(hasLoop)
    {
    	size_t num=loopHeaderList.size();
    	//errs()<<num<<"\n";
    	map<BasicBlock *,bool> tmp=visited;
    	for(size_t i=0;i<num;++i)
    	{
    		BasicBlock *bb=loopHeaderList[i];
    		auto header=find(worklist.begin(),worklist.end(),bb);
    		if(header!=worklist.end())
    		{
    			for(;header!=worklist.end();++header)
    			{
    				loop_list.insert(*header);
		    		visited[*header]=true;
		    		for(succ_iterator si = succ_begin(*header), se = succ_end(*header);si!=se;++si)
		    		{
		    			//BasicBlock *succ = *si;
		    			if(!tmp[*si])
			    			loop_list.insert(*si);
		    		}
    			}
    		}
    	}
    }
    //errs()<<loop_list.size()<<"\n";

    //find loop,it may be wrong
    auto it=worklist.begin();
    auto ie=worklist.end();
    for( ;it != ie; ++it )
    {
    	visited[*it]=false;
    }
    it=worklist.begin();
    for( ;it != ie; ++it )
    {
    	BasicBlock *bb = *it;
    	visited[bb]=true;
    	for (succ_iterator si = succ_begin(bb), se = succ_end(bb); si != se; ++si) 
	    {
	        BasicBlock *succ = *si;
	        if(visited[succ])
	        {
	        	// printBasicBlock(bb);
	        	// errs()<<"\n\n";
	        	back_edge.insert(make_pair(bb,succ));
	        }
	    }
    }
    //errs()<<back_edge.size()<<"\n";
    
    
    // Iteratively compute the dataflow result
    
    //the start block has no in dataflow, process independently
    BasicBlock *bb=*worklist.begin();
    worklist.erase(worklist.begin());
    //T bbEnterVal = (*result)[bb].first;
    //compute the start block's out-flow value(each var's interval)
    visitor->compDFVal( bb, &((*result)[bb].second),result,back_edge,loop_list, true );


    while(!worklist.empty())
    {
        bb=*worklist.begin();
        worklist.erase(worklist.begin());


        //conditionalbranch
        for (pred_iterator pi = pred_begin(bb), pe = pred_end(bb); pi != pe; ++pi) 
        {
            visitor->handlePredIcmp(*pi,result);
        }

        T *bbEnterVal = &((*result)[bb].first);

        for (pred_iterator pi = pred_begin(bb), pe = pred_end(bb); pi != pe; ++pi) 
        {
            //merge all pred basic blocks
            //visitor->merge(&bbexitval, (*result)[succ].first);
            visitor->merge(bbEnterVal,(*result)[*pi].second,bb,result);
        }

        visitor->compDFVal(bb, bbEnterVal,result,back_edge,loop_list, true);
        //compute the basic block's out-flow
        (*result)[bb].second = *bbEnterVal;

    }
    return;
}



template<class T>
void compBackwardDataflow (Function *fn, 
                           DataflowVisitor<T> *visitor,
                           typename DataflowResult<T>::Type *result,
                           map<BasicBlock *, BasicBlock * > &back_edge,
                           set<BasicBlock *> &loop_list,
                           const T &initval) 
{

    std::set<BasicBlock *> worklist;

    // Initialize the worklist with all exit blocks
    for ( Function::iterator bi = fn->begin(); bi != fn->end(); ++bi ) 
    {
        BasicBlock * bb = &*bi;
        result->insert(std::make_pair(bb, std::make_pair(initval, initval)));
        worklist.insert(bb);
    }

    // Iteratively compute the dataflow result
    while (!worklist.empty()) 
    {
        BasicBlock *bb = *worklist.begin();
        worklist.erase(worklist.begin());

        // Merge all incoming value
        T bbexitval = (*result)[bb].second;
        for (succ_iterator si = succ_begin(bb), se = succ_end(bb); si != se; si ++) 
        {
            BasicBlock *succ = *si;
            visitor->merge(&bbexitval, (*result)[succ].first);
        }

        (*result)[bb].second = bbexitval;
        visitor->compDFVal(bb, &bbexitval, result,back_edge,loop_list,false);

        // If outgoing value changed, propagate it along the CFG
        if (bbexitval == (*result)[bb].first) continue;
        (*result)[bb].first = bbexitval;

        for (pred_iterator pi = pred_begin(bb), pe = pred_end(bb); pi != pe; pi++) 
        {
            worklist.insert(*pi);
        }
    }
}
