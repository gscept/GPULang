//------------------------------------------------------------------------------
// @file memory.cc
//------------------------------------------------------------------------------
#include "memory.h"

#if __WIN32__
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef min
#undef max
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

namespace GPULang
{

std::recursive_mutex StaticAllocMutex;

thread_local Allocator* CurrentAllocator;
Allocator DefaultAllocator = 
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
bool IsDefaultAllocatorInitialized = false;

Allocator StaticAllocator =
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
bool IsStaticAllocatorInitialized = false;
thread_local bool IsStaticAllocating = false;

static int NumAllocatorBlocks = 2048;
static int NumAllocatorVirtualAllocs = 8192;
//------------------------------------------------------------------------------
/**
*/
void 
InitAllocator(Allocator* alloc)
{
    // Virtual alloc 16MB and commit the first page
    alloc->mem = vmalloc(alloc->maxSize);
    alloc->it = alloc->mem;
    alloc->end = (char*)alloc->mem + alloc->maxSize;
    vcommit(alloc->mem, SystemPageSize);
    alloc->pageCount = 1; // Start with one page committed

    alloc->freeVirtualMemSlots = (uint32_t*)malloc(sizeof(uint32_t) * NumAllocatorVirtualAllocs);
    alloc->virtualMem = (Allocator::VAlloc*)malloc(sizeof(Allocator::VAlloc) * NumAllocatorVirtualAllocs);
    for (int32_t i = NumAllocatorVirtualAllocs - 1; i >= 0; i--)
    {
        alloc->virtualMem[i] = Allocator::VAlloc();
        alloc->freeVirtualMemSlots[NumAllocatorVirtualAllocs - 1 - i] = i;
    }
    alloc->freeVirtualMemSlotCounter = NumAllocatorVirtualAllocs - 1;
}

//------------------------------------------------------------------------------
/**
*/
void 
DestroyAllocator(Allocator* alloc)
{
    if (alloc->mem != nullptr)
    {
        if (alloc->pageCount > 0)
        {
            vdecommit(alloc->mem, alloc->pageCount * SystemPageSize);
        }
        vfree(alloc->mem, alloc->maxSize);
    }
    alloc->pageCount = 0;
    alloc->mem = nullptr;
    alloc->it = nullptr;
    
    // Free up any dangling virtual memory ranges
    if (alloc->virtualMem != nullptr)
    {
        for (size_t i = 0; i < 4096; i++)
        {
            if (alloc->virtualMem[i].mem != nullptr)
            { 
                vfree(alloc->virtualMem[i].mem, alloc->virtualMem[i].size);
                alloc->virtualMem[i] = Allocator::VAlloc();
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
MakeAllocatorCurrent(Allocator* alloc)
{
    CurrentAllocator = alloc;
}

//------------------------------------------------------------------------------
/**
*/
void 
ResetAllocator(Allocator* alloc)
{
    DestroyAllocator(alloc);
    InitAllocator(alloc);
}

//------------------------------------------------------------------------------
/**
*/
void SetupSystem()
{
#if __WIN32__
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    SystemPageSize = sysInfo.dwPageSize;
#else
    SystemPageSize = getpagesize();
#endif
}

thread_local char* ThreadLocalHeap = nullptr;
thread_local void* ThreadLocalHeapPtr = ThreadLocalHeap;
thread_local size_t ThreadLocalIterator = 0;
size_t SystemPageSize;



//------------------------------------------------------------------------------
/**
*/
void*
vmalloc(size_t size)
{
#if __WIN32__
    void* ret = VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
    assert(ret != nullptr);
    return ret;
#else
    void* ret = mmap(nullptr, size, PROT_NONE, MAP_ANON | MAP_PRIVATE, 0, 0);
    assert(ret != nullptr);
    return ret;
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
vfree(void* data, size_t size)
{
#if __WIN32__
    bool res = VirtualFree(data, 0, MEM_RELEASE);
    assert(res);
#else
    bool res = munmap(data, size);
    assert(res == 0);
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
vcommit(void* data, size_t size)
{
#if __WIN32__
    bool res = VirtualAlloc(data, size, MEM_COMMIT, PAGE_READWRITE);
    assert(res != 0);
#else
    int res = mprotect(data, size, PROT_READ | PROT_WRITE);
    
    assert(res == 0);
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
vdecommit(void* data, size_t size)
{
#if __WIN32__
    bool res = VirtualFree(data, size, MEM_DECOMMIT);
    assert(res != 0);
#else
    int res = madvise(data, size, MADV_DONTNEED);
    assert(res == 0);
    res = mprotect(data, size, PROT_NONE);
    assert(res == 0);
#endif
}

} // namespace GPULang
