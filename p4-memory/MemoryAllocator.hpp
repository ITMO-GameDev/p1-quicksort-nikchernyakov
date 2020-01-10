#pragma once

#include <windows.h>
#include <cmath>


MemoryAllocator::MemoryAllocator()
{
	reservedPtr = nullptr;
	memInUse = 0;

	for (int i = 8; i <= 512; i *= 2)
	{
		fsaHeap[i] = std::vector<FSA>();
		fsaHeap[i].push_back(FSA{ PAGE_SIZE / i,
			PAGE_SIZE / i, i, 0, nullptr });
	}

	coalesceHeap.push_back(new FreeListAllocator(ALLOC_COMMIT_SIZE_COALESCE));
}

MemoryAllocator::~MemoryAllocator()
{
	destroy();
}

void MemoryAllocator::init()
{
	// Reserve the memory
	reservedPtr = VirtualAlloc(nullptr, ALLOC_RESERVATION_SIZE,
		MEM_RESERVE, PAGE_READWRITE);

	// Commit first pages
	for (int i = 0, j = 8; i < 7; i++, j *= 2)
	{
		pages.push_back(VirtualAlloc(((char*)reservedPtr) + memInUse,
			PAGE_SIZE, MEM_COMMIT, PAGE_READWRITE));
		fsaHeap[j][0].base = ((char*)reservedPtr) + memInUse;
		memInUse += PAGE_SIZE;
	}

	coalesceHeap[0]->init(VirtualAlloc(((char*)reservedPtr) + memInUse, ALLOC_COMMIT_SIZE_COALESCE, MEM_COMMIT,
		PAGE_READWRITE));
	memInUse += ALLOC_COMMIT_SIZE_COALESCE;
}

void MemoryAllocator::destroy()
{
	VirtualFree(reservedPtr, 0, MEM_RELEASE);

	reservedPtr = nullptr;
	memInUse = 0;

	// Free large heap
	for (auto ptr : largeHeap)
	{
		VirtualFree(ptr, 0, MEM_RELEASE);
	}
}

void* MemoryAllocator::alloc(size_t size)
{
	if (size == 0)
	{
		return nullptr;
	}

	const int alignedSize = size <= 8 ? 8 : std::pow(2,std::ceil(std::log2(size)));
	if (size <= FSA_BOUND_SIZE)
	{
		// Allocate from FSA
		return fsaAlloc(alignedSize);
	}
	if (size <= COALESCE_BOUND_SIZE)
	{
		// Allocate from First Coalesce Heap
		coalesceAlloc(alignedSize);
	}
	return largeAlloc(alignedSize); // Allocate in Large Heap
}

void* MemoryAllocator::alloc(FSA& fsa)
{
	void* res = ((char*)fsa.base) + fsa.freeP * fsa.blockSize;
	fsa.freeP = *((int*)res);
	fsa.freeBlocksCount--;
	return res;
}

void* MemoryAllocator::fsaAlloc(const int size)
{
#ifdef _DEBUG
	std::cout << "FSA Allocation" << "\n";
#endif
	
	for (auto& fsa : fsaHeap[size])
	{
		if (fsa.freeBlocksCount > 0)
		{
			return alloc(fsa);
		}
	}

	// Create new FSA
	if (memInUse + PAGE_SIZE > ALLOC_RESERVATION_SIZE)
	{
		return nullptr;
	}

	pages.push_back(VirtualAlloc(
		((char*)reservedPtr) + memInUse, PAGE_SIZE,
		MEM_COMMIT, PAGE_READWRITE));
	fsaHeap[size].push_back(FSA{
		PAGE_SIZE / size, PAGE_SIZE / size,
		size, 0,((char*)reservedPtr) + memInUse});

	memInUse += PAGE_SIZE;

	return alloc(fsaHeap[size].back());
}

void* MemoryAllocator::coalesceAlloc(const int size)
{
#ifdef _DEBUG
	std::cout << "Coalesce Allocation" << "\n";
#endif
	
	void* p = nullptr;
	int i = 0;
	while (p == nullptr && i < ((int)coalesceHeap.size()))
	{
		p = coalesceHeap[i]->allocate(size);
		i++;
	}

	if (p != nullptr)
	{
		return p;
	}

	// Create new FreeListAllocator
	if (memInUse + ALLOC_COMMIT_SIZE_COALESCE > ALLOC_RESERVATION_SIZE)
	{
		return nullptr;
	}

	coalesceHeap.push_back(new FreeListAllocator(ALLOC_COMMIT_SIZE_COALESCE));
	coalesceHeap[coalesceHeap.size() - 1]->init(
		VirtualAlloc(((char*)reservedPtr) + memInUse, ALLOC_COMMIT_SIZE_COALESCE,
			MEM_COMMIT, PAGE_READWRITE));
	
	memInUse += ALLOC_COMMIT_SIZE_COALESCE;
	
	return coalesceHeap[coalesceHeap.size() - 1]->allocate(size);
}

void* MemoryAllocator::largeAlloc(const int size)
{
#ifdef _DEBUG
	std::cout << "Large Allocation" << "\n";
#endif

	if (memInUse + size > ALLOC_RESERVATION_SIZE)
	{
		return nullptr;
	}

	void* p = VirtualAlloc(nullptr, size,
	                       MEM_COMMIT, PAGE_READWRITE);
	largeHeap.push_back(p);
	return p;
}

void MemoryAllocator::free(void* p)
{
	FSA& fsaToFree = findFsa(p);

	// If fsa was found
	if (fsaToFree.base != nullptr)
	{
		free(fsaToFree, p);
		return;
	}

	// Free for Coalesce Heap 1
	for (auto allocator : coalesceHeap)
	{
		if (allocator->isInRange(p))
		{
			allocator->free(p);
			return;
		}
	}

	// Free for Large Heap
	if (std::find(largeHeap.begin(), largeHeap.end(), p) != largeHeap.end())
	{
		VirtualFree(p, 0, MEM_RELEASE);
	}
}

MemoryAllocator::FSA& MemoryAllocator::findFsa(void* p)
{
	// Searching in the first FSA Heap
	for (auto& element : fsaHeap)
	{
		for (FSA& fsa : element.second)
		{
			if ((uintptr_t)p >= (uintptr_t)fsa.base &&
				(uintptr_t)p < (uintptr_t)fsa.base + (uintptr_t)PAGE_SIZE)
			{
				return fsa;
			}
		}
	}

	FSA nullFSA = FSA{ 0, 0, 0, 0, nullptr };
	return nullFSA;
}

void MemoryAllocator::free(FSA& fsa, void* p)
{
	*((int*)p) = fsa.freeP;
	fsa.freeP = (((char*)p) - ((char*)fsa.base)) / (fsa.blockSize / sizeof(char));
}
