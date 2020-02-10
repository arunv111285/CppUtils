//=========================================================================================================================!
#include "Allocator.h"


#include <cassert>

//=========================================================================================================================!

using namespace av::utils;

//=========================================================================================================================!

Allocator::Allocator(size_t objectSize, size_t nbOfObjects/* = 0*/, char * staticMemoryPool)
		  :objectSize_(objectSize < sizeof(size_t) ? sizeof(size_t) : objectSize),
		   nbOfObjects_(nbOfObjects)
{
	if (nbOfObjects_) // Pool mode of allocation. Nb of objects is limited
	{
		if (staticMemoryPool)
		{
			memoryPool_ = staticMemoryPool;
			mode_ = EAllocationMode::eStaticPool;
		}
		else
		{
			memoryPool_ = new char[objectSize_ * nbOfObjects_];
			mode_ = EAllocationMode::eHeapPool;
		}
	}
	else // Block mode of allocation
		mode_ = EAllocationMode::eHeapBlocks;

	assert(mode_ != EAllocationMode::eUnknown);
}

//=========================================================================================================================!

Allocator::~Allocator()
{
	switch (mode_)
	{
	default:
	case EAllocationMode::eUnknown:
	{
		assert(false);
		break;
	}
	case EAllocationMode::eStaticPool:
	{
		// Do nothing
		break;
	}
	case EAllocationMode::eHeapPool:
	{
		delete[] memoryPool_;
		memoryPool_ = nullptr;
		break;
	}
	case EAllocationMode::eHeapBlocks:
	{
		while (auto block = pop())
		{
			delete[] block;
		}
		break;
	}
	}
}

//=========================================================================================================================!

void* Allocator::allocate(size_t size)
{
	assert(size <= objectSize_);

	switch (mode_)
	{
	case EAllocationMode::eStaticPool:
	case EAllocationMode::eHeapPool:
	{
		if (objectIndex_ < nbOfObjects_)
		{
			return reinterpret_cast<void*>(memoryPool_ + (objectSize_ * objectIndex_++));
		}
		else
			assert(0);
		break;
	}
	case EAllocationMode::eHeapBlocks:
	{
		if (auto freeblock = pop())
		{
			return freeblock;
		}
		else
		{
			auto block = new char[objectSize_];
			return block;
		}
	}
	default:
	case EAllocationMode::eUnknown:
	{
		assert(false);
		break;
	}
	}

	return nullptr;
}

//=========================================================================================================================!

void Allocator::deallocate(void* block)
{
	switch (mode_)
	{
	default:
	case EAllocationMode::eUnknown:
		assert(false);
		break;
	case EAllocationMode::eStaticPool:
	case EAllocationMode::eHeapPool:
		break;
	case EAllocationMode::eHeapBlocks:
	{
		push(block);
		break;
	}
	}
}

//=========================================================================================================================!

void* Allocator::pop()
{
	Block* block = nullptr;
	if (head_)
	{
		block = head_;
		head_ = head_->next;
	}
	return block;
}

//=========================================================================================================================!

void Allocator::push(void* block)
{
	auto freeBlock = reinterpret_cast<Block*>(block);
	freeBlock->next = head_;
	head_ = freeBlock;
}

//=========================================================================================================================!
