//------------------------------------------------------------------------------
// @file memory.cc
//------------------------------------------------------------------------------

#include "memory.h"

#if __WIN32__
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef min
#undef max
#endif

bool SYMBOL_STATIC_ALLOC = false;

namespace GPULang
{

std::recursive_mutex StaticAllocMutex;

thread_local Allocator* CurrentAllocator;
Allocator DefaultAllocator = 
{
    .freeBlockCounter = 0,
    .freeBlocks = nullptr,
    .blocks = nullptr,
    .currentBlock = 0,
    .blockSize = 65535
};
bool IsDefaultAllocatorInitialized = false;

Allocator StaticAllocator =
{
    .freeBlockCounter = 0,
    .freeBlocks = nullptr,
    .blocks = nullptr,
    .currentBlock = 0,
    .blockSize = 65535
};
bool IsStaticAllocatorInitialized = false;
thread_local bool IsStaticAllocating = false;
//------------------------------------------------------------------------------
/**
*/
void 
InitAllocator(Allocator* alloc)
{
    assert(alloc->blockSize > 0);
    if (alloc->freeBlocks != nullptr)
        free(alloc->freeBlocks);
    if (alloc->blocks != nullptr)
        free(alloc->blocks);
    alloc->freeBlocks = (uint32_t*)malloc(sizeof(uint32_t) * 2048);
    alloc->blocks = (MemoryBlock*)malloc(sizeof(MemoryBlock) * 2048);
    for (int32_t i = 2047; i >= 0; i--)
    {
        alloc->blocks[i] = MemoryBlock();
        alloc->freeBlocks[2047 - i] = i;
    }
    alloc->freeBlockCounter = 2047;
    alloc->currentBlock = alloc->freeBlocks[alloc->freeBlockCounter];
    alloc->blocks[alloc->currentBlock] = MemoryBlock(malloc(alloc->blockSize), alloc->currentBlock);
    alloc->freeBlockCounter--;
}

//------------------------------------------------------------------------------
/**
*/
void 
DestroyAllocator(Allocator* alloc)
{
    if (alloc->blocks != nullptr)
    {
        for (uint32_t i = 0; i < 2048; i++)
        {
            if (alloc->blocks[i].mem != nullptr)
            {
                free(alloc->blocks[i].mem);
                alloc->blocks[i] = MemoryBlock();
            }
        }
    }
    if (alloc->freeBlocks != nullptr)
        free(alloc->freeBlocks);
    if (alloc->blocks != nullptr)
        free(alloc->blocks);

    alloc->freeBlocks = nullptr;
    alloc->blocks = nullptr;
    alloc->freeBlockCounter = 0;
    alloc->currentBlock = 0;
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

thread_local char ThreadLocalHeap[ThreadLocalHeapSize];
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
vfree(void* buf, size_t size)
{
#if __WIN32__
    bool res = VirtualFree(buf, 0, MEM_RELEASE);
    assert(res);
#else
    int res = madvise(ptr, size, MADV_DONTNEED);
    assert(res == 0);
    res = munmap(ptr, size);
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
    res = mprotect(ptr, size, PROT_NONE);
    assert(res == 0);
#endif
}

} // namespace GPULang
