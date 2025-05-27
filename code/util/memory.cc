//------------------------------------------------------------------------------
/**
    @file memory.h

    Memory management

    2024 Gustav Sterbrant
*/
//------------------------------------------------------------------------------

#include "memory.h"

bool SYMBOL_STATIC_ALLOC = false;

namespace GPULang
{

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

thread_local char ThreadLocalHeap[0xFFFF];
thread_local size_t ThreadLocalIterator = 0;

} // namespace GPULang
