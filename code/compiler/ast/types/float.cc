//------------------------------------------------------------------------------
//  @file float.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float.h"
#include "builtins.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Float::Float()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->name = ConstantString("f32");
    this->baseType = TypeCode::Float;
    this->columnSize = 1;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(Float_ctor_UInt, f32, Float, UInt);
    __IMPLEMENT_CTOR_1(Float_ctor_Int, f32, Float, Int);

    __IMPLEMENT_FUNCTION_1(Float_additionOperator, operator+, Float, Float);
    __IMPLEMENT_FUNCTION_1(Float_subtractionOperator, operator-, Float, Float);
    __IMPLEMENT_FUNCTION_1(Float_multiplicationOperator, operator*, Float, Float);
    __IMPLEMENT_FUNCTION_1(Float_divisionOperator, operator/, Float, Float);
    __IMPLEMENT_FUNCTION_1(Float_modOperator, operator%, Float, Float);

    __IMPLEMENT_FUNCTION_1(Float_additionAssignOperator, operator+=, Float, Float);
    __IMPLEMENT_FUNCTION_1(Float_subtractionAssignOperator, operator-=, Float, Float);
    __IMPLEMENT_FUNCTION_1(Float_multiplicationAssignOperator, operator*=, Float, Float);
    __IMPLEMENT_FUNCTION_1(Float_divisionAssignOperator, operator/=, Float, Float);

    __IMPLEMENT_FUNCTION_1(Float_ltOperator, operator<, Bool, Float);
    __IMPLEMENT_FUNCTION_1(Float_lteOperator, operator<=, Bool, Float);
    __IMPLEMENT_FUNCTION_1(Float_gtOperator, operator>, Bool, Float);
    __IMPLEMENT_FUNCTION_1(Float_gteOperator, operator>=, Bool, Float);
    __IMPLEMENT_FUNCTION_1(Float_eOperator, operator==, Bool, Float);
    __IMPLEMENT_FUNCTION_1(Float_neOperator, operator!=, Bool, Float);

    __IMPLEMENT_FUNCTION_1(Float_intAssignOperator, operator=, Float, Int);
    __IMPLEMENT_FUNCTION_1(Float_uintAssignOperator, operator=, Float, UInt);
    __END_TYPE()

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
