#pragma once
//------------------------------------------------------------------------------
/**
    types.h
    
    Contains data types used to maintain initializers and data types
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "util.h"
namespace GPULang
{

template<typename T, int S>
struct vector
{
	T v[S];
};

template<typename T, int R, int C>
struct matrix
{
	T m[R][C];
};

}
