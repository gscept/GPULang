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

namespace GPULang
{

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

struct Allocator
{
    uint32_t freeBlockCounter;
    uint32_t* freeBlocks;
    MemoryBlock* blocks;
    size_t currentBlock;
    size_t blockSize;
};

inline Allocator CreateAllocator()
{
    return Allocator
    {
        .freeBlockCounter = 0,
        .freeBlocks = nullptr,
        .blocks = nullptr,
        .currentBlock = 0,
        .blockSize = 65535
    };
}

// The default allocator is used for all static initialization
extern Allocator DefaultAllocator;
extern bool IsDefaultAllocatorInitialized;

extern thread_local Allocator* CurrentAllocator;
void InitAllocator(Allocator* alloc);
void DestroyAllocator(Allocator* alloc);
void MakeAllocatorCurrent(Allocator* alloc);
void ResetAllocator(Allocator* alloc);

void Dealloc(void* alloc);


struct SPVArg;

//------------------------------------------------------------------------------
/**
*/
template<typename T, typename ... ARGS>
inline T*
Alloc(ARGS... args)
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
    const size_t size = sizeof(T);
    MemoryBlock* block = &Allocator->blocks[Allocator->currentBlock];
    if (block->iterator + size > Allocator->blockSize)
    {
        Allocator->currentBlock = Allocator->freeBlocks[Allocator->freeBlockCounter--];
        block = &Allocator->blocks[Allocator->currentBlock];
        block->blockIndex = Allocator->currentBlock;
        block->mem = malloc(Allocator->blockSize);
    }
    assert(block->blockIndex != -1);

    char* buf = (char*)block->mem + block->iterator;
    block->iterator += size;
    new (buf) T(args...);
    return (T*)buf;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, typename ... ARGS>
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

    const size_t size = sizeof(T) * num;
    MemoryBlock* block = &Allocator->blocks[Allocator->currentBlock];
    if (block->iterator + size > Allocator->blockSize)
    {
        Allocator->currentBlock = Allocator->freeBlocks[Allocator->freeBlockCounter--];
        block = &Allocator->blocks[Allocator->currentBlock];
        block->blockIndex = Allocator->currentBlock;
        block->mem = malloc(Allocator->blockSize);
    }
    assert(block->blockIndex != -1);

    char* buf = (char*)block->mem + block->iterator;
    block->iterator += size;
    new (buf) T();
    return (T*)buf;
}

//------------------------------------------------------------------------------
/**
*/
int constexpr 
const_len(const char* str)
{
    return *str ? 1 + const_len(str + 1) : 0;
}

struct ConstantString
{
    size_t size = 0;
    const char* buf;

    constexpr ConstantString()
        : size(0)
        , buf(nullptr)
    {}
    constexpr ConstantString(std::nullptr_t)
        : size(0)
        , buf(nullptr)
    {}

    constexpr ConstantString(const char* buf, size_t size)
    {
        this->buf = buf;
        this->size = size;
    }

    constexpr ConstantString(const char* buf)
    {
        this->buf = buf;
        this->size = const_len(buf);
    }
};

extern thread_local char ThreadLocalHeap[];
extern thread_local size_t ThreadLocalIterator;

template<typename TYPE>
TYPE* AllocStack(size_t count)
{
    TYPE* ptr = (TYPE*)(ThreadLocalHeap + ThreadLocalIterator);
    ThreadLocalIterator += count * sizeof(TYPE);
    memset(ptr, 0x0, count * sizeof(TYPE));
    return ptr;
}

template<typename TYPE>
void DeallocStack(size_t count, TYPE* buf)
{
    TYPE* topPtr = (TYPE*)(ThreadLocalHeap + ThreadLocalIterator - count * sizeof(TYPE));
    assert(topPtr == buf);
    ThreadLocalIterator -= count * sizeof(TYPE);
}

template<typename TYPE>
struct StackArray
{
    TYPE* ptr = nullptr;
    size_t size = 0;
    size_t capacity = 0;

    StackArray(size_t count)
    {
        if (count != 0)
        {
            this->ptr = AllocStack<TYPE>(count);
            this->capacity = count;
        }
    }

    ~StackArray()
    {
        if (this->ptr != nullptr)
            DeallocStack(this->capacity, this->ptr);
        this->capacity = 0;
        this->size = 0;
    }

    void Append(const TYPE& t)
    {
        assert(this->size + 1 <= this->capacity);
        this->ptr[this->size++] = t;
    }
};

inline size_t NumChars(int arg)
{
    int x = abs(arg);
    return (x < 10 ? 1 :
        (x < 100 ? 2 :
            (x < 1000 ? 3 :
            (x < 10000 ? 4 :
            (x < 100000 ? 5 :
            (x < 1000000 ? 6 :
            (x < 10000000 ? 7 :
            (x < 100000000 ? 8 :
            (x < 1000000000 ? 9 :
            10)))))))));
}

inline size_t NumChars(unsigned int arg)
{
    return (arg < 10 ? 1 :
        (arg < 100 ? 2 :
            (arg < 1000 ? 3 :
            (arg < 10000 ? 4 :
            (arg < 100000 ? 5 :
            (arg < 1000000 ? 6 :
            (arg < 10000000 ? 7 :
            (arg < 100000000 ? 8 :
            (arg < 1000000000 ? 9 :
            10)))))))));
}

inline size_t NumChars(size_t arg)
{
    return (arg < 10 ? 1 :
        (arg < 100 ? 2 :
            (arg < 1000 ? 3 :
            (arg < 10000 ? 4 :
            (arg < 100000 ? 5 :
            (arg < 1000000 ? 6 :
            (arg < 10000000 ? 7 :
            (arg < 100000000 ? 8 :
            (arg < 1000000000 ? 9 :
            10)))))))));
}

static const unsigned NUM_FLOAT_DIGITS = 256;

inline size_t NumChars(float arg)
{
    // 16 decimal points should suffice
    return NUM_FLOAT_DIGITS;
}

template<typename T>
inline size_t FragmentSize(T arg)
{
    assert(false && "Should never enter");
    //static_assert(false, "Should never enter");
    return 0;
}

template<>
inline size_t FragmentSize(nullptr_t)
{
    return 0;
}

template<>
inline size_t FragmentSize<int>(int arg)
{
    return NumChars(arg);
}

template<>
inline size_t FragmentSize<unsigned int>(unsigned int arg)
{
    return NumChars(arg);
}

template<>
inline size_t FragmentSize<char>(char arg)
{
    return NumChars(arg);
}


template<>
inline size_t FragmentSize<unsigned char>(unsigned char arg)
{
    return NumChars(arg);
}

template<>
inline size_t FragmentSize<float>(float arg)
{
    char buf[256];
    return snprintf(buf, 256, "%f", arg);
}

template<>
inline size_t FragmentSize<size_t>(size_t arg)
{
    return NumChars(arg);
}

template<>
inline constexpr size_t FragmentSize<const char*>(const char* str)
{
    return const_len(str);
}

template<>
inline size_t FragmentSize<const std::string&>(const std::string& str)
{
    return str.length();
}

template<>
inline size_t FragmentSize<std::string>(std::string str)
{
    return str.length();
}

template<>
inline size_t FragmentSize<ConstantString>(ConstantString str)
{
    return str.size;
}


template<typename T>
inline void FragmentString(T arg, char* buf, size_t size)
{
    assert(false);
}

template<>
inline void FragmentString<nullptr_t>(nullptr_t, char* buf, size_t size)
{
}

template<>
inline void FragmentString<int>(int arg, char* buf, size_t size)
{
    snprintf(buf, size, "%d", arg);
}

template<>
inline void FragmentString<unsigned int>(unsigned int arg, char* buf, size_t size)
{
    snprintf(buf, size, "%lu", arg);
}

template<>
inline void FragmentString<char>(char arg, char* buf, size_t size)
{
    snprintf(buf, size, "%hhd", arg);
}

template<>
inline void FragmentString<unsigned char>(unsigned char arg, char* buf, size_t size)
{
    snprintf(buf, size, "%hhu", arg);
}

template<>
inline void FragmentString<float>(float arg, char* buf, size_t size)
{
    snprintf(buf, size, "%f", arg);
}

template<>
inline void FragmentString<size_t>(size_t arg, char* buf, size_t size)
{
    snprintf(buf, size, "%zu", arg);
}

template<>
inline void FragmentString<const char*>(const char* arg, char* buf, size_t size)
{
    memcpy(buf, arg, const_len(arg));
}

template<>
inline void FragmentString<const std::string&>(const std::string& arg, char* buf, size_t size)
{
    memcpy(buf, arg.c_str(), arg.length());
}

template<>
inline void FragmentString<std::string>(std::string arg, char* buf, size_t size)
{
    memcpy(buf, arg.c_str(), arg.length());
}

template<>
inline void FragmentString<ConstantString>(ConstantString arg, char* buf, size_t size)
{
    memcpy(buf, arg.buf, arg.size);
}

struct TransientString
{
    char buf[2048];
    size_t capacity = 2048;
    size_t size = 0;

    TransientString()
        : size(0)
        , capacity(2048)
    {
    }

    // Copy constructor
    TransientString(const TransientString& rhs)
        : size(0)
        , capacity(2048)
    {
        *this = rhs;
    }

    TransientString(const char* buf)
        : size(0)
        , capacity(2048)
    {
        this->size = const_len(buf);
        memcpy(this->buf, buf, const_len(buf));
        this->buf[this->size] = '\0';
    }

    TransientString(const char* buf, std::size_t len)
        : size(0)
        , capacity(2048)
    {
        this->size = len;
        memcpy(this->buf, buf, len);
        this->buf[this->size] = '\0';
    }

    TransientString(const ConstantString& str)
        : size(0)
        , capacity(2048)
    {
        memcpy(this->buf, str.buf, str.size);
        this->size = str.size;
        this->buf[str.size] = '\0';
    }

    template<typename ...ARGS>
    TransientString(ARGS... args)
        : size(0)
        , capacity(2048)
    {
        this->Concatenate<false>(std::forward<ARGS>(args)...);
    }

    template<typename ...ARGS>
    static TransientString Separated(ARGS... args)
    {
        TransientString ret;
        ret.Concatenate<true>(std::forward<ARGS>(args)...);
        return ret;
    }

    template<typename ...ARGS>
    static TransientString Compact(ARGS... args)
    {
        return TransientString(std::forward<ARGS>(args)...);
    }

    void operator=(const TransientString& rhs)
    {
        if (rhs.buf != nullptr)
        {
            memcpy(this->buf, rhs.buf, rhs.size * sizeof(char));
            this->size = rhs.size;
            this->buf[this->size] = '\0';
        }
    }

    std::string ToString() const
    {
        return std::string(this->buf, this->size);
    }

    // Format contents, replaces contents in string
    void Format(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        this->size = vsnprintf(this->buf, this->capacity, format, args);
        va_end(args);
    }

    template<bool SPACE_SEPARATED, typename ...ARGS>
    void Concatenate(ARGS... args)
    {
        size_t numArgs = sizeof...(ARGS);
        size_t* subStringLengths = AllocStack<size_t>(numArgs);
        size_t size = this->size;

        // Setup the substring lengths and calculate full size
        int i = 0;
        ([&]
        {
            subStringLengths[i] = FragmentSize(args);
            size += subStringLengths[i];
            if constexpr (SPACE_SEPARATED)
            {
                if (i < numArgs)
                {
                    size += 1;
                }
            }
            i++;
        } (), ...);

        assert(size + 1 <= this->capacity);

        // Merge strings
        i = 0;
        ([&]
        {
            FragmentString(args, this->buf + this->size, this->capacity - this->size);
            this->size += subStringLengths[i];
            if constexpr (SPACE_SEPARATED)
            {
                if (i < numArgs - 1)
                {
                    this->buf[this->size] = ' ';
                    this->size += 1;
                }
            }
            i++;
        } (), ...);

        this->buf[this->size] = '\0';
        DeallocStack(numArgs, subStringLengths);
    }

    template<typename T, typename U>
    void Append(T arg, U argN) {}

    // Append
    template<>
    void Append<const char*, size_t>(const char* buf, size_t size)
    {
        if (buf == nullptr)
            return;
        assert(this->size + size < 2048);
        memcpy(this->buf + this->size, buf, size * sizeof(char));
        this->size += size;
        this->buf[this->size] = '\0';
    }

    template<typename T>
    void Append(T arg) 
    {
        assert(false && "Should never enter");
        //static_assert(false, "Should never enter");
    }
    
    template<>
    void Append<nullptr_t>(nullptr_t)
    {
        // do nothing
    }

    template<>
    void Append<const char*>(const char* buf)
    {
        memcpy(this->buf + this->size, buf, const_len(buf));
        this->size += const_len(buf);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<int>(int arg)
    {
        snprintf(this->buf + this->size, this->capacity - this->size, "%d", arg);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<std::string>(std::string arg)
    {
        memcpy(this->buf + this->size, arg.c_str(), arg.size());
        this->size += arg.size();
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<unsigned int>(unsigned int arg)
    {
        snprintf(this->buf + this->size, this->capacity - this->size, "%ul", arg);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<char>(char arg)
    {
        snprintf(this->buf + this->size, this->capacity - this->size, "%hhd", arg);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<unsigned char>(unsigned char arg)
    {
        snprintf(this->buf + this->size, this->capacity - this->size, "%hhu", arg);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<float>(float arg)
    {
        size_t written = snprintf(this->buf + this->size, this->capacity - this->size, "%f", arg);
        this->size += written;
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<size_t>(size_t arg)
    {
        snprintf(this->buf + this->size, this->capacity - this->size, "%zu", arg);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<GPULang::SPVArg>(GPULang::SPVArg arg);

    const char* Data() const
    {
        return buf;
    }
};

template<>
inline size_t FragmentSize<const TransientString&>(const TransientString& str)
{
    return str.size;
}

template<>
inline size_t FragmentSize<TransientString>(TransientString str)
{
    return str.size;
}

template<>
inline void FragmentString<TransientString>(TransientString arg, char* buf, size_t size)
{
    memcpy(buf, arg.buf, arg.size);
}

template<>
inline void FragmentString<const TransientString&>(const TransientString& arg, char* buf, size_t size)
{
    memcpy(buf, arg.buf, arg.size);
}

struct GrowingString
{
    char* data = nullptr;
    size_t capacity = 0;
    size_t size = 0;

    template<typename ...ARGS>
    void Line(ARGS... args)
    {
        size_t numArgs = sizeof...(ARGS);
        size_t* subStringLengths = AllocStack<size_t>(numArgs);
        size_t size = this->size;

        // Setup the substring lengths and calculate full size
        int i = 0;
        ([&]
        {
            subStringLengths[i] = FragmentSize(args);
            size += subStringLengths[i] + (i > 0 ? 1 : 0);
            i++;
        } (), ...);

        // Grow size of buffer as needed
        size_t newSize = this->size + size;
        if (this->capacity <= newSize)
        {
            newSize = std::min((size_t)0xFFFFFFFu, newSize << 1);
            char* newData = (char*)malloc(newSize * sizeof(char) + 2);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }

        // Merge strings
        i = 0;
        ([&]
        {
            if (i == 0)
            {
                FragmentString(args, this->data + this->size, this->capacity - this->size);
                this->size += subStringLengths[i];
            }
            else
            {
                this->data[this->size] = ' ';
                FragmentString(args, this->data + this->size + 1, this->capacity - this->size - 1);
                this->size += subStringLengths[i] + 1;
            }
            i++;
        } (), ...);

        this->data[this->size] = '\n';
        this->data[this->size + 1] = '\0';
        this->size += 1;
        DeallocStack(numArgs, subStringLengths);
    }

    template<typename ...ARGS>
    void CompactLine(ARGS... args)
    {
        size_t numArgs = sizeof...(ARGS);
        size_t* subStringLengths = AllocStack<size_t>(numArgs);
        size_t size = this->size;

        // Setup the substring lengths and calculate full size
        int i = 0;
        ([&]
        {
            subStringLengths[i] = FragmentSize(args);
            size += subStringLengths[i];
            i++;
        } (), ...);

        // Grow size of buffer as needed
        size_t newSize = this->size + size;
        if (this->capacity <= newSize)
        {
            newSize = std::min((size_t)0xFFFFFFFu, newSize << 1);
            char* newData = (char*)malloc(newSize * sizeof(char) + 2);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }

        // Merge strings
        i = 0;
        ([&]
        {
            FragmentString(args, this->data + this->size, this->capacity - this->size);
            this->size += subStringLengths[i];
            i++;
        } (), ...);

        this->data[this->size] = '\n';
        this->data[this->size + 1] = '\0';
        this->size += 1;
        DeallocStack(numArgs, subStringLengths);
    }

    void Append(const char* str)
    {
        if (str == nullptr)
            return;
        size_t len = const_len(str);
        size_t newSize = this->size + len;
        if (this->capacity <= newSize)
        {
            newSize = std::min((size_t)0xFFFFFFFu, newSize << 1);
            char* newData = (char*)malloc(newSize * sizeof(char) + 2);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }
        memcpy(this->data + this->size, str, len * sizeof(char));
        this->size += len;
        this->data[this->size] = '\0';
    }

    void Append(const char* str, size_t len)
    {
        if (str == nullptr)
            return;
        size_t newSize = this->size + len;
        if (this->capacity <= newSize)
        {
            newSize = std::min((size_t)0xFFFFFFFu, newSize << 1);
            char* newData = (char*)malloc(newSize * sizeof(char) + 2);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }
        memcpy(this->data + this->size, str, len * sizeof(char));
        this->size += len;
        this->data[this->size] = '\0';
    }

    void Append(const TransientString& arg)
    {
        size_t newSize = this->size + arg.size;
        if (this->capacity <= newSize)
        {
            newSize = std::min((size_t)0xFFFFFFFu, newSize << 1);
            char* newData = (char*)malloc(newSize * sizeof(char) + 2);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }
        memcpy(this->data + this->size, arg.buf, arg.size * sizeof(char));
        this->size += arg.size;
        this->data[this->size] = '\0';
    }

    void Append(const GrowingString& arg)
    {
        size_t newSize = this->size + arg.size;
        if (this->capacity <= newSize)
        {
            newSize = std::min((size_t)0xFFFFFFFu, newSize << 1);
            char* newData = (char*)malloc(newSize * sizeof(char) + 2);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }
        memcpy(this->data + this->size, arg.data, arg.size * sizeof(char));
        this->size += arg.size;
        this->data[this->size] = '\0';
    }
    

    void Clear()
    {
        // Don't free memory
        this->size = 0;
    }
};

using TStr = TransientString;
using GStr = GrowingString;

} // namespace GPULang
