//------------------------------------------------------------------------------
//  @file bool2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool2.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Bool2::Bool2()
{
    this->baseType = TypeCode::Bool;
    this->rowSize = 1;
    this->columnSize = 2;
    this->byteSize = 2;
    this->category = Type::ScalarCategory;

    // setup constructor for xy
    __IMPLEMENT_GLOBAL(ctor_XY, bool2, bool2);
    __ADD_FUNCTION_PARAM(x, bool);
    __ADD_FUNCTION_PARAM(y, bool);
    __ADD_CONSTRUCTOR()

        // setup constructor for single component
    __IMPLEMENT_GLOBAL_1(ctorSingleValue, bool2, bool2, bool);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], bool, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], bool, uint);

    char swizzleMask[] = { 'x', 'y' };
    for (char x = 0; x < 2; x++)
    {
        __ADD_SWIZZLE(bool, "%c", swizzleMask[x]);
        for (char y = 0; y < 2; y++)
        {
            __ADD_SWIZZLE(bool2, "%c%c", swizzleMask[x], swizzleMask[y]);
        }
    }   
}

} // namespace GPULang
