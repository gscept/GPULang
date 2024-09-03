//------------------------------------------------------------------------------
//  @file float4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float4.h"
namespace AnyFX
{

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

    __IMPLEMENT_FUNCTION(ctorXYZW, float4, float4);
    __ADD_FUNCTION_PARAM(x, float);
    __ADD_FUNCTION_PARAM(y, float);
    __ADD_FUNCTION_PARAM(z, float);
    __ADD_FUNCTION_PARAM(w, float);

    __IMPLEMENT_FUNCTION_1(ctorSingleValue, float4, float4, float);

    __IMPLEMENT_FUNCTION(ctorFloat3W, float4, float4);
    __ADD_FUNCTION_PARAM(xyz, float3);
    __ADD_FUNCTION_PARAM(w, float);

    __IMPLEMENT_FUNCTION(ctorFloat2ZW, float4, float4);
    __ADD_FUNCTION_PARAM(xy, float2);
    __ADD_FUNCTION_PARAM(z, float);
    __ADD_FUNCTION_PARAM(w, float);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float4, float4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float4, float4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, float4, float4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, float4, float4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float4, float4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float4, float4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, float4, float4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, float4, float4);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], float, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], float, uint);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    for (char x = 0; x < 4; x++)
    {
        __ADD_SWIZZLE(Format("%c", swizzleMask[x]));
        for (char y = 0; y < 4; y++)
        {
            __ADD_SWIZZLE(Format("%c%c", swizzleMask[x], swizzleMask[y]));
            for (char z = 0; z < 4; z++)
            {
                __ADD_SWIZZLE(Format("%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]));
                for (char w = 0; w < 4; w++)
                {
                    __ADD_SWIZZLE(Format("%c%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z], swizzleMask[w]));
                }
            }
        }
    }
}

} // namespace AnyFX
