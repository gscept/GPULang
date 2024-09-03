#pragma once
//------------------------------------------------------------------------------
/**
    Generates LLVM output from frontend

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "generator.h"
#include "llvm/IR/IRBuilder.h"
#include "ast/types/type.h"

namespace AnyFX
{

class LLVMGenerator : public Generator
{

public:
    enum class Backend
    {
        SPIRV,
        AMDGPU
    };

    /// constructor
    LLVMGenerator(Backend backend);

    /// generate LLVM
    bool Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;

    Backend backend;
    llvm::Module* mod;
    llvm::LLVMContext llvmCtx;
    llvm::IRBuilder<> irBuilder;

    llvm::BasicBlock* scope;
    llvm::Function* function;

    std::map<std::string, llvm::StructType*> structs;

    struct Scope
    {
        std::map<std::string, llvm::Value*> symbolMap;
        std::map<Symbol*, llvm::Value*> symbolToLlvmMap;
        std::map<Symbol*, llvm::Type*> symbolToLlvmTypeMap;
        std::map<llvm::Value*, TypeCode> llvmToTypeMap;
        std::multimap<Symbol*, llvm::MDNode*> symbolToMetadataMap;
    };
    std::vector<Scope*> scopes;
    Type* typeScope;

    std::multimap<std::string, Symbol*>* symbolLookup;

    /// Produce error
    void Error(const std::string& msg, Symbol* symbol);
    
    /// Push new scope
    void PushScope();
    /// Pop scope
    void PopScope();

    /// Add symbol->llvm value association to current scope
    void AddSymbol(Symbol* sym, llvm::Value* val);
    /// Get LLVM value based on symbol
    llvm::Value* GetSymbol(Symbol* sym);
    /// Get LLVM value based on symbol name
    llvm::Value* GetSymbol(const std::string& str);
    /// Add symbol->llvm type association to the current scope
    void AddType(Symbol* sym, llvm::Type* type);
    /// Get LLVM type based on symbol
    llvm::Type* GetType(Symbol* sym);
    /// Add llvm->base type
    void AddBaseType(llvm::Value* value, TypeCode code);
    /// Get llvm base type
    TypeCode GetBaseType(llvm::Value* value);

    std::multimap<Symbol*, llvm::MDNode*> symbolToMetadataMap;
    std::multimap<llvm::Value*, llvm::MDNode*> llvmToMetadataMap;
    std::multimap<llvm::Type*, llvm::MDNode*> llvmTypeToMetadataMap;
};

} // namespace AnyFX
