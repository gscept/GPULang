//------------------------------------------------------------------------------
//  @file builtins.cc
//  @copyright (C) 2025 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "builtins.h"
#include "memory.h"
#include "ast/expressions/uintexpression.h"

#define __ADD_ENUM(val) labels.Append(#val); values.Append(nullptr);
#define __FINISH_ENUM(enum) enum->labels = StaticArray<FixedString>(labels); enum->values = StaticArray<Expression*>(values); labels.size = 0; values.size = 0;
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
MemorySemanticsType::MemorySemanticsType()
{
    this->name = ConstantString("MemorySemantics");
    this->category = EnumCategory;

    TransientArray<ConstantString> labels(32);
    TransientArray<Expression*> values(32);

    this->name = ConstantString("MemorySemantics");
    this->type = Type::FullType{ ConstantString("u32") };
    this->type.literal = true;
    Symbol::Resolved(this)->typeSymbol = &UIntType;
    this->baseType = GPULang::TypeCode::UInt;
    labels.Append("Relaxed"); values.Append(StaticAlloc<UIntExpression>(0x0));
    labels.Append("Acquire"); values.Append(StaticAlloc<UIntExpression>(0x1));
    labels.Append("Release"); values.Append(StaticAlloc<UIntExpression>(0x2));
    labels.Append("AcquireRelease"); values.Append(StaticAlloc<UIntExpression>(0x4));
    __FINISH_ENUM(this);
    this->builtin = true;
}

//------------------------------------------------------------------------------
/**
*/
ExecutionScopeType::ExecutionScopeType()
{
    this->name = ConstantString("ExecutionScope");
    this->category = EnumCategory;

    TransientArray<ConstantString> labels(32);
    TransientArray<Expression*> values(32);

    this->name = ConstantString("ExecutionScope");
    this->type = Type::FullType{ ConstantString("u32") };
    Symbol::Resolved(this)->typeSymbol = &UIntType;
    this->type.literal = true;
    this->baseType = GPULang::TypeCode::UInt;
    __ADD_ENUM(Global)
    __ADD_ENUM(Device)
    __ADD_ENUM(Workgroup)
    __ADD_ENUM(Subgroup)
    __ADD_ENUM(Invocation)
    __ADD_ENUM(Queue)
    __FINISH_ENUM(this);
    this->builtin = true;
}

//------------------------------------------------------------------------------
/**
*/
StencilOpType::StencilOpType()
{
    this->name = ConstantString("StencilOp");
    this->category = EnumCategory;

    TransientArray<ConstantString> labels(32);
    TransientArray<Expression*> values(32);

    this->name = ConstantString("StencilOp");
    this->type = Type::FullType{ ConstantString("u32") };
    Symbol::Resolved(this)->typeSymbol = &UIntType;
    this->type.literal = true;
    this->baseType = GPULang::TypeCode::UInt;
    __ADD_ENUM(Invalid);
    __ADD_ENUM(Keep);
    __ADD_ENUM(Zero);
    __ADD_ENUM(Replace);
    __ADD_ENUM(IncrementClamp);
    __ADD_ENUM(DecrementClamp);
    __ADD_ENUM(Invert);
    __ADD_ENUM(IncrementWrap);
    __ADD_ENUM(DecrementWrap);
    __FINISH_ENUM(this);
    this->builtin = true;
}

//------------------------------------------------------------------------------
/**
*/
CompareModeType::CompareModeType()
{
    this->name = ConstantString("CompareMode");
    this->category = EnumCategory;
    this->type = Type::FullType{ ConstantString("u32") };
    Symbol::Resolved(this)->typeSymbol = &UIntType;
    this->type.literal = true;
    this->baseType = GPULang::TypeCode::UInt;
    this->builtin = true;
    TransientArray<ConstantString> labels(32);
    TransientArray<Expression*> values(32);

    __ADD_ENUM(InvalidCompareMode);
    __ADD_ENUM(Never);
    __ADD_ENUM(Less);
    __ADD_ENUM(Equal);
    __ADD_ENUM(LessEqual);
    __ADD_ENUM(Greater);
    __ADD_ENUM(NotEqual);
    __ADD_ENUM(GreaterEqual);
    __ADD_ENUM(Always);
    __FINISH_ENUM(this);
}

} // namespace GPULang