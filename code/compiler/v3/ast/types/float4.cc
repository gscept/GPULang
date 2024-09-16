//------------------------------------------------------------------------------
//  @file float4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float4.h"
namespace GPULang
{

Function Float4::ctor_XYZW;
Function Float4::ctorSingleValue;
Function Float4::ctor_Float3_W;
Function Float4::ctor_Float2_ZW;
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

    __IMPLEMENT_GLOBAL(ctor_XYZW, float4, float4);
    __ADD_FUNCTION_PARAM(x, float);
    __ADD_FUNCTION_PARAM(y, float);
    __ADD_FUNCTION_PARAM(z, float);
    __ADD_FUNCTION_PARAM(w, float);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, float4, float4, float);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_Float3_W, float4, float4);
    __ADD_FUNCTION_PARAM(xyz, float3);
    __ADD_FUNCTION_PARAM(w, float);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_Float2_ZW, float4, float4);
    __ADD_FUNCTION_PARAM(xy, float2);
    __ADD_FUNCTION_PARAM(z, float);
    __ADD_FUNCTION_PARAM(w, float);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float4, float4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float4, float4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, float4, float4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, float4, float4);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, float4, float4);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, float4, float);

    __IMPLEMENT_FUNCTION_1(matrix44Mul, operator*, float4, float4x4);
    __IMPLEMENT_FUNCTION_1(matrix43Mul, operator*, float4, float4x3);
    __IMPLEMENT_FUNCTION_1(matrix42Mul, operator*, float4, float4x2);

    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float4, float4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float4, float4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, float4, float4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, float4, float4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], float, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], float, uint);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    for (char x = 0; x < 4; x++)
    {
        __ADD_SWIZZLE(float, "%c", swizzleMask[x]);
        for (char y = 0; y < 4; y++)
        {
            __ADD_SWIZZLE(float2, "%c%c", swizzleMask[x], swizzleMask[y]);
            for (char z = 0; z < 4; z++)
            {
                __ADD_SWIZZLE(float3, "%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]);
                for (char w = 0; w < 4; w++)
                {
                    __ADD_SWIZZLE(float4, "%c%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z], swizzleMask[w]);
                }
            }
        }
    }
}

} // namespace GPULang
