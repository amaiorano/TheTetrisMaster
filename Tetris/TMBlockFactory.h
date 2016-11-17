// TMBlockFactory.h: interface for the TMBlockFactory class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TMBLOCK_FACTORY_H_
#define _TMBLOCK_FACTORY_H_

#include "TMBlock.h"
#include <stack>

class TMBlockFactory  
{
	public:		
		TMBlockFactory();
		~TMBlockFactory();

		TMBlock GetNextRandomBlock();		// Returns a copy of the block
		TMBlock* GetBlockWithId(int id);	// Returns actual block

	private:
		TMBlock m_allBlocksArray[7];

		std::stack<int, std::vector<int>> m_nextBlockIndices;
};

#endif _TMBLOCK_FACTORY_H_
