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

struct MemoryBlock
{
    MemoryBlock()
        : iterator(0)
        , blockIndex(-1)
        , mem(nullptr)
    {}
    MemoryBlock(void* mem, uint32_t index)
        : iterator(0)
        , blockIndex(index)
        , mem(mem)
    {}
    void* mem;
    size_t iterator;
    uint32_t blockIndex;
};

void InitMemory();
void ResetMemory();
extern uint32_t FreeBlockCounter;
extern uint32_t* FreeBlocks;
extern MemoryBlock* Blocks;
extern size_t CurrentBlock;
extern size_t BlockSize;
extern bool IsInit;

void Dealloc(void* alloc);

//------------------------------------------------------------------------------
/**
*/
template<typename T, typename ... ARGS>
inline T*
Alloc(ARGS... args)
{
    if (!IsInit)
        InitMemory();
    
    const size_t size = sizeof(T);
    MemoryBlock* block = &Blocks[CurrentBlock];
    if (block->iterator + size > BlockSize)
    {
        CurrentBlock = FreeBlocks[FreeBlockCounter--];
        block = &Blocks[CurrentBlock];
        block->blockIndex = CurrentBlock;
        block->mem = malloc(BlockSize);
    }
    assert(block->blockIndex != -1);

    char* buf = (char*)block->mem + block->iterator;
    block->iterator += size;
    new (buf) T(args...);
    return (T*)buf;
}