//------------------------------------------------------------------------------
//  @file bool2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool2.h"
#include "builtins.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Bool2::Bool2()
{
    SYMBOL_STATIC_ALLOC = true;
    __BEGIN_TYPE()
    this->name = ConstantString("b8x2");
    this->baseType = TypeCode::Bool;
    this->rowSize = 1;
    this->columnSize = 2;
    this->byteSize = 2;
    this->category = Type::ScalarCategory;

    // setup constructor for xy
    __IMPLEMENT_CTOR(Bool2_ctor_XY, b8x2, Bool2);
    __ADD_FUNCTION_PARAM(x, Bool);
    __ADD_FUNCTION_PARAM(y, Bool);
    __ADD_CONSTRUCTOR()

        // setup constructor for single component
    __IMPLEMENT_CTOR_1(Bool2_ctorSingleValue, b8x2, Bool2, Bool);
    
    __IMPLEMENT_FUNCTION_1(Bool2_eOperator, operator==, Bool2, Bool2);
    __IMPLEMENT_FUNCTION_1(Bool2_neOperator, operator!=, Bool2, Bool2);
    __IMPLEMENT_FUNCTION_1(Bool2_ororOperator, operator||, Bool2, Bool2);
    __IMPLEMENT_FUNCTION_1(Bool2_andandOperator, operator&&, Bool2, Bool2);

    __IMPLEMENT_FUNCTION_1(Bool2_elementAccessOperatorInt, operator[], Bool, Int);
    __IMPLEMENT_FUNCTION_1(Bool2_elementAccessOperatorUInt, operator[], Bool, UInt);

    char swizzleMask[] = { 'x', 'y' };
    __IMPLEMENT_SWIZZLE(Bool, 2, swizzleMask)
    
    char colorMask[] = { 'r', 'g' };
    __IMPLEMENT_SWIZZLE(Bool, 2, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
