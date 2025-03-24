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

namespace GPULang
{
struct SPVArg;
}

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

//------------------------------------------------------------------------------
/**
*/
template<typename T, typename ... ARGS>
inline T*
AllocArray(std::size_t num)
{
    if (!IsInit)
        InitMemory();

    const size_t size = sizeof(T) * num;
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
    new (buf) T();
    return (T*)buf;
}

int constexpr const_len(const char* str)
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
    static_assert(false, "Should never enter");
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
    return 1;
}

template<>
inline size_t FragmentSize<float>(float arg)
{
    char buf[256];
    std::to_chars_result res = std::to_chars(buf, buf + NUM_FLOAT_DIGITS, arg);
    return res.ptr - buf;
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

static const char* SPACE = " ";

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
            FragmentString(args);
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
        static_assert(false, "Should never enter");
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
        _itoa_s(arg, this->buf + this->size, this->capacity - this->size, 10);
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
        _ultoa_s(arg, this->buf + this->size, this->capacity - this->size, 10);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<char>(char arg)
    {
        this->buf[this->size] = arg;
        this->size += 1;
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<float>(float arg)
    {
        std::to_chars_result res = std::to_chars(this->buf + this->size, this->buf + this->size + NUM_FLOAT_DIGITS, arg);
        this->size += res.ptr - (this->buf + this->size);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<size_t>(size_t arg)
    {
        _ui64toa_s(arg, this->buf + this->size, this->capacity - this->size, 10);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append<GPULang::SPVArg>(GPULang::SPVArg arg);

    const char* Data() const
    {
        return buf;
    }

    template<typename T>
    void FragmentString(T arg)
    {
        assert(false);
    }

    template<>
    void FragmentString<nullptr_t>(nullptr_t)
    {
    }

    template<>
    void FragmentString<int>(int arg)
    {
        errno_t err = _itoa_s(arg, this->buf + this->size, this->capacity - this->size, 10);
        assert(err != EINVAL);
    }

    template<>
    void FragmentString<unsigned int>(unsigned int arg)
    {
        errno_t err = _ultoa_s(arg, this->buf + this->size, this->capacity - this->size, 10);
        assert(err != EINVAL);
    }

    template<>
    void FragmentString<char>(char arg)
    {
        this->buf[this->size] = arg;
    }

    template<>
    void FragmentString<float>(float arg)
    {
        std::to_chars(this->buf + this->size, this->buf + this->size + NUM_FLOAT_DIGITS, arg);
    }

    template<>
    void FragmentString<size_t>(size_t arg)
    {
        errno_t err = _ui64toa_s(arg, this->buf + this->size, this->capacity - this->size, 10);
        assert(err != EINVAL);
    }

    template<>
    void FragmentString<const char*>(const char* arg)
    {
        if (arg != nullptr)
            memcpy(this->buf + this->size, arg, const_len(arg));
    }

    template<>
    void FragmentString<const std::string&>(const std::string& arg)
    {
        memcpy(this->buf + this->size, arg.c_str(), arg.length());
    }

    template<>
    void FragmentString<std::string>(std::string arg)
    {
        memcpy(this->buf + this->size, arg.c_str(), arg.length());
    }

    template<>
    void FragmentString<TransientString>(TransientString arg)
    {
        memcpy(this->buf + this->size, arg.buf, arg.size);
    }

    template<>
    void FragmentString<const TransientString&>(const TransientString& arg)
    {
        memcpy(this->buf + this->size, arg.buf, arg.size);
    }

    template<>
    void FragmentString<ConstantString>(ConstantString arg)
    {
        memcpy(this->buf + this->size, arg.buf, arg.size);
    }

    template<>
    void FragmentString<const ConstantString&>(const ConstantString& arg)
    {
        memcpy(this->buf + this->size, arg.buf, arg.size);
    }

    template<>
    void FragmentString<GPULang::SPVArg>(GPULang::SPVArg arg);
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

    template<typename T>
    void FragmentString(T arg, char* buf, size_t size)
    {
        assert(false);
    }

    template<>
    void FragmentString<nullptr_t>(nullptr_t, char* buf, size_t size)
    {
    }

    template<>
    void FragmentString<int>(int arg, char* buf, size_t size)
    {
        _itoa_s(arg, buf, size, 10);
    }

    template<>
    void FragmentString<unsigned int>(unsigned int arg, char* buf, size_t size)
    {
        _ultoa_s(arg, buf, size, 10);
    }

    template<>
    void FragmentString<char>(char arg, char* buf, size_t size)
    {
        buf[size] = arg;
    }

    template<>
    void FragmentString<float>(float arg, char* buf, size_t size)
    {
        std::to_chars(buf, buf + size, arg);
    }

    template<>
    void FragmentString<size_t>(size_t arg, char* buf, size_t size)
    {
        _ui64toa_s(arg, buf, size, 10);
    }

    template<>
    void FragmentString<const char*>(const char* arg, char* buf, size_t size)
    {
        memcpy(buf, arg, const_len(arg));
    }

    template<>
    void FragmentString<const std::string&>(const std::string& arg, char* buf, size_t size)
    {
        memcpy(buf, arg.c_str(), arg.length());
    }

    template<>
    void FragmentString<std::string>(std::string arg, char* buf, size_t size)
    {
        memcpy(buf, arg.c_str(), arg.length());
    }

    template<>
    void FragmentString<ConstantString>(ConstantString arg, char* buf, size_t size)
    {
        memcpy(buf, arg.buf, arg.size);
    }

    template<>
    void FragmentString<TransientString>(TransientString arg, char* buf, size_t size)
    {
        memcpy(buf, arg.buf, arg.size);
    }

    template<>
    void FragmentString<const TransientString&>(const TransientString& arg, char* buf, size_t size)
    {
        memcpy(buf, arg.buf, arg.size);
    }

    template<>
    void FragmentString<GPULang::SPVArg>(GPULang::SPVArg arg, char* buf, size_t size);
};


using TStr = TransientString;
using GStr = GrowingString;