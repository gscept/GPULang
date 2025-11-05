#pragma once
//------------------------------------------------------------------------------
/**
    AST for Function 
            
    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "attribute.h"
#include "annotation.h"
#include "variable.h"
#include "shaderusage.h"
#include <vector>
#include <set>

namespace GPULang
{

struct Variable;
struct Type;
struct Statement;
struct Structure;
struct Enumeration;
struct Function : public Symbol
{
    /// constructor
    Function();
    /// destructor
    virtual ~Function();

    Statement* ast;
    Type::FullType returnType;
    Symbol* constructorType = nullptr;
    Storage returnTypeStorage;
    bool hasBody;
    bool compileTime;
    FixedArray<Variable*> parameters;
    uint64_t backendIndex = UINT64_MAX;
    _IMPLEMENT_ANNOTATIONS()
    _IMPLEMENT_ATTRIBUTES()

    static Symbol* MatchOverload(Compiler* compiler, const std::vector<Symbol*>& functions, const std::vector<Type::FullType>& args, bool allowImplicitConversion = false);


    /// returns true if functions are compatible
    bool IsCompatible(Function* otherFunction, bool checkReturnType = false);
    
    /// Resolve as a builtin (intrinsic or type constructor) function
    void SetupAsBuiltin(const FixedArray<Variable*>& parameters);

    struct __Resolved : public Symbol::__Resolved
    {
        virtual ~__Resolved() { this->visibleSymbols.data.Invalidate(); };
        Type* returnTypeSymbol = nullptr;
        Scope scope;

        FixedString name;
        FixedString nameWithVarNames;
        FixedString signature;
        bool hasExplicitReturn;
        
        static const uint8_t INVALID_SIZE = 0xF;


        struct ExecutionModifiers
        {
            unsigned int computeShaderWorkGroupSize[3] = {0};
            unsigned int groupSize = 0;
            unsigned int groupsPerWorkgroup = 0;
            uint32_t earlyDepth : 1 = 0;
            uint32_t depthAlwaysGreater : 1 = 0;
            uint32_t depthAlwaysLesser : 1 = 0;

            uint32_t invocations = 0;

            uint32_t patchSize = 0;                     // Hull shader patch size
            uint32_t maxOutputVertices = 0;             // Maximum allowed vertices to spawn from geometry shader

            uint32_t patchType = 0x0;                   // Managed by the generated Patch type
            uint32_t windingOrder = 0x0;                // Managed by the generated Winding type
            uint32_t inputPrimitiveTopology = 0x0;      // Managed by the generated InputTopology type
            uint32_t outputPrimitiveTopology = 0x0;     // Managed by the generated OutputTopology type

            uint32_t partitionMethod = 0x0;
            uint32_t pixelOrigin = 0x0;
            uint32_t computeDerivativeIndexing = 0x0;
            uint32_t layerOrViewportOutput = 0x0;

        } executionModifiers;

        bool isEntryPoint;
        bool isPrototype;
        
        PinnedSet<Symbol*> visibleSymbols;
    } functionResolved;
};

} // namespace GPULang
