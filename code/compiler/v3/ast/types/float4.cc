//------------------------------------------------------------------------------
//  @file float4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float4.h"
namespace GPULang
{

Function Float4::ctor_XYZW;
Function Float4::ctorSingleValue;
Function Float4::ctor_f32x3_W;
Function Float4::ctor_f32x2_ZW;
Function Float4::additionOperator;
Function Float4::subtractionOperator;
Function Float4::multiplicationOperator;
Function Float4::divisionOperator;
Function Float4::modOperator;
Function Float4::scaleOperator;
Function Float4::matrix44Mul;
Function Float4::matrix43Mul;
Function Float4::matrix42Mul;
Function Float4::additionAssignOperator;
Function Float4::subtractionAssignOperator;
Function Float4::multiplicationAssignOperator;
Function Float4::divisionAssignOperator;
Function Float4::elementAccessOperatorInt;
Function Float4::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Float4::Float4()
{
    this->baseType = TypeCode::Float;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(ctor_XYZW, f32x4, f32x4);
    __ADD_FUNCTION_PARAM(x, f32);
    __ADD_FUNCTION_PARAM(y, f32);
    __ADD_FUNCTION_PARAM(z, f32);
    __ADD_FUNCTION_PARAM(w, f32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, f32x4, f32x4, f32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_f32x3_W, f32x4, f32x4);
    __ADD_FUNCTION_PARAM(xyz, f32x3);
    __ADD_FUNCTION_PARAM(w, f32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_f32x2_ZW, f32x4, f32x4);
    __ADD_FUNCTION_PARAM(xy, f32x2);
    __ADD_FUNCTION_PARAM(z, f32);
    __ADD_FUNCTION_PARAM(w, f32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, f32x4, f32);

    __IMPLEMENT_FUNCTION_1(matrix44Mul, operator*, f32x4, f32x4x4);
    __IMPLEMENT_FUNCTION_1(matrix43Mul, operator*, f32x4, f32x4x3);
    __IMPLEMENT_FUNCTION_1(matrix42Mul, operator*, f32x4, f32x4x2);

    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, f32x4, f32x4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32, u32);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    for (char x = 0; x < 4; x++)
    {
        __ADD_SWIZZLE(f32, "%c", swizzleMask[x]);
        for (char y = 0; y < 4; y++)
        {
            __ADD_SWIZZLE(f32x2, "%c%c", swizzleMask[x], swizzleMask[y]);
            for (char z = 0; z < 4; z++)
            {
                __ADD_SWIZZLE(f32x3, "%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]);
                for (char w = 0; w < 4; w++)
                {
                    __ADD_SWIZZLE(f32x4, "%c%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z], swizzleMask[w]);
                }
            }
        }
    }
}

} // namespace GPULang
