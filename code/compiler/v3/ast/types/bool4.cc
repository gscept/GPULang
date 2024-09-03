//------------------------------------------------------------------------------
//  @file bool4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool4.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Bool4::Bool4()
{
    this->baseType = TypeCode::Bool;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    // setup constructor for xyzw
    __IMPLEMENT_FUNCTION(ctorXYZW, bool4, bool4);
    __ADD_FUNCTION_PARAM(x, bool);
    __ADD_FUNCTION_PARAM(y, bool);
    __ADD_FUNCTION_PARAM(z, bool);
    __ADD_FUNCTION_PARAM(w, bool);

    // setup constructor for single component
    __IMPLEMENT_FUNCTION_1(ctorSingleValue, bool4, bool4, bool);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], bool, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], bool, uint);

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

} // namespace GPULang
