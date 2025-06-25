//------------------------------------------------------------------------------
//  @file bool3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool3.h"
#include "builtins.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Bool3::Bool3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->name = ConstantString("b8x3");
    this->baseType = TypeCode::Bool;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 3;
    this->category = Type::ScalarCategory;

    // setup constructor for xyz
    __IMPLEMENT_CTOR(Bool3_ctor_XYZ, b8x3, Bool3);
    __ADD_FUNCTION_PARAM(x, Bool);
    __ADD_FUNCTION_PARAM(y, Bool);
    __ADD_FUNCTION_PARAM(z, Bool);
    __ADD_CONSTRUCTOR()

    // setup constructor for single component
    __IMPLEMENT_CTOR_1(Bool3_ctorSingleValue, b8x3, Bool3, Bool);
    
    __IMPLEMENT_FUNCTION_1(Bool3_eOperator, operator==, Bool3, Bool3);
    __IMPLEMENT_FUNCTION_1(Bool3_neOperator, operator!=, Bool3, Bool3);
    __IMPLEMENT_FUNCTION_1(Bool3_ororOperator, operator||, Bool3, Bool3);
    __IMPLEMENT_FUNCTION_1(Bool3_andandOperator, operator&&, Bool3, Bool3);

    __IMPLEMENT_FUNCTION_1(Bool3_elementAccessOperatorInt, operator[], Bool, Int);
    __IMPLEMENT_FUNCTION_1(Bool3_elementAccessOperatorUInt, operator[], Bool, UInt);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(Bool, 3, swizzleMask)
    
    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(Bool, 3, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
