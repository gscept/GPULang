//------------------------------------------------------------------------------
//  @file float.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Float::Float()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->columnSize = 1;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(Float_ctor_UInt, f32, f32, u32, Float);
    __IMPLEMENT_CTOR_1(Float_ctor_Int, f32, f32, i32, Float);

    __IMPLEMENT_FUNCTION_1(Float_additionOperator, operator+, f32, f32);
    __IMPLEMENT_FUNCTION_1(Float_subtractionOperator, operator-, f32, f32);
    __IMPLEMENT_FUNCTION_1(Float_multiplicationOperator, operator*, f32, f32);
    __IMPLEMENT_FUNCTION_1(Float_divisionOperator, operator/, f32, f32);
    __IMPLEMENT_FUNCTION_1(Float_modOperator, operator%, f32, f32);

    __IMPLEMENT_FUNCTION_1(Float_additionAssignOperator, operator+=, f32, f32);
    __IMPLEMENT_FUNCTION_1(Float_subtractionAssignOperator, operator-=, f32, f32);
    __IMPLEMENT_FUNCTION_1(Float_multiplicationAssignOperator, operator*=, f32, f32);
    __IMPLEMENT_FUNCTION_1(Float_divisionAssignOperator, operator/=, f32, f32);

    __IMPLEMENT_FUNCTION_1(Float_ltOperator, operator<, b8, f32);
    __IMPLEMENT_FUNCTION_1(Float_lteOperator, operator<=, b8, f32);
    __IMPLEMENT_FUNCTION_1(Float_gtOperator, operator>, b8, f32);
    __IMPLEMENT_FUNCTION_1(Float_gteOperator, operator>=, b8, f32);
    __IMPLEMENT_FUNCTION_1(Float_eOperator, operator==, b8, f32);
    __IMPLEMENT_FUNCTION_1(Float_neOperator, operator!=, b8, f32);

    __IMPLEMENT_FUNCTION_1(Float_intAssignOperator, operator=, f32, i32);
    __IMPLEMENT_FUNCTION_1(Float_uintAssignOperator, operator=, f32, u32);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
