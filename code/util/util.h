#pragma once
//------------------------------------------------------------------------------
/**
    @file util.h

    Implements Emit, Error and Assert, as well as removes some MSVC warnings.

    2012 Gustav Sterbrant
*/
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <cstdint>
#include <locale.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <string>
#include <type_traits>

#if __WIN32__
#include <intrin.h>
#endif

// remove warning for vsnprintf
#pragma warning( disable : 4996 )

// remove incompatibility with C linkage warning
#pragma warning( disable : 4190 )

// remove loss of data conversion from double to float
#pragma warning( disable : 4244 )

namespace GPULang
{

//------------------------------------------------------------------------------
/**
	Formats const char* to std::string
*/
static std::string
Format(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char buf[65535];
	vsnprintf(buf, sizeof(buf), format, args);
	std::string retVal(buf);
	va_end(args);
	return retVal;
}


//------------------------------------------------------------------------------
/**
	Neat macro to make enums act as bit flags, be able to check if bits are set, and convert to integers
*/
#define ENUM_OPERATORS(TYPE) \
	constexpr TYPE operator|(TYPE a, TYPE b) { return static_cast<TYPE>(static_cast<unsigned>(a) | static_cast<unsigned>(b)); }\
	constexpr TYPE operator&(TYPE a, TYPE b) { return static_cast<TYPE>(static_cast<unsigned>(a) & static_cast<unsigned>(b)); }\
	constexpr TYPE& operator|=(TYPE& a, TYPE b) { a = static_cast<TYPE>(static_cast<unsigned>(a) | static_cast<unsigned>(b)); return a; }\
	constexpr TYPE& operator&=(TYPE& a, TYPE b) { a = static_cast<TYPE>(static_cast<unsigned>(a) & static_cast<unsigned>(b)); return a; }\
	constexpr TYPE operator|(TYPE a, unsigned b) { return static_cast<TYPE>(static_cast<unsigned>(a) | b); }\
	constexpr TYPE operator&(TYPE a, unsigned b) { return static_cast<TYPE>(static_cast<unsigned>(a) & b); }\
	constexpr TYPE& operator|=(TYPE& a, unsigned b) { a = static_cast<TYPE>(static_cast<unsigned>(a) | b); return a; }\
	constexpr TYPE& operator&=(TYPE& a, unsigned b) { a = static_cast<TYPE>(static_cast<unsigned>(a) & b); return a; }\
	constexpr unsigned operator|(unsigned a, TYPE b) { return a | static_cast<unsigned>(b); }\
	constexpr unsigned operator&(unsigned a, TYPE b) { return a & static_cast<unsigned>(b); }\
	constexpr unsigned& operator|=(unsigned& a, TYPE b) { a = a | static_cast<unsigned>(b); return a; }\
	constexpr unsigned& operator&=(unsigned& a, TYPE b) { a = a & static_cast<unsigned>(b); return a; }\
	constexpr bool operator>(TYPE a, unsigned b) { return static_cast<unsigned>(a) > b; }\
	constexpr bool operator>(unsigned a, TYPE b) { return a > static_cast<unsigned>(b); }\
	constexpr bool operator<(TYPE a, unsigned b) { return static_cast<unsigned>(a) < b; }\
	constexpr bool operator<(unsigned a, TYPE b) { return a < static_cast<unsigned>(b); }\
	constexpr bool HasFlags(const TYPE& a, TYPE flags) { return (a & flags) == flags; }\
	constexpr typename std::underlying_type<TYPE>::type EnumToInt(TYPE a) { return static_cast<typename std::underlying_type<TYPE>::type>(a); }\
    constexpr TYPE IntToEnum(typename std::underlying_type<TYPE>::type a) { return static_cast<TYPE>(a); }

//------------------------------------------------------------------------------
/**
*/
template <typename T>
inline constexpr T max(T a, T b)
{
    return a > b ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename T2>
inline constexpr T max(T a, T2 b)
{
    return a > b ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
inline uint8_t
CountLeadingZeroes(uint64_t word)
{
    if (word == 0)
        return 255;
    else
    {
#if __WIN32__
        unsigned long index;
        _BitScanForward64(&index, word);
        return index;
#else
        return __builtin_ctzll(word);
#endif
    }
}

//------------------------------------------------------------------------------
/**
*/
template <typename T>
inline constexpr T min(T a, T b)
{
    return a < b ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename T2>
inline constexpr T min(T a, T2 b)
{
    return a < b ? a : b;
}

} // namespace GPULang
