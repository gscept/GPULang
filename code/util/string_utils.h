#pragma once
//------------------------------------------------------------------------------
/**
    @file strings.h

    String types

    2024 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "memory.h"
#include <algorithm>

namespace GPULang
{
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
    const char* buf;
    size_t size;

    constexpr ConstantString()
        : buf(nullptr)
        , size(0)
    {
    }
    constexpr ConstantString(std::nullptr_t)
        : buf(nullptr)
        , size(0)
    {
    }

    template<int num>
    constexpr ConstantString(const char (&buf)[num])
    {
        this->buf = buf;
        this->size = num;
    }
    
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
    
    constexpr bool operator<(const ConstantString& rhs) const
    {
        size_t minSize = (this->size < rhs.size) ? this->size : rhs.size;
        for (size_t i = 0; i < minSize; i++)
        {
            if (this->buf[i] < rhs.buf[i]) return true;
            if (this->buf[i] > rhs.buf[i]) return false;
        }
        return this->size < rhs.size;
       
    }
    
    constexpr bool operator==(const ConstantString& rhs) const
    {
        if (this->size != rhs.size)
            return false;
        for (size_t i = 0; i < this->size && i < rhs.size; i++)
        {
            if (this->buf[i] != rhs.buf[i]) return false;
        }
        return true;
    }
    
    constexpr const char* c_str() const
    {
        return this->buf;
    }
};

inline constexpr ConstantString operator ""_c(const char* buf, const size_t len)
{
    return ConstantString{ buf, len };
}

//------------------------------------------------------------------------------
/**
*/
inline size_t
NumChars(int arg)
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

//------------------------------------------------------------------------------
/**
*/
inline size_t
NumChars(unsigned int arg)
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

//------------------------------------------------------------------------------
/**
*/
inline size_t
NumChars(size_t arg)
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

inline size_t
NumChars(float arg)
{
    // 16 decimal points should suffice
    return NUM_FLOAT_DIGITS;
}

template<typename T>
inline size_t
FragmentSize(T arg)
{
    assert(false && "Should never enter");
    return 0;
}

template<>
inline size_t
FragmentSize(std::nullptr_t)
{
    return 0;
}

template<>
inline size_t
FragmentSize(int arg)
{
    return NumChars(arg);
}

template<>
inline size_t
FragmentSize(unsigned int arg)
{
    return NumChars(arg);
}

template<>
inline size_t
FragmentSize(char arg)
{
    return 1;
}

template<>
inline size_t
FragmentSize(unsigned char arg)
{
    return NumChars(arg);
}

template<>
inline size_t
FragmentSize(float arg)
{
    char buf[256];
    return snprintf(buf, 256, "%f", arg);
}

template<>
inline size_t
FragmentSize(size_t arg)
{
    return NumChars(arg);
}

template<>
inline constexpr size_t
FragmentSize(char* str)
{
    return const_len(str);
}

template<>
inline constexpr size_t
FragmentSize(const char* str)
{
    return const_len(str);
}

template<int SIZE>
inline constexpr size_t
FragmentSize(const char (&str)[SIZE])
{
    return SIZE-1;
}

template<>
inline size_t
FragmentSize(const std::string& str)
{
    return str.length();
}

template<>
inline size_t
FragmentSize(std::string str)
{
    return str.length();
}

template<>
inline size_t
FragmentSize(std::string_view str)
{
    return str.length();
}

template<>
inline size_t
FragmentSize(ConstantString str)
{
    return str.size;
}

template<typename T>
inline void
FragmentString(T arg, char* buf, size_t size)
{
    assert(false && "Should never enter");
}

template<>
inline void
FragmentString(std::nullptr_t, char* buf, size_t size)
{
}

template<>
inline void
FragmentString(int arg, char* buf, size_t size)
{
    snprintf(buf, size, "%d", arg);
}

template<>
inline void
FragmentString(bool arg, char* buf, size_t size)
{
    const char* value = arg ? "true" : "false";
    memcpy(buf, value, arg ? 4 : 5);
}

template<>
inline void
FragmentString(unsigned int arg, char* buf, size_t size)
{
    snprintf(buf, size, "%lu", arg);
}

template<>
inline void
FragmentString(char arg, char* buf, size_t size)
{
    *buf = arg;
}

template<>
inline void
FragmentString(unsigned char arg, char* buf, size_t size)
{
    snprintf(buf, size, "%hhu", arg);
}

template<>
inline void
FragmentString(float arg, char* buf, size_t size)
{
    snprintf(buf, size, "%f", arg);
}

template<>
inline void
FragmentString(size_t arg, char* buf, size_t size)
{
    snprintf(buf, size, "%zu", arg);
}

template<>
inline void
FragmentString(const char* arg, char* buf, size_t size)
{
    memcpy(buf, arg, const_len(arg));
}

template<int SIZE>
inline void
FragmentString(const char (&arg)[SIZE], char* buf, size_t size)
{
    memcpy(buf, arg, SIZE-1);
}

template<>
inline void
FragmentString(char* arg, char* buf, size_t size)
{
    memcpy(buf, arg, const_len(arg));
}

template<>
inline void
FragmentString(const std::string_view& arg, char* buf, size_t size)
{
    memcpy(buf, arg.data(), arg.length());
}

template<>
inline void
FragmentString(std::string_view arg, char* buf, size_t size)
{
    memcpy(buf, arg.data(), arg.length());
}

template<>
inline void
FragmentString(const std::string& arg, char* buf, size_t size)
{
    memcpy(buf, arg.c_str(), arg.length());
}

template<>
inline void
FragmentString(std::string arg, char* buf, size_t size)
{
    memcpy(buf, arg.c_str(), arg.length());
}

template<>
inline void
FragmentString(ConstantString arg, char* buf, size_t size)
{
    memcpy(buf, arg.buf, arg.size);
}


struct FixedString;
struct TransientString
{
    char* buf;
    char chars[2048];
    size_t capacity = 2048;
    size_t size = 0;

    TransientString()
        : capacity(2048)
        , size(0)
    {
        this->buf = this->chars;
    }

    // Copy constructor
    TransientString(const TransientString& rhs)
        : capacity(2048)
        , size(0)
    {
        this->buf = this->chars;
        if (rhs.buf != rhs.chars)
        {
            this->buf = new char[rhs.capacity];
            memcpy(this->buf, rhs.buf, rhs.size);
        }
        *this = rhs;
    }

    TransientString(const char* buf)
        : capacity(2048)
        , size(0)
    {
        this->buf = this->chars;
        this->size = const_len(buf);
        if (this->size > this->capacity)
        {
            this->buf = new char[this->size+1];
            this->capacity = this->size;
        }
        memcpy(this->buf, buf, this->size);
        this->buf[this->size] = '\0';
    }


    TransientString(const char* buf, std::size_t len)
        : capacity(2048)
        , size(0)
    {
        this->buf = this->chars;
        this->size = len;
        if (this->size > this->capacity)
        {
            this->buf = new char[this->size+1];
            this->capacity = this->size;
        }
        memcpy(this->buf, buf, len);
        this->buf[this->size] = '\0';
    }
    
    TransientString(char* buf, std::size_t len)
        : capacity(2048)
        , size(0)
    {
        this->buf = this->chars;
        this->size = len;
        if (this->size > this->capacity)
        {
            this->buf = new char[this->size+1];
            this->capacity = this->size;
        }
        memcpy(this->buf, buf, len);
        this->buf[this->size] = '\0';
    }

    TransientString(const ConstantString& str)
        : capacity(2048)
        , size(0)
    {
        assert(str.size <= sizeof(this->chars));
        this->buf = this->chars;
        memcpy(this->buf, str.buf, str.size);
        this->size = str.size;
        this->buf[str.size] = '\0';
    }

    template<typename ...ARGS>
    TransientString(const ARGS&... args)
        : capacity(2048)
        , size(0)
    {
        this->buf = this->chars;
        this->Concatenate<false>(std::forward<const ARGS&>(args)...);
    }

    ~TransientString()
    {
        if (this->buf != nullptr && this->buf != this->chars)
        {
            delete[] this->buf;
        }
        this->capacity = 2048;
        this->size = 0;
        this->buf = this->chars;
    }

    template<typename ...ARGS>
    static TransientString Separated(const ARGS&... args)
    {
        TransientString ret;
        ret.Concatenate<true>(std::forward<const ARGS&>(args)...);
        return ret;
    }

    template<typename ...ARGS>
    static TransientString Compact(const ARGS&... args)
    {
        return TransientString(std::forward<const ARGS&>(args)...);
    }

    void operator=(const TransientString& rhs)
    {
        if (this->buf != nullptr && this->buf != this->chars)
        {
            delete[] this->buf;
        }
        this->buf = this->chars;
        memcpy(this->buf, rhs.buf, rhs.size * sizeof(char));
        this->size = rhs.size;
        this->buf[this->size] = '\0';
    }

    std::string ToString() const
    {
        return std::string(this->buf, this->size);
    }

    std::string_view ToView() const
    {
        return std::string_view(this->buf, this->size);
    }
    
    const char* c_str() const
    {
        return this->buf;
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
    void Concatenate(const ARGS&... args)
    {
        size_t numArgs = sizeof...(ARGS);
        size_t numBytes = 0;
        size_t* subStringLengths = AllocStack<size_t>(numArgs, numBytes);
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

        if (size + 1 > this->capacity)
        {
            this->buf = new char[size + 1];
            this->capacity = size + 1;
        }

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
        DeallocStack(numArgs, numArgs, subStringLengths, numBytes);
    }
    
    bool operator==(const char* buf) const
    {
        return strcmp(this->buf, buf) == 0;
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
        //assert(false && "Should never enter");
        assert(false && "Should never enter");
    }

    template<>
    void Append<std::nullptr_t>(std::nullptr_t)
    {
        // do nothing
    }

    template<>
    void Append(const char* buf)
    {
        memcpy(this->buf + this->size, buf, const_len(buf));
        this->size += const_len(buf);
        this->buf[this->size] = '\0';
    }
    
    template<int SIZE>
    void Append(const char (&str)[SIZE])
    {
        memcpy(this->buf + this->size, str, SIZE-1);
        this->size += SIZE-1;
        this->buf[this->size] = '\0';
    }

    template<>
    void Append(int arg)
    {
        snprintf(this->buf + this->size, this->capacity - this->size, "%d", arg);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append(std::string arg)
    {
        memcpy(this->buf + this->size, arg.c_str(), arg.size());
        this->size += arg.size();
        this->buf[this->size] = '\0';
    }

    template<>
    void Append(std::string_view arg)
    {
        memcpy(this->buf + this->size, arg.data(), arg.size());
        this->size += arg.size();
        this->buf[this->size] = '\0';
    }

    template<>
    void Append(unsigned int arg)
    {
        snprintf(this->buf + this->size, this->capacity - this->size, "%ul", arg);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append(char arg)
    {
        this->buf[this->size] = arg;
        this->size++;
        this->buf[this->size] = '\0';
    }

    template<>
    void Append(unsigned char arg)
    {
        snprintf(this->buf + this->size, this->capacity - this->size, "%hhu", arg);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append(float arg)
    {
        size_t written = snprintf(this->buf + this->size, this->capacity - this->size, "%f", arg);
        this->size += written;
        this->buf[this->size] = '\0';
    }

    template<>
    void Append(size_t arg)
    {
        snprintf(this->buf + this->size, this->capacity - this->size, "%zu", arg);
        this->size += NumChars(arg);
        this->buf[this->size] = '\0';
    }

    template<>
    void Append(GPULang::SPVArg arg);
    
    template<>
    void Append(TransientString arg);
    
    template<>
    void Append(const TransientString& arg);
    
    template<>
    void Append(FixedString arg);
    
    template<>
    void Append(const FixedString& arg);
    
    TransientString operator+(const TransientString& rhs)
    {
        TransientString ret;
        ret.Append(*this);
        ret.Append(rhs);
        return ret;
    }
    
    TransientString operator+(const char* rhs)
    {
        TransientString ret;
        ret.Append(*this);
        ret.Append(rhs);
        return ret;
    }
    
    TransientString& operator+=(const TransientString& rhs)
    {
        this->Append(rhs);
        return *this;
    }
    
    bool operator<(const TransientString& rhs) const
    {
        return strcmp(this->buf, rhs.buf) < 0;
    }

    bool operator<(const std::string_view &rhs) const
    {
        return rhs.compare(this->buf) > 0;
    }
    
    bool operator==(const TransientString& rhs) const
    {
        return strcmp(this->buf, rhs.buf) == 0;
    }
    
    bool operator!=(const TransientString& rhs) const
    {
        return strcmp(this->buf, rhs.buf) != 0;
    }

    bool operator==(const ConstantString& rhs) const
    {
        return strcmp(this->buf, rhs.buf) == 0;
    }

    const char* Data() const
    {
        return buf;
    }
};

template<>
inline void
TransientString::Append(TransientString arg)
{
    memcpy(this->buf + this->size, arg.buf, arg.size);
    this->size += arg.size;
    this->buf[this->size] = '\0';
}

template<>
inline void
TransientString::Append(const TransientString& arg)
{
    memcpy(this->buf + this->size, arg.buf, arg.size);
    this->size += arg.size;
    this->buf[this->size] = '\0';
}


template<>
inline size_t
FragmentSize(bool b)
{
    return sizeof(bool);
}

template<>
inline size_t
FragmentSize(const TransientString& str)
{
    return str.size;
}

template<>
inline size_t
FragmentSize(TransientString str)
{
    return str.size;
}

template<>
inline void
FragmentString(TransientString arg, char* buf, size_t size)
{
    memcpy(buf, arg.buf, arg.size);
}

template<>
inline void
FragmentString(const TransientString& arg, char* buf, size_t size)
{
    memcpy(buf, arg.buf, arg.size);
}
extern thread_local Allocator* StringAllocator;

struct FixedString
{
    FixedString()
        : buf(nullptr)
        , len(0)
    {
    }

    FixedString(const char* buf)
    {
        size_t len = strlen(buf) + 1;
        this->buf = AllocArray<char>(StringAllocator, len);
        this->len = len - 1;
        memcpy(this->buf, buf, len - 1);
        this->buf[this->len] = '\0';
    }

    FixedString(const char* start, const char* end)
    {
        size_t len = (end - start) + 1;
        this->buf = AllocArray<char>(StringAllocator, len);
        this->len = len - 1;
        memcpy(this->buf, start, len - 1);
        this->buf[this->len] = '\0';
    }

    explicit FixedString(const std::string& str)
    {
        size_t len = str.length() + 1;
        this->buf = AllocArray<char>(StringAllocator, len);
        this->len = len - 1;
        memcpy(this->buf, str.data(), len - 1);
        this->buf[this->len] = '\0';
    }

    explicit FixedString(const TransientString& str)
    {
        size_t len = str.size + 1;
        this->buf = AllocArray<char>(StringAllocator, len);
        this->len = len - 1;
        memcpy(this->buf, str.buf, len - 1);
        this->buf[this->len] = '\0';
    }

    explicit FixedString(const std::string_view& str)
    {
        size_t len = str.length() + 1;
        this->buf = AllocArray<char>(StringAllocator, len);
        this->len = len - 1;
        memcpy(this->buf, str.data(), len - 1);
        this->buf[this->len] = '\0';
    }

    explicit FixedString(const ConstantString& str)
    {
        this->buf = const_cast<char*>(str.buf);
        this->len = str.size;
    }

    FixedString(const FixedString& rhs)
    {
        this->buf = rhs.buf;
        this->len = rhs.len;
    }

    void operator=(const FixedString& rhs)
    {
        this->buf = rhs.buf;
        this->len = rhs.len;
    }

    void operator=(const char* buf)
    {
        size_t len = strlen(buf);
        this->len = len;
        this->buf = nullptr;
        if (len > 0)
        {
            this->buf = AllocArray<char>(StringAllocator, len + 1);
            memcpy(this->buf, buf, len);
            this->buf[this->len] = '\0';
        }
    }

    void operator=(const std::string& str)
    {
        size_t len = str.length();
        this->len = len;
        this->buf = nullptr;
        if (!str.empty())
        {
            this->buf = AllocArray<char>(StringAllocator, len + 1);
            memcpy(this->buf, str.data(), len);
            this->buf[this->len] = '\0';
        }
    }

    void operator=(const std::string_view& str)
    {
        size_t len = str.length();
        this->len = len;
        this->buf = nullptr;
        if (!str.empty())
        {
            this->buf = AllocArray<char>(StringAllocator, len + 1);
            memcpy(this->buf, str.data(), len);
            this->buf[this->len] = '\0';
        }
    }

    void operator=(const TransientString& str)
    {
        size_t len = str.size;
        this->len = len;
        this->buf = nullptr;
        if (str.size != 0)
        {
            this->buf = AllocArray<char>(StringAllocator, len + 1);
            memcpy(this->buf, str.buf, len);
            this->buf[this->len] = '\0';
        }
    }

    void operator=(const ConstantString& str)
    {
        this->len = str.size;
        this->buf = const_cast<char*>(str.buf);
    }

    FixedString(FixedString&& rhs) noexcept
    {
        this->buf = rhs.buf;
        this->len = rhs.len;
        rhs.buf = nullptr;
        rhs.len = 0;
    }

    void operator=(FixedString&& rhs) noexcept
    {
        this->buf = rhs.buf;
        this->len = rhs.len;
        rhs.buf = nullptr;
        rhs.len = 0;
    }

    bool operator==(const char* cmp) const
    {
        return strcmp(this->buf, cmp) == 0;
    }

    bool operator<(const FixedString& rhs) const
    {
        size_t min_size = this->len < rhs.len ? this->len : rhs.len;
        for (size_t i = 0; i < min_size; i++)
        {
            char lhs_char = this->buf[i];
            char rhs_char = rhs.buf[i];

            if (lhs_char != rhs_char)
                return lhs_char < rhs_char;
        }
        return this->len < rhs.len;
    }

    bool operator<(const std::string_view& rhs) const
    {
        size_t min_size = this->len < rhs.length() ? this->len : rhs.length();
        for (size_t i = 0; i < min_size; i++)
        {
            char lhs_char = this->buf[i];
            char rhs_char = rhs.data()[i];

            if (lhs_char != rhs_char)
                return lhs_char < rhs_char;
        }
        return this->len < rhs.length();
    }

    bool operator==(const FixedString& rhs) const
    {
        if (this->len != rhs.len)
            return false;

        for (size_t i = 0; i < this->len; i++)
        {
            char lhs_char = this->buf[i];
            char rhs_char = rhs.buf[i];

            if (lhs_char != rhs_char)
                return false;
        }
        return true;
    }

    bool operator==(const ConstantString& rhs) const
    {
        if (this->len != rhs.size)
            return false;

        for (size_t i = 0; i < this->len; i++)
        {
            char lhs_char = this->buf[i];
            char rhs_char = rhs.buf[i];

            if (lhs_char != rhs_char)
                return false;
        }
        return true;
    }

    bool operator==(const std::string& rhs) const
    {
        if (this->len != rhs.size())
            return false;

        for (size_t i = 0; i < this->len; i++)
        {
            char lhs_char = this->buf[i];
            char rhs_char = rhs.data()[i];

            if (lhs_char != rhs_char)
                return false;
        }
        return true;
    }

    bool StartsWith(const char* str) const
    {
        if (this->buf == nullptr)
        {
            if (strlen(str) == 0)
                return true;
            else
                return false;
        }
        return strncmp(this->buf, str, strlen(str)) == 0;
    }

    bool StartsWith(const std::string_view& str) const
    {
        if (this->buf == nullptr)
        {
            if (str.empty())
                return true;
            else
                return false;
        }
        return strncmp(this->buf, str.data(), str.length()) == 0;
    }

    bool StartsWith(const std::string& str) const
    {
        if (this->buf == nullptr)
        {
            if (str.empty())
                return true;
            else
                return false;
        }
        return strncmp(this->buf, str.data(), str.length()) == 0;
    }

    const char* c_str() const { return this->buf; }

    std::string_view ToView() const { return std::string_view(this->buf, this->len); }

    char* buf;
    size_t len;
};

struct GrowingString
{
    char* data = nullptr;
    size_t capacity = 0;
    size_t size = 0;

    template<typename ...ARGS>
    void Line(const ARGS&... args)
    {
        size_t numArgs = sizeof...(ARGS);
        size_t numBytes = 0;
        size_t* subStringLengths = AllocStack<size_t>(numArgs, numBytes);
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
        DeallocStack(numArgs, numArgs, subStringLengths, numBytes);
    }

    template<typename ...ARGS>
    void CompactLine(const ARGS&... args)
    {
        size_t numArgs = sizeof...(ARGS);
        size_t numBytes = 0;
        size_t* subStringLengths = AllocStack<size_t>(numArgs, numBytes);
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
        DeallocStack(numArgs, numArgs, subStringLengths, numBytes);
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
            assert(newData != nullptr);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }
        memcpy(this->data + this->size, str, len * sizeof(char));
        this->size += len;
        this->data[this->size] = '\0';
    }

    std::string_view ToView()
    {
        return std::string_view(this->data, this->size);
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
            assert(newData != nullptr);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }
        memcpy(this->data + this->size, str, len * sizeof(char));
        this->size += len;
        this->data[this->size] = '\0';
    }

    void Append(const char* begin, const char* end)
    {
        if (begin == nullptr)
            return;
        size_t strlen = (end - begin);
        size_t newSize = this->size + strlen;
        if (this->capacity <= newSize)
        {
            newSize = std::min((size_t)0xFFFFFFFu, newSize << 1);
            char* newData = (char*)malloc(newSize * sizeof(char) + 2);
            assert(newData != nullptr);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }
        memcpy(this->data + this->size, begin, strlen * sizeof(char));
        this->size += strlen;
        this->data[this->size] = '\0';
    }

    void Append(const TransientString& arg)
    {
        size_t newSize = this->size + arg.size;
        if (this->capacity <= newSize)
        {
            newSize = std::min((size_t)0xFFFFFFFu, newSize << 1);
            char* newData = (char*)malloc(newSize * sizeof(char) + 2);
            assert(newData != nullptr);
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
            assert(newData != nullptr);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }
        memcpy(this->data + this->size, arg.data, arg.size * sizeof(char));
        this->size += arg.size;
        this->data[this->size] = '\0';
    }

    void Append(const FixedString& arg)
    {
        size_t newSize = this->size + arg.len;
        if (this->capacity <= newSize)
        {
            newSize = std::min((size_t)0xFFFFFFFu, newSize << 1);
            char* newData = (char*)malloc(newSize * sizeof(char) + 2);
            assert(newData != nullptr);
            memcpy(newData, this->data, sizeof(char) * this->size);
            this->data = newData;
            this->capacity = newSize;
        }
        memcpy(this->data + this->size, arg.buf, arg.len * sizeof(char));
        this->size += arg.len;
        this->data[this->size] = '\0';
    }

    void Clear()
    {
        // Don't free memory
        this->size = 0;
    }

    ~GrowingString()
    {
        free(this->data);
    }
};

template<>
inline size_t
FragmentSize<FixedString>(FixedString str)
{
    return str.len;
}

template<>
inline void
FragmentString<FixedString>(FixedString arg, char* buf, size_t size)
{
    memcpy(buf, arg.buf, arg.len);
}

template<>
inline void
TransientString::Append(FixedString arg)
{
    memcpy(this->buf + this->size, arg.buf, arg.len);
    this->size += arg.len;
    this->buf[this->size] = '\0';
}

template<>
inline void
TransientString::Append(const FixedString& arg)
{
    memcpy(this->buf + this->size, arg.buf, arg.len);
    this->size += arg.len;
    this->buf[this->size] = '\0';
}

using TStr = TransientString;
using GStr = GrowingString;

constexpr uint64_t fnv1a_64(const char* str, std::size_t len) {
    uint64_t hash = 14695981039346656037ull; // FNV offset basis (64-bit)
    for (std::size_t i = 0; i < len; ++i) {
        hash ^= static_cast<uint64_t>(static_cast<unsigned char>(str[i]));
        hash *= 1099511628211ull; // FNV prime (64-bit)
    }
    return hash;
}

constexpr uint64_t hash_str(const char* str, std::size_t len) {
    return fnv1a_64(str, len);
}

constexpr uint64_t hash_str(const std::string_view& str) {
    return fnv1a_64(str.data(), str.size());
}

constexpr uint64_t hash_str(const TransientString& str) {
    return fnv1a_64(str.buf, str.size);
}

constexpr uint64_t hash_str(const FixedString& str) {
    return fnv1a_64(str.buf, str.len);
}

struct HashString
{
    uint64_t hash;
    
    constexpr HashString()
        : hash(0)
    {
    }
    
    constexpr HashString(std::nullptr_t)
        : hash(0)
    {
    }

    template<int num>
    constexpr HashString(const char (&buf)[num])
    {
        this->hash = hash_str(buf, num);
    }
    
    constexpr HashString(const char* buf, size_t size)
    {
        this->hash = hash_str(buf, size);
    }
    
    constexpr HashString(const TransientString& str)
    {
        this->hash = hash_str(str);
    }
    
    constexpr HashString(const FixedString& str)
    {
        this->hash = hash_str(str);
    }
    
    constexpr HashString(const std::string_view& str)
    {
        this->hash = hash_str(str);
    }
    
    constexpr bool operator<(const HashString& rhs) const
    {
        return this->hash < rhs.hash;
    }
    
    constexpr bool operator==(const HashString& rhs) const
    {
        return this->hash == rhs.hash;
    }
};

inline constexpr HashString operator ""_h(const char* buf, const size_t len)
{
    return HashString{ buf, len };
}

//------------------------------------------------------------------------------
/**
*/
inline const TransientString
FourCCToString(uint32_t fourCC)
{
    char buf[4];
    char usedBytes = 0;
    uint32_t masks[4] =
    {
        0x000000FF
        , 0x0000FF00
        , 0x00FF0000
        , 0xFF000000
    };
    for (int i = 0, shift = 0; i < 4; i++)
    {
        char c = (char)((fourCC & masks[i]) >> shift);
        if (c != 0x0)
            usedBytes++;
        buf[i] = c;
        shift += 8;
    }
    std::reverse(&buf[0], &buf[usedBytes]);
    TransientString ret(buf, (std::size_t)usedBytes);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
inline uint32_t
StringToFourCC(const std::string& str)
{
    uint32_t fourcc = 0;
    uint32_t shift = 0;
    auto it = str.crbegin();
    while (it != str.crend())
    {
        fourcc |= uint32_t(*it) << shift;
        shift += 8;
        it++;
    }
    return fourcc;
}

//------------------------------------------------------------------------------
/**
*/
inline uint32_t
StringToFourCC(const GPULang::TransientString& str)
{
    uint32_t fourcc = 0;
    uint32_t shift = 0;
    auto it = str.buf + str.size - 1;
    while (it != str.buf-1)
    {
        fourcc |= uint32_t(*it) << shift;
        shift += 8;
        it--;
    }
    return fourcc;
}



} // namespace GPULang


inline bool
operator<(const GPULang::ConstantString& lhs, const std::string_view& rhs)
{
    return rhs.compare(lhs.buf) > 0;
}

inline bool
operator<(const char lhs, const GPULang::ConstantString& rhs)
{
    std::string_view view(&lhs, 1);
    return view.compare(rhs.buf) < 0;
}

inline bool
operator<(const GPULang::ConstantString& lhs, const char rhs)
{
    std::string_view view(&rhs, 1);
    return view.compare(lhs.buf) > 0;
}


inline bool
operator<(const GPULang::TransientString& lhs, const GPULang::FixedString& rhs)
{
    return strcmp(lhs.buf, rhs.buf) < 0;
}

inline bool
operator<(const GPULang::TransientString& lhs, const GPULang::ConstantString& rhs)
{
    return strcmp(lhs.buf, rhs.buf) < 0;
}

inline bool
operator<(const GPULang::FixedString& lhs, const GPULang::TransientString& rhs)
{
    return strcmp(lhs.buf, rhs.buf) < 0;
}

inline bool
operator<(const GPULang::ConstantString& lhs, const GPULang::FixedString& rhs)
{
    return strcmp(lhs.buf, rhs.buf) < 0;
}

inline bool
operator<(const GPULang::ConstantString& lhs, const GPULang::TransientString& rhs)
{
    return strcmp(lhs.buf, rhs.buf) < 0;
}


inline bool
operator<(const GPULang::FixedString& lhs, const GPULang::ConstantString& rhs)
{
    return strcmp(lhs.buf, rhs.buf) < 0;
}

inline bool
operator<(const std::string_view& lhs, const GPULang::TransientString& rhs)
{
    return lhs.compare(rhs.buf) < 0;
}

inline bool
operator<(const std::string_view& lhs, const GPULang::FixedString& rhs)
{
    return lhs.compare(rhs.buf) < 0;
}

inline bool
operator<(const std::string_view& lhs, const GPULang::ConstantString& rhs)
{
    return lhs.compare(rhs.buf) < 0;
}

template<>
struct std::hash<GPULang::FixedString>
{
    std::size_t operator()(const GPULang::FixedString& str) const noexcept
    {
        return std::hash<std::string_view>{}(std::string_view(str.buf, str.buf + str.len));
    }
};
