#pragma once

#include "LinkedList.h"

class FreeListAllocator
{
private:

	struct FreeHeader
	{
		int blockSize;
	};

	struct AllocationHeader
	{
		int blockSize;
		char padding;
	};

	const static int ALLOCATION_HEADER_SIZE = sizeof(AllocationHeader);
	const static int ALIGNMENT = 8;
	
	int totalSize;
	int memInUse;
	void* startPtr = nullptr;

	typedef LinkedList<FreeHeader>::Node Node;

	LinkedList<FreeHeader> freeList;

	void coalescence(Node* prevBlock, Node* freeBlock);

	void findBest(int size, int& padding, Node*& previousNode,
		Node*& foundNode);

	static int calculatePaddingWithHeader(int baseAddress, int headerSize);

public:
	FreeListAllocator(int totalSize);
	virtual ~FreeListAllocator();

	virtual void* allocate(int size);
	virtual void free(void* p);
	virtual void init(void* p);
	virtual void reset();
	virtual bool isInRange(void* p);
};

#include "FreeListAllocator.hpp"