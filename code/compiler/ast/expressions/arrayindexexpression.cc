//------------------------------------------------------------------------------
//  @file arrayindexexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "arrayindexexpression.h"
#include "util.h"
#include "compiler.h"
#include "ast/function.h"
#include "ast/types/type.h"
namespace GPULang 
{

//------------------------------------------------------------------------------
/**
*/
ArrayIndexExpression::ArrayIndexExpression(Expression* left, Expression* right)
    : left(left)
    , right(right)
{
    this->resolved = Alloc<ArrayIndexExpression::__Resolved>();
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->lhsType = nullptr;
    thisResolved->rhsType = nullptr;
    this->symbolType = ArrayIndexExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
ArrayIndexExpression::~ArrayIndexExpression()
{
    this->left->~Expression();
    this->right->~Expression();
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayIndexExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    if (this->isLhsValue)
        this->left->isLhsValue = true;
    
    if (this->left == nullptr || this->right == nullptr)
        return false;

    // If rhs value, array indexing needs to provide an argument 
    if (this->right == nullptr)
    {
        compiler->Error(Format("Array access needs index but got '[]'"), this);
        return false;
    }

    if (!this->left->Resolve(compiler))
        return false;
    if (this->right != nullptr && !this->right->Resolve(compiler))
        return false;
    this->left->EvalType(thisResolved->leftFullType);
    if (this->right != nullptr)
        this->right->EvalType(thisResolved->rightFullType);

    thisResolved->returnFullType = thisResolved->leftFullType;

    // If there are no modifiers, then we check for an array access operator for the type
    if (thisResolved->returnFullType.modifiers.empty())
    {
        Type* type = static_cast<Type*>(compiler->GetType(thisResolved->returnFullType));
        TStr lookup = TStr("operator[](", thisResolved->rightFullType.name, ")");
        auto it = type->scope.symbolLookup.Find(lookup);
        if (it == type->scope.symbolLookup.end())
        {
            compiler->Error(Format("'%s' does not implement the [] operator", thisResolved->leftFullType.ToString().c_str()), this);
            return false;
        }
            
        // If access operator was found, set the return type of this expression as the result of it
        Function* accessFunc = static_cast<Function*>(it->second);
        thisResolved->returnFullType = accessFunc->returnType;
        //thisResolved->returnType = compiler->GetType(thisResolved->returnFullType.name);
        /*
        if (!this->right->EvalUInt(thisResolved->literalAccess))
        {
            compiler->Error(Format("'%s' only allows indexing with a literal or compile time deducable value", thisResolved->leftFullType.ToString().c_str()), this);
            return false;
        }
        */
        //return true;
    }
    else
    {
        if (thisResolved->returnFullType.modifiers.front() != Type::FullType::Modifier::Array)
        {
            compiler->Error(Format("Invalid array access operator '[]' on non-array type"), this);
            return false;
        }
        thisResolved->returnFullType.modifiers.erase(thisResolved->returnFullType.modifiers.begin());
        thisResolved->returnFullType.modifierValues.erase(thisResolved->returnFullType.modifierValues.begin());
    }
    
    thisResolved->returnType = compiler->GetType(thisResolved->returnFullType);

    if (!thisResolved->leftFullType.modifierValues.empty())
    {
        ValueUnion val;
        if (this->right->EvalValue(val) && thisResolved->leftFullType.modifierValues[0] != nullptr)
        {
            ValueUnion val2;
            if (thisResolved->leftFullType.modifierValues[0]->EvalValue(val2))
            {
                if (val.ui[0] > val2.ui[0])
                {
                    compiler->Error(Format("Index '%d' is outside of array bounds (%d)", val.ui[0], val2.ui[0]), this);
                    return false;
                }
            }
        }
    }
    // Return type is the left type with one less modifier
    this->left->EvalTypeSymbol(thisResolved->lhsType);
    if (thisResolved->lhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(thisResolved->leftFullType.name, this);
        return false;
    }

    // If we have a right expression (array index), get the type and validate
    if (this->right != nullptr)
    {
        this->right->EvalTypeSymbol(thisResolved->rhsType);
        if (thisResolved->rhsType == nullptr)
        {
            compiler->UnrecognizedTypeError(thisResolved->rightFullType.ToString(), this);
            return false;
        }

        if (thisResolved->rhsType->name != "u32" && thisResolved->rhsType->name != "i32")
        {
            compiler->Error(Format("Expected array index to be u32 or i32 but got '%s'", thisResolved->rightFullType.ToString().c_str()), this->right);
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayIndexExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->returnFullType.name == UNDEFINED_TYPE)
        return false;
    out = thisResolved->returnFullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
ArrayIndexExpression::EvalTypeSymbol(Type*& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->returnType == nullptr)
        return false;
    out = thisResolved->returnType;
    assert(out->symbolType == Symbol::SymbolType::TypeType || out->symbolType == Symbol::SymbolType::EnumerationType || out->symbolType == Symbol::SymbolType::StructureType);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
ArrayIndexExpression::EvalSymbol(FixedString& out) const
{
    return this->left->EvalSymbol(out);
}

//------------------------------------------------------------------------------
/**
*/
bool
ArrayIndexExpression::EvalValue(ValueUnion& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->lhsType == nullptr || thisResolved->rhsType == nullptr)
        return false;
    return this->left->EvalValue(out);
}

//------------------------------------------------------------------------------
/**
*/
TransientString
ArrayIndexExpression::EvalString() const
{
    TransientString left, right;
    left = this->left->EvalString();
    right = this->right->EvalString();

    if (this->right != nullptr)
        return TransientString(left, "[", right, "]");
    else
        return TransientString(left, "[]");
}

//------------------------------------------------------------------------------
/**
*/
bool 
ArrayIndexExpression::EvalAccessFlags(unsigned& out) const
{
    return this->left->EvalAccessFlags(out);
}

//------------------------------------------------------------------------------
/**
*/
bool
ArrayIndexExpression::EvalStorage(Storage& out) const
{
    return this->left->EvalStorage(out);
}
} // namespace GPULang
