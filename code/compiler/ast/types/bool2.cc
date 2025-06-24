//------------------------------------------------------------------------------
//  @file bool2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool2.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Bool2::Bool2()
{
    SYMBOL_STATIC_ALLOC = true;
    __BEGIN_TYPE()
    this->baseType = TypeCode::Bool;
    this->rowSize = 1;
    this->columnSize = 2;
    this->byteSize = 2;
    this->category = Type::ScalarCategory;

    // setup constructor for xy
    __IMPLEMENT_CTOR(Bool2_ctor_XY, b8x2, b8x2, Bool2);
    __ADD_FUNCTION_PARAM(x, b8);
    __ADD_FUNCTION_PARAM(y, b8);
    __ADD_CONSTRUCTOR()

        // setup constructor for single component
    __IMPLEMENT_CTOR_1(Bool2_ctorSingleValue, b8x2, b8x2, b8, Bool2);
    
    __IMPLEMENT_FUNCTION_1(Bool2_eOperator, operator==, b8x2, b8x2);
    __IMPLEMENT_FUNCTION_1(Bool2_neOperator, operator!=, b8x2, b8x2);
    __IMPLEMENT_FUNCTION_1(Bool2_ororOperator, operator||, b8x2, b8x2);
    __IMPLEMENT_FUNCTION_1(Bool2_andandOperator, operator&&, b8x2, b8x2);

    __IMPLEMENT_FUNCTION_1(Bool2_elementAccessOperatorInt, operator[], b8, i32);
    __IMPLEMENT_FUNCTION_1(Bool2_elementAccessOperatorUInt, operator[], b8, u32);

    char swizzleMask[] = { 'x', 'y' };
    __IMPLEMENT_SWIZZLE(b8, 2, swizzleMask)
    
    char colorMask[] = { 'r', 'g' };
    __IMPLEMENT_SWIZZLE(b8, 2, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
