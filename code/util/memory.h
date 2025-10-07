//------------------------------------------------------------------------------
/**
    @file memory.h

    Memory management

    2024 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#pragma once
#include <vector>
#include <assert.h>
#include <cstring>
#include <stdarg.h>
#include <mutex>
#include <type_traits>
#include <memory>
#include <limits>
#include <cmath>


namespace GPULang
{

    
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
    // Virtual memory range
    void* mem;
    void* it;
    void* end;
    size_t maxSize;
    size_t pageCount;

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
        .mem = nullptr,
        .it = nullptr,
        .end = nullptr,
        .maxSize = 0xFFFFFFFF,
        .pageCount = 0,
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

extern thread_local Allocator* CurrentAllocator;
void InitAllocator(Allocator* alloc);
void DestroyAllocator(Allocator* alloc);
void MakeAllocatorCurrent(Allocator* alloc);
void ResetAllocator(Allocator* alloc);

extern size_t SystemPageSize;

void SetupSystem();

struct SystemSetup
{
    SystemSetup()
    {
        SetupSystem();
    }
};

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
    void* alignedIt = (char*)allocator->it;
    size_t vmemLeft = (char*)allocator->end - (char*)allocator->it;
    alignedIt = (char*)std::align(alignment, size, alignedIt, vmemLeft);
    assert(alignedIt != nullptr && "Allocator is full");
    
    // check if we need a new commit
    char* validOffset = (char*)allocator->mem + allocator->pageCount * SystemPageSize;
    char* neededOffset = (char*)alignedIt + size;
    if (validOffset <= neededOffset)
    {
        size_t numNeededPages = (size_t)ceil((neededOffset - (validOffset-1)) / (float)SystemPageSize);
        assert(numNeededPages > 0 && "Invalid number of pages needed");
        vcommit(validOffset, numNeededPages * SystemPageSize);
        allocator->pageCount += numNeededPages;
    }
    
    allocator->it = (char*)alignedIt + size;
    if (std::is_trivially_constructible<T>::value)
        std::memset(alignedIt, 0x0, sizeof(T));
    else
        new (alignedIt) T(std::forward<ARGS>(args)...);
    
    return (T*)alignedIt;
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
    void* alignedIt = (char*)allocator->it;
    size_t vmemLeft = (char*)allocator->end - (char*)allocator->it;
    alignedIt = (char*)std::align(alignment, size, alignedIt, vmemLeft);
    assert(alignedIt != nullptr && "Allocator is full");
    
    // check if we need a new commit
    char* validOffset = (char*)allocator->mem + allocator->pageCount * SystemPageSize;
    char* neededOffset = (char*)alignedIt + size;
    if (validOffset <= neededOffset)
    {
        size_t numNeededPages = (size_t)ceil((neededOffset - (validOffset-1)) / (float)SystemPageSize);
        assert(numNeededPages > 0 && "Invalid number of pages needed");
        vcommit(validOffset, numNeededPages * SystemPageSize);
        allocator->pageCount += numNeededPages;
    }
    
    allocator->it = (char*)alignedIt + size;
    if (std::is_trivially_constructible<T>::value)
        std::memset(alignedIt, 0x0, sizeof(T) * num);
    else
        new (alignedIt) T[num];
    
    return (T*)alignedIt;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Dealloc(Allocator* allocator, void* mem)
{
    vdecommit(allocator->mem, allocator->pageCount * SystemPageSize);
    allocator->pageCount = 0;
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
    
    if (Allocator == nullptr)
    {
        if (!IsDefaultAllocatorInitialized)
        {
            InitAllocator(&DefaultAllocator);
            IsDefaultAllocatorInitialized = true;
        }
        Allocator = &DefaultAllocator;
    }
    
    assert(Allocator->freeVirtualMemSlotCounter > 0 && "Too many virtual allocations");
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
Alloc(ARGS&&... args)
{
    Allocator* Allocator = CurrentAllocator;
    
    if (Allocator == nullptr)
    {
        if (!IsDefaultAllocatorInitialized)
        {
            InitAllocator(&DefaultAllocator);
            IsDefaultAllocatorInitialized = true;
        }
        Allocator = &DefaultAllocator;
    }
    
    return __AllocInternal<T>(Allocator, std::forward<ARGS>(args)...);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, typename ... ARGS>
inline T*
Alloc(Allocator* allocoator, ARGS&&... args)
{
    return __AllocInternal<T>(allocoator, std::forward<ARGS>(args)...);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T*
AllocArray(std::size_t num)
{
    Allocator* Allocator = CurrentAllocator;

    if (Allocator == nullptr)
    {
        if (!IsDefaultAllocatorInitialized)
        {
            InitAllocator(&DefaultAllocator);
            IsDefaultAllocatorInitialized = true;
        }
        Allocator = &DefaultAllocator;
    }

    return __AllocArrayInternal<T>(Allocator, num);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T*
AllocArray(Allocator* allocoator, std::size_t num)
{
    return __AllocArrayInternal<T>(allocoator, num);
}


static const size_t ThreadLocalHeapSize = 16_MB;
extern thread_local char* ThreadLocalHeap;
extern thread_local void* ThreadLocalHeapPtr;

//------------------------------------------------------------------------------
/**
*/
template<typename TYPE>
TYPE* 
AllocStack(size_t count, size_t& numBytes)
{
    // Initialize if not already initialized
    if (ThreadLocalHeap == nullptr)
    {
        ThreadLocalHeap = (char*)malloc(ThreadLocalHeapSize);
        ThreadLocalHeapPtr = ThreadLocalHeap;
    }
    const char* HeapStart = (const char*)ThreadLocalHeap;
    const char* HeapEnd = HeapStart + ThreadLocalHeapSize;
    const char* HeapPtr = (const char*)ThreadLocalHeapPtr;
    size_t alignment = alignof(TYPE);
    size_t size = sizeof(TYPE) * count;
    size_t sizeLeft = HeapEnd - HeapPtr;
    void* alignedPtr = std::align(alignment, size, ThreadLocalHeapPtr, sizeLeft);
    assert(alignedPtr != nullptr);
    ThreadLocalHeapPtr = ((char*)alignedPtr) + count * sizeof(TYPE);
    numBytes = (char*)ThreadLocalHeapPtr - HeapPtr;
    TYPE* ret = (TYPE*)alignedPtr;
    if (std::is_trivially_constructible<TYPE>::value)
        std::memset(ret, 0x0, sizeof(TYPE) * count);
    else
        new (ret) TYPE[count];
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
template<typename TYPE>
void 
DeallocStack(size_t count, size_t size, TYPE* buf, size_t numBytes)
{
    if (!std::is_trivially_destructible<TYPE>::value)
    {
        for (size_t i = size-1; i != std::numeric_limits<size_t>::max(); i--)
            (buf + i)->~TYPE();
    }
    const char* HeapPtr = (const char*)ThreadLocalHeapPtr;
    TYPE* topPtr = (TYPE*)(HeapPtr - count * sizeof(TYPE));
    assert(topPtr == buf);
    ThreadLocalHeapPtr = (void*)(HeapPtr - numBytes);
}


} // namespace GPULang
