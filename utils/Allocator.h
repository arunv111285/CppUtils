//=========================================================================================================================!
// Implementation base on the following code project article
// https://www.codeproject.com/Articles/1083210/An-Efficient-Cplusplus-Fixed-Block-Memory-Allocato
//=========================================================================================================================!

#pragma once

//=========================================================================================================================!
namespace av { namespace utils {
//=========================================================================================================================!

class Allocator
{
public:
	
	/// <summary>
	/// Initializes a new instance of the <see cref="Allocator"/> class.
	/// </summary>
	/// <param name="objectSize">Size of the object for which the allocator is required.</param>
	/// <param name="maxNbOfObjects">Applicable for eStatic/eHeapPool. The max number of objects that can be instantiated .</param>
	/// <param name="staticMemoryPool">Applicable for eStaticPool. A handle to the static memory pool.</param>
	Allocator(size_t objectSize, size_t maxNbOfObjects = 0, char* staticMemoryPool = nullptr);
	
	/// <summary>
	/// Finalizes an instance of the <see cref="Allocator"/> class.
	/// </summary>
	~Allocator();
	
	/// <summary>
	/// Get a pointer to a memory block.
	/// </summary>
	/// <param name="size">The size of the block to allocated. 
	///					   Has to be lesser than the objectSize specified when constructing the Allocator.</param>
	/// <returns></returns>
	void* allocate(size_t size);
	
	/// <summary>
	/// Return the memory block to the pool.
	/// </summary>
	/// <param name="block">The block.</param>
	void deallocate(void* block);

private:

	void* pop();

	void push(void* block);

	// Use pool when we know the limit on the maximum number of objects
	// Block mode has no such quantity limitations. Will construct as many
	// objects as storage permits.
	enum class EAllocationMode
	{
		eUnknown,
		eStaticPool,
		eHeapPool,
		eHeapBlocks
	};

	size_t			objectSize_ = 0;
	size_t			nbOfObjects_ = 0;
	size_t			objectIndex_ = 0;
	EAllocationMode mode_ = EAllocationMode::eUnknown;
	char*			memoryPool_ = nullptr;

	struct Block
	{
		Block* next;
	};

	Block*	head_ = nullptr;
};

//=========================================================================================================================!
}}
//=========================================================================================================================!

#define DECLARE_ALLOCATOR \
public:\
void* operator new (size_t size)\
{\
	return allocator_.allocate(size);\
}\
void operator delete (void* block)\
{\
	return allocator_.deallocate(block);\
}\
private:\
	static av::utils::Allocator allocator_;


#define IMPLEMENT_ALLOCATOR(className) av::utils::Allocator className::allocator_(sizeof(className));
