//------------------------------------------------------------------------------
/**
    @file memory.h

    Memory management

    2024 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#pragma once
#include <stdint.h>
#include <vector>
#include <assert.h>
#include <string>
#include <stdarg.h>
#include <charconv>
#include <mutex>
#include <type_traits>


extern bool SYMBOL_STATIC_ALLOC;

//------------------------------------------------------------------------------
/**
*/
constexpr size_t
operator"" _KB(const unsigned long long val)
{
    return val * 1024;
}

//------------------------------------------------------------------------------
/**
*/
constexpr size_t
operator"" _MB(const unsigned long long val)
{
    return val * 1024 * 1024;
}

//------------------------------------------------------------------------------
/**
*/
constexpr size_t
operator"" _GB(const unsigned long long val)
{
    return val * 1024 * 1024 * 1024;
}


namespace GPULang
{

struct MemoryBlock
{
    MemoryBlock()
        : mem(nullptr)
        , iterator(nullptr)
        , allocs(0)
        , blockIndex(-1)
    {
    }
    MemoryBlock(void* mem, uint32_t index)
        : mem(mem)
        , iterator(mem)
        , allocs(0)
        , blockIndex(index)
    {
    }
    void* mem;
    void* iterator;
    size_t allocs;
    size_t blockIndex;
};

struct Allocator
{
    uint32_t freeBlockCounter;
    uint32_t* freeBlocks;
    MemoryBlock* blocks;
    size_t currentBlock;
    size_t blockSize;

    struct VAlloc
    {
        void* mem = nullptr;
        uint32_t index = 0;
        size_t size = 0;
        Allocator* alloc;
    };
    VAlloc* virtualMem;
    uint32_t freeVirtualMemSlotCounter;
    uint32_t* freeVirtualMemSlots;
};

inline Allocator CreateAllocator()
{
    return Allocator
    {
        .freeBlockCounter = 0,
        .freeBlocks = nullptr,
        .blocks = nullptr,
        .currentBlock = 0,
        .blockSize = 65535,
        .virtualMem = nullptr,
        .freeVirtualMemSlotCounter = 0,
        .freeVirtualMemSlots = nullptr
    };
}

// Virtual memory interface
void* vmalloc(size_t size);
void vfree(void* buf, size_t size);
void vcommit(void* data, size_t size);
void vdecommit(void* data, size_t size);


// The default allocator is used for all static initialization
extern Allocator DefaultAllocator;
extern bool IsDefaultAllocatorInitialized;

extern Allocator StaticAllocator;
extern bool IsStaticAllocatorInitialized;
extern thread_local bool IsStaticAllocating;

extern thread_local Allocator* CurrentAllocator;
extern std::recursive_mutex StaticAllocMutex;
void InitAllocator(Allocator* alloc);
void DestroyAllocator(Allocator* alloc);
void MakeAllocatorCurrent(Allocator* alloc);
void ResetAllocator(Allocator* alloc);

extern size_t SystemPageSize;

void SetupSystem();

void Dealloc(void* alloc);

//------------------------------------------------------------------------------
/**
*/
inline unsigned int
align(unsigned int alignant, unsigned int alignment)
{
    return (alignant + alignment - 1) & ~(alignment - 1);
}


struct SPVArg;

//------------------------------------------------------------------------------
/**
*/
template<typename T, typename ... ARGS>
inline T*
__AllocInternal(Allocator* allocator, ARGS&&... args)
{
    const size_t size = sizeof(T);
    const size_t alignment = alignof(T);
    MemoryBlock* block = &allocator->blocks[allocator->currentBlock];
    const char* blockBegin = (char*)block->mem;
    const char* blockEnd = blockBegin + allocator->blockSize;
    size_t sizeLeft = blockEnd - (char*)block->iterator;
    void* alignedIterator = std::align(alignment, size, block->iterator, sizeLeft);
    assert(size <= allocator->blockSize);
    if (alignedIterator == nullptr)
    {
        allocator->currentBlock = allocator->freeBlocks[allocator->freeBlockCounter--];
        block = &allocator->blocks[allocator->currentBlock];
        block->blockIndex = allocator->currentBlock;
        block->mem = malloc(allocator->blockSize);
        sizeLeft = allocator->blockSize;
        alignedIterator = std::align(alignment, size, block->mem, sizeLeft);
    }
    assert(block->blockIndex != -1);
    block->allocs++;

    block->iterator = (char*)alignedIterator + size;
    if (std::is_trivially_constructible<T>::value)
        memset(alignedIterator, 0x0, sizeof(T));
    else
        new (alignedIterator) T(std::forward<ARGS>(args)...);
    return (T*)alignedIterator;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T*
__AllocArrayInternal(Allocator* allocator, size_t num)
{
    const size_t size = sizeof(T) * num;
    const size_t alignment = alignof(T);
    MemoryBlock* block = &allocator->blocks[allocator->currentBlock];
    const char* blockBegin = (char*)block->mem;
    const char* blockEnd = blockBegin + allocator->blockSize;
    size_t sizeLeft = blockEnd - (char*)block->iterator;
    void* alignedIterator = std::align(alignment, size, block->iterator, sizeLeft);
    assert(size <= allocator->blockSize);
    if (alignedIterator == nullptr)
    {
        allocator->currentBlock = allocator->freeBlocks[allocator->freeBlockCounter--];
        block = &allocator->blocks[allocator->currentBlock];
        block->blockIndex = allocator->currentBlock;
        block->mem = malloc(allocator->blockSize);
        sizeLeft = allocator->blockSize;
        alignedIterator = std::align(alignment, size, block->mem, sizeLeft);
    }
    assert(block->blockIndex != -1);
    block->allocs++;

    block->iterator = (char*)alignedIterator + size;
    if (std::is_trivially_constructible<T>::value)
        memset(alignedIterator, 0x0, sizeof(T) * num);
    else
        new (alignedIterator) T[num];
    return (T*)alignedIterator;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Dealloc(Allocator* allocator, void* mem)
{
    for (size_t i = 0; i < allocator->currentBlock; i++)
    {
        MemoryBlock* block = &allocator->blocks[i];

        // Check if memory lies within block
        if (block->mem <= mem && ((char*)block->mem + allocator->blockSize) > mem)
        {
            // This is the block the memory comes from, decrement counter and free if the block is empty
            assert(block->allocs > 0);
            block->allocs--;
            if (block->allocs == 0)
            {
                free(block->mem);
                allocator->freeBlocks[++allocator->freeBlockCounter] = (int)i;
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline Allocator::VAlloc*
AllocVirtual(size_t num)
{
    if (num == 0)
        return nullptr;
    Allocator* Allocator = CurrentAllocator;
    if (IsStaticAllocating)
    {
        Allocator = &StaticAllocator;
    }
    else
    {
        if (Allocator == nullptr)
        {
            if (!IsDefaultAllocatorInitialized)
            {
                InitAllocator(&DefaultAllocator);
                IsDefaultAllocatorInitialized = true;
            }
            Allocator = &DefaultAllocator;
        }
    }
    assert(Allocator->freeVirtualMemSlotCounter > 0);
    uint32_t vmemIndex = Allocator->freeVirtualMemSlots[Allocator->freeVirtualMemSlotCounter--];
    Allocator::VAlloc* ret = &Allocator->virtualMem[vmemIndex];
    ret->mem = (T*)vmalloc(num * sizeof(T));
    ret->index = vmemIndex;
    ret->size = num * sizeof(T);
    ret->alloc = Allocator;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DeallocVirtual(Allocator::VAlloc* alloc)
{
    Allocator* Allocator = alloc->alloc;
    assert(alloc->mem != nullptr);
    Allocator->freeVirtualMemSlots[++Allocator->freeVirtualMemSlotCounter] = alloc->index;
    vfree(alloc->mem, alloc->size);
    *alloc = Allocator::VAlloc();
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, typename ... ARGS>
inline T*
StaticAlloc(ARGS&&... args)
{
    if (!IsStaticAllocatorInitialized)
    {
        InitAllocator(&StaticAllocator);
        IsStaticAllocatorInitialized = true;
    }
    Allocator* Allocator = &StaticAllocator;
    IsStaticAllocating = true;
    T* ret = __AllocInternal<T>(Allocator, std::forward<ARGS>(args)...);
    IsStaticAllocating = false;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, typename ... ARGS>
inline T*
Alloc(ARGS&&... args)
{
    Allocator* Allocator = CurrentAllocator;
    if (IsStaticAllocating)
    {
        Allocator = &StaticAllocator;
    }
    else
    {
        if (Allocator == nullptr)
        {
            if (!IsDefaultAllocatorInitialized)
            {
                InitAllocator(&DefaultAllocator);
                IsDefaultAllocatorInitialized = true;
            }
            Allocator = &DefaultAllocator;
        }
    }
    return __AllocInternal<T>(Allocator, std::forward<ARGS>(args)...);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T*
AllocArray(std::size_t num)
{
    Allocator* Allocator = CurrentAllocator;
    if (IsStaticAllocating)
    {
        Allocator = &StaticAllocator;
    }
    else
    {
        if (Allocator == nullptr)
        {
            if (!IsDefaultAllocatorInitialized)
            {
                InitAllocator(&DefaultAllocator);
                IsDefaultAllocatorInitialized = true;
            }
            Allocator = &DefaultAllocator;
        }
    }
    return __AllocArrayInternal<T>(Allocator, num);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T*
StaticAllocArray(std::size_t num)
{
    if (!IsStaticAllocatorInitialized)
    {
        InitAllocator(&StaticAllocator);
        IsStaticAllocatorInitialized = true;
    }
    Allocator* Allocator = &StaticAllocator;
    IsStaticAllocating = true;
    T* ret = __AllocArrayInternal<T>(Allocator, num);
    IsStaticAllocating = false;
    return ret;
}

static const size_t ThreadLocalHeapSize = 16_MB;
extern thread_local char ThreadLocalHeap[];
extern thread_local void* ThreadLocalHeapPtr;

//------------------------------------------------------------------------------
/**
*/
template<typename TYPE>
TYPE* 
AllocStack(size_t count)
{
    const char* HeapStart = (const char*)ThreadLocalHeap;
    const char* HeapEnd = HeapStart + ThreadLocalHeapSize;
    const char* HeapPtr = (const char*)ThreadLocalHeapPtr;
    size_t alignment = alignof(TYPE);
    size_t size = sizeof(TYPE) * count;
    size_t sizeLeft = HeapEnd - HeapPtr;
    void* alignedPtr = std::align(alignment, size, ThreadLocalHeapPtr, sizeLeft);
    assert(alignedPtr != nullptr);
    ThreadLocalHeapPtr = (char*)alignedPtr + count * sizeof(TYPE);
    TYPE* ret = (TYPE*)alignedPtr;
    if (std::is_trivially_constructible<TYPE>::value)
        memset(ret, 0x0, sizeof(TYPE) * count);
    else
        new (ret) TYPE[count];
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
template<typename TYPE>
void 
DeallocStack(size_t count, TYPE* buf)
{
    const char* HeapPtr = (const char*)ThreadLocalHeapPtr;
    TYPE* topPtr = (TYPE*)(HeapPtr - count * sizeof(TYPE));
    assert(topPtr == buf);
    if (!std::is_trivially_destructible<TYPE>::value)
    {
        for (size_t i = 0; i < count; i++)
            (buf + i)->~TYPE();
    }
    ThreadLocalHeapPtr = topPtr;
}


} // namespace GPULang
