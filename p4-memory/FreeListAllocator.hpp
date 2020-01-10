#pragma once

#include<windows.h>

FreeListAllocator::FreeListAllocator(int totalSize)
{
	this->totalSize = totalSize;
	memInUse = 0;
}

void FreeListAllocator::init(void* p)
{
	startPtr = p;
	this->reset();
}

FreeListAllocator::~FreeListAllocator()
{
	startPtr = nullptr;
	totalSize = 0;
}

void* FreeListAllocator::allocate(int size)
{
	// Search through the free list for a free block that has enough space to allocate our data
	int padding;
	Node* affectedNode;
	Node* previousNode;
	this->findBest(size, padding, previousNode, affectedNode);
	int alignmentPadding = padding - ALLOCATION_HEADER_SIZE;
	int requiredSize = size + padding;

	if (memInUse + requiredSize > totalSize)
	{
		return nullptr;
	}

	const int rest = affectedNode->data.blockSize - requiredSize;
	if (rest > 0)
	{
		// We have to split the block into the data block and a free block of size 'rest'
		Node* newFreeNode = (Node*)((int)affectedNode + requiredSize);

		newFreeNode->data.blockSize = rest;
		freeList.insert(affectedNode, newFreeNode);
	}

	freeList.remove(previousNode, affectedNode);

	// Setup data block
	const int headerAddress = (int)affectedNode + alignmentPadding;
	const int dataAddress = headerAddress + ALLOCATION_HEADER_SIZE;

	((FreeListAllocator::AllocationHeader*)headerAddress)->blockSize = requiredSize;
	((FreeListAllocator::AllocationHeader*)headerAddress)->padding = alignmentPadding;

	memInUse += requiredSize;

	return (void*)dataAddress;
}

void FreeListAllocator::findBest(int size, int& padding, Node*& previousNode,
	Node*& foundNode)
{
	// Iterate whole list keeping a pointer to the best fit
	Node* bestBlock = nullptr;
	Node* currentNode = freeList.head;
	Node* prevNode = nullptr;

	while (currentNode != nullptr)
	{
		padding = calculatePaddingWithHeader((int)currentNode,
			sizeof(FreeListAllocator::AllocationHeader));
		const int requiredSpace = size + padding;

		if (currentNode->data.blockSize >= requiredSpace)
		{
			bestBlock = currentNode;
		}

		prevNode = currentNode;
		currentNode = currentNode->next;
	}

	previousNode = prevNode;
	foundNode = bestBlock;
}

int FreeListAllocator::calculatePaddingWithHeader(int baseAddress, int headerSize)
{
	const int multiplier = (baseAddress / ALIGNMENT) + 1;
	const int alignedAddress = multiplier * ALIGNMENT;
	int padding = alignedAddress - baseAddress;
	int neededSpace = headerSize;

	if (padding < neededSpace)
	{
		// Header does not fit - Calculate next aligned address that header fits
		neededSpace -= padding;

		// How many alignments I need to fit the header        
		if (neededSpace % ALIGNMENT > 0)
		{
			padding += ALIGNMENT * (1 + (neededSpace / ALIGNMENT));
		}
		else
		{
			padding += ALIGNMENT * (neededSpace / ALIGNMENT);
		}
	}

	return padding;
}

void FreeListAllocator::free(void* p)
{
	// Insert it in a sorted position by the address number
	int currentAddress = (int)p;
	int headerAddress = currentAddress - sizeof(FreeListAllocator::AllocationHeader);
	const FreeListAllocator::AllocationHeader* allocationHeader{
		(FreeListAllocator::AllocationHeader*)headerAddress
	};

	Node* freeNode = (Node*)(headerAddress);
	freeNode->data.blockSize = allocationHeader->blockSize + allocationHeader->padding;
	freeNode->next = nullptr;

	Node* currentNode = freeList.head;
	Node* prevNode = nullptr;
	while (currentNode != nullptr)
	{
		if (p < currentNode)
		{
			freeList.insert(prevNode, freeNode);
			break;
		}

		prevNode = currentNode;
		currentNode = currentNode->next;
	}

	memInUse -= freeNode->data.blockSize;

	// Merge contiguous nodes
	coalescence(prevNode, freeNode);
}

void FreeListAllocator::coalescence(Node* previousNode, Node* freeNode)
{
	if (freeNode->next != nullptr 
		&& (int)freeNode + freeNode->data.blockSize == (int)freeNode->next)
	{
		freeNode->data.blockSize += freeNode->next->data.blockSize;
		freeList.remove(freeNode, freeNode->next);
	}

	if (previousNode != nullptr 
		&& (int)previousNode + previousNode->data.blockSize == (int)freeNode)
	{
		previousNode->data.blockSize += freeNode->data.blockSize;
		freeList.remove(previousNode, freeNode);
	}
}

void FreeListAllocator::reset()
{
	memInUse = 0;
	Node* firstNode = (Node*)startPtr;
	firstNode->data.blockSize = totalSize;
	firstNode->next = nullptr;
	freeList.head = nullptr;
	freeList.insert(nullptr, firstNode);
}

bool FreeListAllocator::isInRange(void* p)
{
	return (uintptr_t)p >= (uintptr_t)startPtr &&
		(uintptr_t)p < (uintptr_t)startPtr + (uintptr_t)totalSize;
}
