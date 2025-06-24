//------------------------------------------------------------------------------
//  @file bool4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool4.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Bool4::Bool4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Bool;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    // setup constructor for xyzw
    __IMPLEMENT_CTOR(Bool4_ctor_XYZW, b8x4, b8x4, Bool4);
    __ADD_FUNCTION_PARAM(x, b8);
    __ADD_FUNCTION_PARAM(y, b8);
    __ADD_FUNCTION_PARAM(z, b8);
    __ADD_FUNCTION_PARAM(w, b8);
    __ADD_CONSTRUCTOR();

    // setup constructor for single component
    __IMPLEMENT_CTOR_1(Bool4_ctorSingleValue, b8x4, b8x4, b8, Bool4);
    
    __IMPLEMENT_FUNCTION_1(Bool4_eOperator, operator==, b8x4, b8x4);
    __IMPLEMENT_FUNCTION_1(Bool4_neOperator, operator!=, b8x4, b8x4);
    __IMPLEMENT_FUNCTION_1(Bool4_ororOperator, operator||, b8x4, b8x4);
    __IMPLEMENT_FUNCTION_1(Bool4_andandOperator, operator&&, b8x4, b8x4);

    __IMPLEMENT_FUNCTION_1(Bool4_elementAccessOperatorInt, operator[], b8, i32);
    __IMPLEMENT_FUNCTION_1(Bool4_elementAccessOperatorUInt, operator[], b8, u32);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(b8, 4, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(b8, 4, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
