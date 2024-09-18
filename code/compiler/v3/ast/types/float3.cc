//------------------------------------------------------------------------------
//  @file float3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float3.h"
namespace GPULang
{

Function Float3::ctor_XYZ;
Function Float3::ctor_f32x2_Z;
Function Float3::ctorSingleValue;
Function Float3::additionOperator;
Function Float3::subtractionOperator;
Function Float3::multiplicationOperator;
Function Float3::divisionOperator;
Function Float3::modOperator;
Function Float3::scaleOperator;
Function Float3::matrix34Mul;
Function Float3::matrix33Mul;
Function Float3::matrix32Mul;
Function Float3::additionAssignOperator;
Function Float3::subtractionAssignOperator;
Function Float3::multiplicationAssignOperator;
Function Float3::divisionAssignOperator;
Function Float3::elementAccessOperatorInt;
Function Float3::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Float3::Float3()
{
    this->baseType = TypeCode::Float;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(ctor_XYZ, f32x3, f32x3);
    __ADD_FUNCTION_PARAM(x, f32);
    __ADD_FUNCTION_PARAM(y, f32);
    __ADD_FUNCTION_PARAM(z, f32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_f32x2_Z, f32x3, f32x3);
    __ADD_FUNCTION_PARAM(xy, f32x2);
    __ADD_FUNCTION_PARAM(z, f32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, f32x3, f32x3, f32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, f32x3, f32);

    __IMPLEMENT_FUNCTION_1(matrix34Mul, operator*, f32x3, f32x3x4);
    __IMPLEMENT_FUNCTION_1(matrix33Mul, operator*, f32x3, f32x3x3);
    __IMPLEMENT_FUNCTION_1(matrix32Mul, operator*, f32x3, f32x3x2);

    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, f32x3, f32x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32, u32);

    char swizzleMask[] = { 'x', 'y', 'z' };
    for (char x = 0; x < 3; x++)
    {
        __ADD_SWIZZLE(f32, "%c", swizzleMask[x]);
        for (char y = 0; y < 3; y++)
        {
            __ADD_SWIZZLE(f32x2, "%c%c", swizzleMask[x], swizzleMask[y]);
            for (char z = 0; z < 3; z++)
            {
                __ADD_SWIZZLE(f32x3, "%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]);
            }
        }
    }
}

} // namespace GPULang
