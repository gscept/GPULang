//------------------------------------------------------------------------------
//  @file bool4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool4.h"
#include "builtins.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Bool4::Bool4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->name = ConstantString("b8x4");
    this->baseType = TypeCode::Bool;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    // setup constructor for xyzw
    __IMPLEMENT_CTOR(Bool4_ctor_XYZW, b8x4, Bool4);
    __ADD_FUNCTION_PARAM(x, Bool);
    __ADD_FUNCTION_PARAM(y, Bool);
    __ADD_FUNCTION_PARAM(z, Bool);
    __ADD_FUNCTION_PARAM(w, Bool);
    __ADD_CONSTRUCTOR();

    // setup constructor for single component
    __IMPLEMENT_CTOR_1(Bool4_ctorSingleValue, b8x4, Bool4, Bool);
    
    __IMPLEMENT_FUNCTION_1(Bool4_eOperator, operator==, Bool4, Bool4);
    __IMPLEMENT_FUNCTION_1(Bool4_neOperator, operator!=, Bool4, Bool4);
    __IMPLEMENT_FUNCTION_1(Bool4_ororOperator, operator||, Bool4, Bool4);
    __IMPLEMENT_FUNCTION_1(Bool4_andandOperator, operator&&, Bool4, Bool4);

    __IMPLEMENT_FUNCTION_1(Bool4_elementAccessOperatorInt, operator[], Bool, Int);
    __IMPLEMENT_FUNCTION_1(Bool4_elementAccessOperatorUInt, operator[], Bool, UInt);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(Bool, 4, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(Bool, 4, colorMask)
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
