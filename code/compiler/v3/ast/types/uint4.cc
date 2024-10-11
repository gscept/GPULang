//------------------------------------------------------------------------------
//  @file uint4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint4.h"
namespace GPULang
{

Function UInt4::ctor;


#define X(type, ctor, val, args, splat, size, conversion)\
    Function type::ctor;

    UINT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    Function type::ctor;

    UINT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    Function type::ctor;

    UINT4_CTOR3_LIST
#undef X


Function UInt4::andOperator;
Function UInt4::orOperator;
Function UInt4::xorOperator;
Function UInt4::leftShiftOperator;
Function UInt4::rightShiftOperator;
Function UInt4::additionOperator;
Function UInt4::subtractionOperator;
Function UInt4::multiplicationOperator;
Function UInt4::divisionOperator;
Function UInt4::modOperator;
Function UInt4::scaleOperator;

Function UInt4::andAssignOperator;
Function UInt4::orAssignOperator;
Function UInt4::xorAssignOperator;
Function UInt4::leftShiftAssignOperator;
Function UInt4::rightShiftAssignOperator;
Function UInt4::additionAssignOperator;
Function UInt4::subtractionAssignOperator;
Function UInt4::multiplicationAssignOperator;
Function UInt4::divisionAssignOperator;
Function UInt4::moduloAssignOperator;

Function UInt4::ltOperator;
Function UInt4::lteOperator;
Function UInt4::gtOperator;
Function UInt4::gteOperator;
Function UInt4::eOperator;
Function UInt4::neOperator;

Function UInt4::elementAccessOperatorInt;
Function UInt4::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
UInt4::UInt4()
{
    this->baseType = TypeCode::UInt;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(ctor, u32x4, u32x4);
    __ADD_FUNCTION_PARAM(x, u32);
    __ADD_FUNCTION_PARAM(y, u32);
    __ADD_FUNCTION_PARAM(z, u32);
    __ADD_FUNCTION_PARAM(w, u32);

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(ctor, u32x4, u32x4, val);

    UINT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(ctor, u32x4, u32x4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)

    UINT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    __IMPLEMENT_CTOR(ctor, u32x4, u32x4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_FUNCTION_PARAM(arg_2, arg2)

    UINT4_CTOR3_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, u32x4, u32);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, u32x4, u32x4);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, b8x4, u32x4);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, b8x4, u32x4);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, b8x4, u32x4);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, b8x4, u32x4);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8x4, u32x4);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8x4, u32x4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], u32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], u32, u32);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    for (char x = 0; x < 4; x++)
    {
        __ADD_SWIZZLE(u32, "%c", swizzleMask[x]);
        for (char y = 0; y < 4; y++)
        {
            __ADD_SWIZZLE(u32x2, "%c%c", swizzleMask[x], swizzleMask[y]);
            for (char z = 0; z < 4; z++)
            {
                __ADD_SWIZZLE(u32x3, "%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]);
                for (char w = 0; w < 4; w++)
                {
                    __ADD_SWIZZLE(u32x4, "%c%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z], swizzleMask[w]);
                }
            }
        }
    }
}

} // namespace GPULang
