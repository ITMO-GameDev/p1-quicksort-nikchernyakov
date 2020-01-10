#pragma once

#include <unordered_map>
#include <vector>

#include "FreeListAllocator.h"

class MemoryAllocator
{
private:

	struct FSA
	{
		int blocksCount;
		int freeBlocksCount;
		int blockSize;
		int freeP;
		void* base;
	};

	const static int ALLOC_RESERVATION_SIZE = 100 * 1024 * 1024;

	void* reservedPtr;
	int memInUse;
	
	const static int FSA_BOUND_SIZE = 512;
	std::unordered_map<int, std::vector<FSA>> fsaHeap;

	const static int PAGE_SIZE = 4 * 1024;
	std::vector<void*> pages;

	const static int COALESCE_BOUND_SIZE = 10 * 1024 * 1024;
	const static int ALLOC_COMMIT_SIZE_COALESCE = 10 * 1024 * 1024;
	std::vector<FreeListAllocator*> coalesceHeap;

	std::vector<void*> largeHeap;

	void* alloc(FSA& fsa);
	void* fsaAlloc(const int size);
	void* coalesceAlloc(const int size);
	void* largeAlloc(const int size);
	
	void free(FSA& fsa, void* p);
	FSA& findFsa(void* p);

public:

	MemoryAllocator();
	virtual ~MemoryAllocator();
	
	virtual void init();
	virtual void destroy();
	virtual void* alloc(size_t size);
	virtual void free(void* p);
};

#include "MemoryAllocator.hpp"