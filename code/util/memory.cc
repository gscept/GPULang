//------------------------------------------------------------------------------
/**
    @file memory.h

    Memory management

    2024 Gustav Sterbrant
*/
//------------------------------------------------------------------------------

#include "memory.h"
uint32_t FreeBlockCounter = 0;

uint32_t* FreeBlocks = nullptr;
MemoryBlock* Blocks = nullptr;
size_t CurrentBlock = 0;
size_t BlockSize = 65535;
bool IsInit = false;
//------------------------------------------------------------------------------
/**
*/
void
InitMemory()
{
    if (FreeBlocks != nullptr)
        free(FreeBlocks);
    if (Blocks != nullptr)
        free(Blocks);
    FreeBlocks = (uint32_t*)malloc(sizeof(uint32_t) * 2048);
    Blocks = (MemoryBlock*)malloc(sizeof(MemoryBlock) * 2048);
    for (int32_t i = 2047; i >= 0; i--)
    {
        Blocks[i] = MemoryBlock();
        FreeBlocks[2047 - i] = i;
    }
    FreeBlockCounter = 2047;
    CurrentBlock = FreeBlocks[FreeBlockCounter];
    Blocks[CurrentBlock] = MemoryBlock(malloc(BlockSize), CurrentBlock);
    FreeBlockCounter--;
    IsInit = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ResetMemory()
{
    for (uint32_t i = 0; i < 2048; i++)
    {
        if (Blocks[i].mem != nullptr)
        {
            free(Blocks[i].mem);
            Blocks[i] = MemoryBlock();
        }
    }
    InitMemory();
}

thread_local char ThreadLocalHeap[0xFFFF];
thread_local size_t ThreadLocalIterator = 0;