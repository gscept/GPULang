//------------------------------------------------------------------------------
//  @file bool2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool2.h"
namespace GPULang
{

Function Bool2::ctor_XY;
Function Bool2::ctorSingleValue;
Function Bool2::elementAccessOperatorInt;
Function Bool2::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Bool2::Bool2()
{
    this->baseType = TypeCode::Bool;
    this->rowSize = 1;
    this->columnSize = 2;
    this->byteSize = 2;
    this->category = Type::ScalarCategory;

    // setup constructor for xy
    __IMPLEMENT_CTOR(ctor_XY, b8x2, b8x2);
    __ADD_FUNCTION_PARAM(x, b8);
    __ADD_FUNCTION_PARAM(y, b8);
    __ADD_CONSTRUCTOR()

        // setup constructor for single component
    __IMPLEMENT_CTOR_1(ctorSingleValue, b8x2, b8x2, b8);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], b8, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], b8, u32);

    char swizzleMask[] = { 'x', 'y' };
    __IMPLEMENT_SWIZZLE(b8, 2, swizzleMask) 
}

} // namespace GPULang