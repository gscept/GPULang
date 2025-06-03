//------------------------------------------------------------------------------
//  @file bool4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool4.h"
namespace GPULang
{

Function Bool4::ctor_XYZW;
Function Bool4::ctorSingleValue;

Function Bool4::ororOperator;
Function Bool4::andandOperator;
Function Bool4::eOperator;
Function Bool4::neOperator;

Function Bool4::elementAccessOperatorInt;
Function Bool4::elementAccessOperatorUInt;
//------------------------------------------------------------------------------
/**
*/
Bool4::Bool4()
{
    __BEGIN_TYPE()
    this->baseType = TypeCode::Bool;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    // setup constructor for xyzw
    __IMPLEMENT_CTOR(ctor_XYZW, b8x4, b8x4);
    __ADD_FUNCTION_PARAM(x, b8);
    __ADD_FUNCTION_PARAM(y, b8);
    __ADD_FUNCTION_PARAM(z, b8);
    __ADD_FUNCTION_PARAM(w, b8);
    __ADD_CONSTRUCTOR()

    // setup constructor for single component
    __IMPLEMENT_CTOR_1(ctorSingleValue, b8x4, b8x4, b8);
    __ADD_CONSTRUCTOR()
    
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8x4, b8x4);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8x4, b8x4);
    __IMPLEMENT_FUNCTION_1(ororOperator, operator||, b8x4, b8x4);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator&&, b8x4, b8x4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], b8, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], b8, u32);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(b8, 4, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(b8, 4, colorMask)
}

} // namespace GPULang
