//------------------------------------------------------------------------------
//  @file bool3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool3.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Bool3::Bool3()
{
    this->baseType = TypeCode::Bool;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 3;
    this->category = Type::ScalarCategory;

    // setup constructor for xyz
    __IMPLEMENT_FUNCTION(ctorXYZ, bool3, bool3);
    __ADD_FUNCTION_PARAM(x, bool);
    __ADD_FUNCTION_PARAM(y, bool);
    __ADD_FUNCTION_PARAM(z, bool);

    // setup constructor for single component
    __IMPLEMENT_FUNCTION_1(ctorSingleValue, bool3, bool3, bool);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], bool, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], bool, uint);

    char swizzleMask[] = { 'x', 'y', 'z' };

    for (char x = 0; x < 3; x++)
    {
        __ADD_SWIZZLE(Format("%c", swizzleMask[x]));
        for (char y = 0; y < 3; y++)
        {
            __ADD_SWIZZLE(Format("%c%c", swizzleMask[x], swizzleMask[y]));
            for (char z = 0; z < 3; z++)
            {
                __ADD_SWIZZLE(Format("%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]));
            }
        }
    }
}

} // namespace GPULang
