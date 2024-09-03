//------------------------------------------------------------------------------
//  @file llvmgenerator.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "llvmgenerator.h"
#include "util.h"
#include "ast/function.h"
#include "ast/symbol.h"
#include "ast/structure.h"
#include "ast/program.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/arrayindexexpression.h"
#include "ast/expressions/callexpression.h"
#include "ast/expressions/initializerexpression.h"
#include "ast/expressions/accessexpression.h"
#include "ast/expressions/binaryexpression.h"
#include "ast/expressions/unaryexpression.h"
#include "ast/expressions/boolexpression.h"
#include "ast/expressions/intexpression.h"
#include "ast/expressions/uintexpression.h"
#include "ast/expressions/floatexpression.h"
#include "ast/expressions/commaexpression.h"
#include "ast/expressions/ternaryexpression.h"
#include "ast/expressions/symbolexpression.h"
#include "ast/statements/statement.h"
#include "ast/statements/scopestatement.h"
#include "ast/statements/expressionstatement.h"
#include "ast/statements/returnstatement.h"
#include "ast/statements/ifstatement.h"
#include "ast/statements/forstatement.h"
#include "ast/statements/whilestatement.h"
#include "ast/statements/continuestatement.h"
#include "ast/statements/breakstatement.h"
#include "ast/statements/switchstatement.h"
#include "ast/types/type.h"
#include "compiler.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IntrinsicsGPULang.h"
#include "llvm/IR/Metadata.h"
#include "llvm/Support/raw_os_ostream.h"
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
LLVMGenerator::LLVMGenerator(Backend backend) :
    irBuilder(this->llvmCtx)
    , scope(nullptr)
    , function(nullptr)
{
    // Push main scope
    this->PushScope();
}

//------------------------------------------------------------------------------
/**
*/
void 
LLVMGenerator::PushScope()
{
    this->scopes.push_back(new Scope());
}

//------------------------------------------------------------------------------
/**
*/
void 
LLVMGenerator::PopScope()
{
    Scope* currentScope = this->scopes.back();
    delete currentScope;
    this->scopes.pop_back();
}

//------------------------------------------------------------------------------
/**
*/
void 
LLVMGenerator::AddSymbol(Symbol* sym, llvm::Value* val)
{
    Scope* scope = this->scopes.back();
    scope->symbolMap.insert({ sym->name, val });
    scope->symbolToLlvmMap.insert({ sym, val });
}

//------------------------------------------------------------------------------
/**
*/
llvm::Value* 
LLVMGenerator::GetSymbol(Symbol* sym)
{
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto it = (*scopeIter)->symbolToLlvmMap.find(sym);
        if (it != (*scopeIter)->symbolToLlvmMap.end())
            return it->second;
        scopeIter++;
    } while (scopeIter != this->scopes.rend());
    return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
llvm::Value* 
LLVMGenerator::GetSymbol(const std::string& str)
{
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto it = (*scopeIter)->symbolMap.find(str);
        if (it != (*scopeIter)->symbolMap.end())
            return it->second;
        scopeIter++;
    } while (scopeIter != this->scopes.rend());
    return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
void 
LLVMGenerator::AddType(Symbol* sym, llvm::Type* type)
{
    Scope* scope = this->scopes.back();
    scope->symbolToLlvmTypeMap.insert({ sym, type });
}

//------------------------------------------------------------------------------
/**
*/
llvm::Type* 
LLVMGenerator::GetType(Symbol* sym)
{
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto it = (*scopeIter)->symbolToLlvmTypeMap.find(sym);
        if (it != (*scopeIter)->symbolToLlvmTypeMap.end())
            return it->second;
        scopeIter++;
    } while (scopeIter != this->scopes.rend());
    return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
void 
LLVMGenerator::AddBaseType(llvm::Value* value, TypeCode code)
{
    Scope* scope = this->scopes.back();
    scope->llvmToTypeMap.insert({ value, code });
}

//------------------------------------------------------------------------------
/**
*/
TypeCode 
LLVMGenerator::GetBaseType(llvm::Value* value)
{
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto it = (*scopeIter)->llvmToTypeMap.find(value);
        if (it != (*scopeIter)->llvmToTypeMap.end())
            return it->second;
        scopeIter++;
    } while (scopeIter != this->scopes.rend());
    return TypeCode::Void;
}

//------------------------------------------------------------------------------
/**
*/
llvm::MDNode*
CreateMetadata(llvm::LLVMContext& ctx, llvm::Module* mod, llvm::StringRef objectName, const char* tag, std::vector<uint32_t> values)
{
    llvm::Type* int32Type = llvm::Type::getInt32Ty(ctx);
    llvm::SmallVector<llvm::Metadata*, 2> metadata;

    // Setup metadata, we want to assign the gpulang.handletype based on TypeCode to our variable
    metadata.push_back(llvm::MDString::get(ctx, tag));
    for (uint32_t value : values)
    {
        llvm::Constant* metadataValue = llvm::ConstantInt::get(int32Type, value);
        metadata.push_back(llvm::ValueAsMetadata::getConstant(metadataValue));
    }
    auto node = llvm::MDTuple::get(ctx, metadata);
    auto handle = mod->getOrInsertNamedMetadata(objectName);
    handle->addOperand(node);
    
    return node;
}


std::map<AnyFX::IntrinsicOpCode, llvm::Intrinsic::GPULANGIntrinsics> intrinsicToLLVM =
{

};

std::map<std::string, std::string> typeToLLVMType =
{
    { "readWriteTexture1D", "image1D" },
    { "readWriteTexture2D", "image2D" },
    { "readWriteTexture2DMS", "image2DMS" },
    { "readWriteTextureCube", "imageCube" },
    { "readWriteTexture3D", "image3D" },
    { "readWriteTexture1DArray", "image1DArray" },
    { "readWriteTexture2DArray", "image2DArray" },
    { "readWriteTexture2DMSArray", "image2DMSArray" },
    { "readWriteTextureCubeArray", "imageCubeArray" },
    { "texture1D", "texture1D" },
    { "texture2D", "texture2D" },
    { "texture2DMS", "texture2DMS" },
    { "textureCube", "textureCube" },
    { "texture3D", "texture3D" },
    { "texture1DArray", "texture1DArray" },
    { "texture2DArray", "texture2DArray" },
    { "texture2DMSArray", "texture2DMSArray" },
    { "textureCubeArray", "textureCubeArray" },
    { "sampledTexture1D", "sampler1D" },
    { "sampledTexture2D", "sampler2D" },
    { "sampledTexture2DMS", "sampler2DMS" },
    { "sampledTextureCube", "samplerCube" },
    { "sampledTexture3D", "sampler3D" },
    { "sampledTexture1DArray", "sampler1DArray" },
    { "sampledTexture2DArray", "sampler2DArray" },
    { "sampledTexture2DMSArray", "sampler2DMSArray" },
    { "sampledTextureCubeArray", "samplerCubeArray" },
    { "inputAttachment", "subpassInput" },
    { "inputAttachmentMS", "subpassInputMS" },
    { "float", "float" },
    { "float2", "vec2" },
    { "float3", "vec3" },
    { "float4", "vec4" },
    { "int", "int" },
    { "int2", "ivec2" },
    { "int3", "ivec3" },
    { "int4", "ivec4" },
    { "uint", "uint" },
    { "uint2", "uvec2" },
    { "uint3", "uvec3" },
    { "uint4", "uvec4" },
    { "bool", "bool" },
    { "bool2", "bvec2" },
    { "bool3", "bvec3" },
    { "bool4", "bvec4" },
    { "float2x2", "mat2" },
    { "float2x3", "mat2x3" },
    { "float2x4", "mat2x4" },
    { "float3x2", "mat3x2" },
    { "float3x3", "mat3" },
    { "float3x4", "mat3x4" },
    { "float4x2", "mat4x2" },
    { "float4x3", "mat4x3" },
    { "float4x4", "mat4" },
    { "sampler", "sampler" },
    { "void", "void" }
};

//------------------------------------------------------------------------------
/**
*/
llvm::Type*
GenerateLLVMType(LLVMGenerator* gen, Type* type, const Type::FullType& fullType, bool& attachMetadata)
{
    llvm::Type* llvmType;
    switch (type->baseType)
    {
        // All handler types are treated as 32 bit integer handles with metadata attached
        case TypeCode::ReadWriteTexture1D:
        case TypeCode::ReadWriteTexture2D:
        case TypeCode::ReadWriteTexture2DMS:
        case TypeCode::ReadWriteTexture3D:
        case TypeCode::ReadWriteTextureCube:
        case TypeCode::ReadWriteTexture1DArray:
        case TypeCode::ReadWriteTexture2DArray:
        case TypeCode::ReadWriteTexture2DMSArray:
        case TypeCode::ReadWriteTextureCubeArray:
        case TypeCode::Texture1D:
        case TypeCode::Texture2D:
        case TypeCode::Texture2DMS:
        case TypeCode::Texture3D:
        case TypeCode::TextureCube:
        case TypeCode::Texture1DArray:
        case TypeCode::Texture2DArray:
        case TypeCode::Texture2DMSArray:
        case TypeCode::TextureCubeArray:
        case TypeCode::PixelCache:
        case TypeCode::PixelCacheMS:
        case TypeCode::Sampler:
            attachMetadata = true;
            llvmType = llvm::Type::getInt32Ty(gen->llvmCtx);
            break;
        case TypeCode::Bool:
            llvmType = llvm::Type::getInt1Ty(gen->llvmCtx);
            break;
        case TypeCode::Float:
            llvmType = llvm::Type::getFloatTy(gen->llvmCtx);
            break;
        case TypeCode::Float16:
            llvmType = llvm::Type::getHalfTy(gen->llvmCtx);
            break;
        case TypeCode::UInt:
        case TypeCode::Int:
            llvmType = llvm::Type::getInt32Ty(gen->llvmCtx);
            break;
        case TypeCode::UInt16:
        case TypeCode::Int16:
            llvmType = llvm::Type::getInt16Ty(gen->llvmCtx);
            break;
        case TypeCode::Void:
            llvmType = llvm::Type::getVoidTy(gen->llvmCtx);
            break;
        default:
        {
            // If type is not a known typecode, assume user defined type (struct)
            auto it = gen->structs.find(type->name);
            if (it == gen->structs.end())
                gen->Error("LLVMGenerator internal error", type);
            llvmType = it->second;
        }
    }

    // Expand to vector if applicable
    if (type->IsVector())
    {
        llvmType = llvm::FixedVectorType::get(llvmType, type->columnSize);
    }
    else if (type->IsMatrix())
    {
        // Matrix is an array of 4 vectors
        llvmType = llvm::FixedVectorType::get(llvmType, type->columnSize);
        llvmType = llvm::ArrayType::get(llvmType, type->rowSize);
    }

    // Resolve any array or pointer indirection
    for (size_t i = 0; i < fullType.modifiers.size(); i++)
    {
        const Type::FullType::Modifier& mod = fullType.modifiers[i];
        const uint32_t value = fullType.modifierValues[i];
        if (mod == Type::FullType::Modifier::ArrayLevel)
        {
            if (value != 0) // runtime size
                llvmType = llvm::ArrayType::get(llvmType, value);
            else
                llvmType = llvm::ScalableVectorType::get(llvmType, 1);
        }
        else if (mod == Type::FullType::Modifier::PointerLevel)
            llvmType = llvm::PointerType::getUnqual(llvmType);
    }

    return llvmType;
}

// To allocate our textures and sampler and any handler type, allocate an i32 constant and assign it a metadata node
//llvm::AllocaInst* val = gen->irBuilder.CreateAlloca(llvm::IntegerType::get(gen->llvmCtx, 32), nullptr);
//llvm::MDNode* metadata = llvm::MDNode::get(gen->llvmCtx, llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(gen->llvmCtx, 

llvm::Value* GenerateExpressionLLVM(LLVMGenerator* gen, Expression* expr);
void GenerateVariableLLVM(LLVMGenerator* gen, Program* program, Symbol* symbol, bool isShaderArgument);

//------------------------------------------------------------------------------
/**
*/
llvm::Value*
GenerateCallExpressionLLVM(LLVMGenerator* gen, Expression* expr)
{
    return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
llvm::Value*
GenerateInitializerExpressionLLVM(LLVMGenerator* gen, Expression* expr)
{

    return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
llvm::Value*
GenerateAccessExpressionLLVM(LLVMGenerator* gen, Expression* expr)
{
    AccessExpression* accessExpression = static_cast<AccessExpression*>(expr);
    auto resolved = Symbol::Resolved(accessExpression);


    
    llvm::Value* left = GenerateExpressionLLVM(gen, accessExpression->left);
    llvm::Value* right = GenerateExpressionLLVM(gen, accessExpression->right);
    return gen->irBuilder.CreateStore(left, right);
}

//------------------------------------------------------------------------------
/**
*/
llvm::Value*
GenerateCommaExpressionLLVM(LLVMGenerator* gen, Expression* expr)
{
    CommaExpression* commaExpression = static_cast<CommaExpression*>(expr);
    llvm::Value* left = GenerateExpressionLLVM(gen, commaExpression->left);
    llvm::Value* right = GenerateExpressionLLVM(gen, commaExpression->right);
    return right;
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateStatementLLVM(LLVMGenerator* gen, Symbol* stat)
{
    switch (stat->symbolType)
    {
        case Symbol::BreakStatementType:
            break;
        case Symbol::ContinueStatementType:
            break;
        case Symbol::ForStatementType:
            break;
        case Symbol::IfStatementType:
        {
            IfStatement* ifStatement = reinterpret_cast<IfStatement*>(stat);

            llvm::BasicBlock* currentBlock = gen->scope;

            // Setup if-block
            llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(gen->llvmCtx, "", gen->function);
            gen->scope = ifBlock;
            gen->irBuilder.SetInsertPoint(ifBlock);

            // Generate if-branch
            GenerateStatementLLVM(gen, ifStatement->ifStatement);


            // Setup else-block
            llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(gen->llvmCtx, "", gen->function);
            gen->scope = elseBlock;
            gen->irBuilder.SetInsertPoint(elseBlock);

            // Generate else-branch
            GenerateStatementLLVM(gen, ifStatement->elseStatement);

            gen->scope = currentBlock;

            llvm::Value* predicate = GenerateExpressionLLVM(gen, ifStatement->condition);
    
            gen->irBuilder.CreateCondBr(predicate, ifBlock, elseBlock);
            break;
        }
        case Symbol::ReturnStatementType:
            break;
        case Symbol::ScopeStatementType:
        {
            ScopeStatement* scope = reinterpret_cast<ScopeStatement*>(stat);
            gen->PushScope();
            for (Symbol* sym : scope->statements)
            {
                if (sym->symbolType == Symbol::VariableType)
                {
                    GenerateVariableLLVM(gen, nullptr, sym, false);
                }
                else
                {
                    GenerateStatementLLVM(gen, sym);
                }
            }
            gen->PopScope();
            break;
        }
        case Symbol::SwitchStatementType:
            break;
        case Symbol::WhileStatementType:
            break;
        case Symbol::ExpressionStatementType:
        {
            ExpressionStatement* exprStat = reinterpret_cast<ExpressionStatement*>(stat);
            GenerateExpressionLLVM(gen, exprStat->expr);
            break;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
llvm::Value*
GenerateExpressionLLVM(LLVMGenerator* gen, Expression* expr)
{
    switch (expr->symbolType)
    {
        case Symbol::AccessExpressionType:
            return GenerateAccessExpressionLLVM(gen, expr);
            break;
        case Symbol::CallExpressionType:
            return GenerateCallExpressionLLVM(gen, expr);
            break;
        case Symbol::InitializerExpressionType:
        {
            InitializerExpression* initExpression = static_cast<InitializerExpression*>(expr);
            for (Expression* expr : initExpression->values)
            {
                //gen->irBuilder.CreateStore(
            }
        }
        case Symbol::CommaExpressionType:
            return GenerateCommaExpressionLLVM(gen, expr);
            break;
        case Symbol::ArrayIndexExpressionType:
        {
            ArrayIndexExpression* arrayExpr = reinterpret_cast<ArrayIndexExpression*>(expr);
            llvm::Value* arr = GenerateExpressionLLVM(gen, arrayExpr->left);
            llvm::Value* idx = GenerateExpressionLLVM(gen, arrayExpr->right);
            return gen->irBuilder.CreateGEP(arr, idx);
        }
        case Symbol::BinaryExpressionType:
        {
            BinaryExpression* binExp = static_cast<BinaryExpression*>(expr);
            llvm::Value* left = GenerateExpressionLLVM(gen, binExp->left);
            llvm::Type* leftType = left->getType();
            llvm::Value* right = GenerateExpressionLLVM(gen, binExp->right);
            llvm::Type* rightType = right->getType();
            TypeCode leftBaseType, rightBaseType;
            leftBaseType = gen->GetBaseType(left);
            rightBaseType = gen->GetBaseType(right);
            bool floatOp = false;
            bool uintOp = false;

            // Figure out if float - if either right or left is float, the whole thing is float
            // Figure out if uint - both sides must be either uint or not, so just check one side
            if (leftType->isFloatTy() || leftType->isHalfTy()
                || rightType->isFloatTy() || rightType->isHalfTy())
                floatOp = true;
            else if (leftBaseType == TypeCode::UInt || leftBaseType == TypeCode::UInt16 
                     || rightBaseType == TypeCode::UInt || rightBaseType == TypeCode::UInt16)
                uintOp = true;

            // If float op, convert either side to float
            if (floatOp)
            {
                // Convert left side if not float
                if (!leftType->isFloatTy() && !leftType->isHalfTy())
                    if (leftType->isIntegerTy(32) || leftType->isIntegerTy(16))
                        if (leftBaseType == TypeCode::UInt || leftBaseType == TypeCode::UInt16)
                            left = gen->irBuilder.CreateUIToFP(left, gen->GetType(binExp->right));
                        else
                            left = gen->irBuilder.CreateSIToFP(left, gen->GetType(binExp->right));

                // Convert right side if not float
                else if (!rightType->isFloatTy() && !rightType->isHalfTy())
                    if (rightType->isIntegerTy(32) || rightType->isIntegerTy(16))
                        if (rightBaseType == TypeCode::UInt || rightBaseType == TypeCode::UInt16)
                            right = gen->irBuilder.CreateUIToFP(right, gen->GetType(binExp->left));
                        else
                            right = gen->irBuilder.CreateSIToFP(right, gen->GetType(binExp->left));
            }

            if (binExp->op == '=')
            {
                if (!left->getType()->isPointerTy())
                {
                    left = gen->irBuilder.CreateGEP(left->getType(), left, { 0 });
                }
            }
            else
            {
                if (left->getType()->isPointerTy())
                    left = gen->irBuilder.CreateLoad(left);
                if (right->getType()->isPointerTy())
                    right = gen->irBuilder.CreateLoad(right);
            }
            
            switch (binExp->op)
            {
                case '=':
                    return gen->irBuilder.CreateStore(right, left);

                case '+':
                    if (floatOp)
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::FAdd, left, right);
                    else
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::Add, left, right);
                case '-':
                    if (floatOp)
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::FSub, left, right);
                    else
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::Sub, left, right);
                case '*':
                    if (floatOp)
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::FMul, left, right);
                    else
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::Mul, left, right);
                    break;
                case '/':
                    if (floatOp)
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::FDiv, left, right);
                    else if (uintOp)
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::UDiv, left, right);
                    else
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::SDiv, left, right);
                case '%':
                    if (floatOp)
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::FRem, left, right);
                    else if (uintOp)
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::URem, left, right);
                    else
                        return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::SRem, left, right);
                case '^':
                    return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::Xor, left, right);
                case '|':
                    return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::Or, left, right);
                case '&':
                    return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::And, left, right);
                case '>>':
                    return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::LShr, left, right);
                case '<<':
                    return gen->irBuilder.CreateBinOp(llvm::Instruction::BinaryOps::Shl, left, right);
                case '||':
                    return gen->irBuilder.CreateLogicalOr(left, right);
                case '&&':
                    return gen->irBuilder.CreateLogicalAnd(left, right);
                case '<':
                    if (floatOp)
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::FCMP_OLT, left, right);
                    else if (uintOp)
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::ICMP_ULT, left, right);
                    else
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SLT, left, right);
                case '>':
                    if (floatOp)
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::FCMP_OGT, left, right);
                    else if (uintOp)
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::ICMP_UGT, left, right);
                    else
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SGT, left, right);
                case '<=':
                    if (floatOp)
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::FCMP_OLE, left, right);
                    else if (uintOp)
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::ICMP_ULE, left, right);
                    else
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SLE, left, right);
                case '>=':
                    if (floatOp)
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::FCMP_OGE, left, right);
                    else if (uintOp)
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::ICMP_UGE, left, right);
                    else
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::ICMP_SGE, left, right);
                case '==':
                    if (floatOp)
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::FCMP_OEQ, left, right);
                    else
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::ICMP_EQ, left, right);
                case '!=':
                    if (floatOp)
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::FCMP_ONE, left, right);
                    else
                        return gen->irBuilder.CreateCmp(llvm::CmpInst::Predicate::ICMP_NE, left, right);

            }
            break;
        }
        case Symbol::UnaryExpressionType:
        {
            UnaryExpression* unExp = static_cast<UnaryExpression*>(expr);
            llvm::Value* val = GenerateExpressionLLVM(gen, unExp->expr);
            llvm::Type* type = gen->GetType(unExp->expr);
            switch (unExp->op)
            {
                case '-':
                    return gen->irBuilder.CreateUnOp(llvm::Instruction::UnaryOps::FNeg, val);
                case '!':
                    return gen->irBuilder.CreateNot(val);
                case '*':
                    return gen->irBuilder.CreateLoad(val);
            }
            switch (unExp->postOp)
            {
                case '++':
                    return gen->irBuilder.CreateAdd(val, llvm::ConstantInt::get(llvm::Type::getInt1Ty(gen->llvmCtx), 1));
                case '--':
                    return gen->irBuilder.CreateSub(val, llvm::ConstantInt::get(llvm::Type::getInt1Ty(gen->llvmCtx), 1));
            }
            break;
        }
        case Symbol::BoolExpressionType:
        {
            BoolExpression* boolExpr = reinterpret_cast<BoolExpression*>(expr);
            auto ret = llvm::ConstantInt::get(llvm::Type::getInt1Ty(gen->llvmCtx), boolExpr->value);
            gen->AddBaseType(ret, TypeCode::Bool);
            return ret;
        }
        case Symbol::IntExpressionType:
        {
            IntExpression* intExpr = reinterpret_cast<IntExpression*>(expr);
            auto ret = llvm::ConstantInt::get(llvm::Type::getInt32Ty(gen->llvmCtx), intExpr->value);
            gen->AddBaseType(ret, TypeCode::Int);
            return ret;
        }
        case Symbol::UIntExpressionType:
        {
            UIntExpression* uintExpr = reinterpret_cast<UIntExpression*>(expr);
            auto ret = llvm::ConstantInt::get(llvm::Type::getInt32Ty(gen->llvmCtx), uintExpr->value);
            gen->AddBaseType(ret, TypeCode::UInt);
            return ret;
        }
        case Symbol::FloatExpressionType:
        {
            FloatExpression* floatExpr = reinterpret_cast<FloatExpression*>(expr);
            auto ret = llvm::ConstantInt::get(llvm::Type::getFloatTy(gen->llvmCtx), floatExpr->value);
            gen->AddBaseType(ret, TypeCode::Float);
            return ret;
        }
        case Symbol::TernaryExpressionType:
        {
            TernaryExpression* ternExpr = reinterpret_cast<TernaryExpression*>(expr);
            llvm::Value* cmp = GenerateExpressionLLVM(gen, ternExpr->lhs);
            llvm::Value* ifVal = GenerateExpressionLLVM(gen, ternExpr->ifExpression);
            llvm::Value* elseVal = GenerateExpressionLLVM(gen, ternExpr->elseExpression);
            return gen->irBuilder.CreateSelect(cmp, ifVal, elseVal);
        }
        case Symbol::SymbolExpressionType:
        {
            SymbolExpression* symExpr = reinterpret_cast<SymbolExpression*>(expr);
            llvm::Value* val = gen->GetSymbol(symExpr->symbol);
            if (val == nullptr)
            {
                gen->Error("LLVMGenerator internal error", symExpr);
                return nullptr;
            }
            return val;
        }
    }
    return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
void 
GenerateFunctionLLVM(LLVMGenerator* gen, Program* program, Symbol* symbol)
{
    Function* fun = reinterpret_cast<Function*>(symbol);
    Function::__Resolved* funResolved = Symbol::Resolved(fun);

    bool isMain = fun == gen->mainFunction;

    bool dummy;
    llvm::Type* returnType = GenerateLLVMType(gen, funResolved->returnTypeSymbol, fun->returnType, dummy);
    llvm::SmallVector<llvm::Type*, 4> args;
    for (Variable* sym : fun->parameters)
    {
        //GenerateVariableLLVM(gen, compiler, program, sym, isMain);
        llvm::Type* type = gen->GetType(sym);
        if (type == nullptr)
        {
            gen->Error("LLVMGenerator internal error", symbol);
            return;
        }
            
        args.push_back(type);
    }

    // Create or get function signature
    llvm::FunctionType* llvmFunType = llvm::FunctionType::get(returnType, args, false);
    gen->AddType(fun, llvmFunType);

    // Create actual function
    llvm::Function* llvmFun = llvm::Function::Create(llvmFunType, llvm::GlobalValue::ExternalLinkage, fun->name, gen->mod);
    gen->AddSymbol(fun, llvmFun);

    // Generate scope
    if (fun->ast)
    {
        gen->function = llvmFun;

        // Create basic block
        gen->scope = llvm::BasicBlock::Create(gen->llvmCtx, "", llvmFun);
        gen->irBuilder.SetInsertPoint(gen->scope);

        // Push scope for variables within function
        gen->PushScope();

        // Traverse generated arguments and setup names
        auto argIt = llvmFun->arg_begin();
        auto funParamIt = fun->parameters.begin();
        while (argIt != llvmFun->arg_end())
        {
            // Assign input/output binding
            Variable::__Resolved* varResolved = Symbol::Resolved((*funParamIt));
            llvm::MDNode* metadata = CreateMetadata(gen->llvmCtx, gen->mod, varResolved->name, "gpulang.iobindings", { varResolved->parameterBits.bits });

            // Create mapping between variable and metadata
            gen->symbolToMetadataMap.insert({ *funParamIt, metadata });
            gen->llvmToMetadataMap.insert({ argIt, metadata });

            // Add symbol to scope
            argIt->setName((*funParamIt)->name);
            gen->AddSymbol((*funParamIt), argIt);
            gen->AddType((*funParamIt), argIt->getType());

            funParamIt++;
            argIt++;
        }

        // Generate the code for the scope of the function
        GenerateStatementLLVM(gen, fun->ast);

        // Pop scope back to previous
        gen->PopScope();
    }
    gen->scope = nullptr;
    gen->function = nullptr;
}

//------------------------------------------------------------------------------
/**
*/
void 
GenerateStructureLLVM(LLVMGenerator* gen, Program* program, Symbol* symbol)
{
    Structure* struc = reinterpret_cast<Structure*>(symbol);
    llvm::SmallVector<llvm::Type*, 8> members;
    std::vector<uint32_t> paddings, offsets;
    for (Symbol* sym : struc->symbols)
    {
        if (sym->symbolType == Symbol::SymbolType::VariableType)
        {
            Variable* var = static_cast<Variable*>(sym);

            // Make sure to generate the variable (which declares the type and such) prior to building the struct
            GenerateVariableLLVM(gen, program, var, false);

            // Lookup type
            llvm::Type* llvmType = gen->GetType(var);
            if (llvmType == nullptr)
            {
                gen->Error("LLVMGenerator internal error", symbol);
                return;
            }
            members.push_back(llvmType);
            Variable::__Resolved* res = Symbol::Resolved(var);
            paddings.push_back(res->elementPadding);
            offsets.push_back(res->structureOffset);
        }
    }

    // Construct struct type
    llvm::StructType* llvmStruct = llvm::StructType::create(gen->llvmCtx, members, struc->name);
    gen->structs.insert({ struc->name, llvmStruct });

    // Insert type lookup
    gen->AddType(symbol, llvmStruct);

    // Add offsets and paddings if buffer is mutable or uniform
    Structure::__Resolved* strucResolved = Symbol::Resolved(struc);
    if (strucResolved->usageFlags.flags.isMutableBuffer || strucResolved->usageFlags.flags.isUniformBuffer)
    {
        llvm::MDNode* metadata = CreateMetadata(gen->llvmCtx, gen->mod, struc->name, "gpulang.structpaddings", paddings);
        gen->symbolToMetadataMap.insert({ struc, metadata });
        gen->llvmTypeToMetadataMap.insert({ llvmStruct, metadata });

        metadata = CreateMetadata(gen->llvmCtx, gen->mod, struc->name, "gpulang.structoffsets", offsets);
        gen->symbolToMetadataMap.insert({ struc, metadata });
        gen->llvmTypeToMetadataMap.insert({ llvmStruct, metadata });

        metadata = CreateMetadata(gen->llvmCtx, gen->mod, struc->name, "gpulang.structtype", { strucResolved->usageFlags.flags.isMutableBuffer ? 0u : 1u });
        gen->symbolToMetadataMap.insert({ struc, metadata });
        gen->llvmTypeToMetadataMap.insert({ llvmStruct, metadata });
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
GenerateVariableLLVM(LLVMGenerator* gen, Program* program, Symbol* symbol, bool isShaderArgument)
{
    Variable* var = reinterpret_cast<Variable*>(symbol);

    Variable::__Resolved* varResolved = Symbol::Resolved(var);
    bool attachMetadata = false;
    llvm::Type* llvmType = GenerateLLVMType(gen, varResolved->typeSymbol, varResolved->type, attachMetadata);

    // Insert type lookup
    gen->AddType(symbol, llvmType);

    // If struct member, stop here
    if (varResolved->usageBits.flags.isStructMember)
        return;

    if (varResolved->usageBits.flags.isParameter)
    {
        // Parameters are handled by the function setup
    }
    else if (gen->scopes.size() == 1) 
    {
        // Global variable (declared in main scope)
        llvm::GlobalVariable* llvmVar = static_cast<llvm::GlobalVariable*>(gen->mod->getOrInsertGlobal(var->name, llvmType));
        llvmVar->setName(var->name);
        gen->AddBaseType(llvmVar, varResolved->typeSymbol->baseType);

        // Assign inbinding and outbinding slots as metadata
        llvm::MDNode* metadata = CreateMetadata(gen->llvmCtx, gen->mod, varResolved->name, "gpulang.bindings", { varResolved->inBinding, varResolved->outBinding });

        // If we have a non-scalar type (texture, sampler, pixelcache)
        if (attachMetadata)
        {
            // Create metadata
            llvm::MDNode* metadata = CreateMetadata(gen->llvmCtx, gen->mod, varResolved->name, "gpulang.handletype", { (uint32_t)varResolved->typeSymbol->baseType });

            // Save metadata for this node
            gen->symbolToMetadataMap.insert({ var, metadata });
            gen->llvmToMetadataMap.insert({ llvmVar, metadata });
        }

        gen->symbolToMetadataMap.insert({ var, metadata });
        gen->llvmToMetadataMap.insert({ llvmVar, metadata });

        if (varResolved->value)
            GenerateExpressionLLVM(gen, varResolved->value);
    }
    else
    {
        // Local variable
        llvm::AllocaInst* alloc = gen->irBuilder.CreateAlloca(llvmType);
        alloc->setName(var->name);
        gen->AddBaseType(alloc, varResolved->typeSymbol->baseType);
        gen->AddSymbol(var, alloc);

        if (varResolved->value)
            GenerateExpressionLLVM(gen, varResolved->value);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
GenerateSymbolLLVM(LLVMGenerator* gen, Program* program, Symbol* sym)
{
    switch (sym->symbolType)
    {
        case Symbol::SymbolType::VariableType:
            GenerateVariableLLVM(gen, program, sym, false);
            break;
        case Symbol::SymbolType::StructureType:
            GenerateStructureLLVM(gen, program, sym);
            break;
        case Symbol::SymbolType::FunctionType:
            GenerateFunctionLLVM(gen, program, sym);
            break;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
LLVMGenerator::Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    // Get pointer to main scope
    this->symbolLookup = &compiler->scopes[0]->symbolLookup;
    Program::__Resolved* progResolved = Symbol::Resolved(program);
    for (auto it : progResolved->programMappings)
    {
        // For each shader entry, generate LLVM
        if (it.first >= Program::__Resolved::VertexShader && it.first <= Program::__Resolved::RayIntersectionShader)
        {
            
            this->mod = new llvm::Module(program->name, this->llvmCtx);
            this->mainFunction = static_cast<Function*>(it.second);
            for (Symbol* sym : symbols)
            {
                GenerateSymbolLLVM(this, program, sym);
            }

            // Just print output
            this->mod->dump();
            delete this->mod;
        }
    }
    //this->mod->print(llvm::outs(), nullptr, true, true);

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void 
LLVMGenerator::Error(const std::string& msg, Symbol* symbol)
{
    printf("LLVMGenerator internal error");
    abort();
}

} // namespace AnyFX
