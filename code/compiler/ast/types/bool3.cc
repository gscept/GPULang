//------------------------------------------------------------------------------
//  @file bool3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool3.h"
namespace GPULang
{

Function Bool3::ctor_XYZ;
Function Bool3::ctorSingleValue;

Function Bool3::ororOperator;
Function Bool3::andandOperator;
Function Bool3::eOperator;
Function Bool3::neOperator;

Function Bool3::elementAccessOperatorInt;
Function Bool3::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Bool3::Bool3()
{
    __BEGIN_TYPE()
    this->baseType = TypeCode::Bool;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 3;
    this->category = Type::ScalarCategory;

    // setup constructor for xyz
    __IMPLEMENT_CTOR(ctor_XYZ, b8x3, b8x3);
    __ADD_FUNCTION_PARAM(x, b8);
    __ADD_FUNCTION_PARAM(y, b8);
    __ADD_FUNCTION_PARAM(z, b8);
    __ADD_CONSTRUCTOR()

    // setup constructor for single component
    __IMPLEMENT_CTOR_1(ctorSingleValue, b8x3, b8x3, b8);
    
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8x3, b8x3);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8x3, b8x3);
    __IMPLEMENT_FUNCTION_1(ororOperator, operator||, b8x3, b8x3);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator&&, b8x3, b8x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], b8, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], b8, u32);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(b8, 3, swizzleMask)
    
    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(b8, 3, colorMask)
}

} // namespace GPULang
