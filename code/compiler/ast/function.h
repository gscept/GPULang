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
    Symbol::Location returnTypeLocation;
    Symbol* constructorType = nullptr;
    bool hasBody;
    bool compileTime;
    FixedArray<Variable*> parameters;
    _IMPLEMENT_ANNOTATIONS()
    _IMPLEMENT_ATTRIBUTES()

    static Symbol* MatchOverload(Compiler* compiler, const std::vector<Symbol*>& functions, const std::vector<Type::FullType>& args, bool allowImplicitConversion = false);

    static void SetupIntrinsics();

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

        enum WindingOrder
        {
            InvalidWindingOrder,
            Clockwise,
            CounterClockwise
        };

        /// convert from string
        static const WindingOrder WindingOrderFromString(const TransientString& str)
        {
            if (str == "cw" || str == "clockwise")
                return Clockwise;
            else if (str == "ccw" || str == "counter_clockwise")
                return CounterClockwise;

            return InvalidWindingOrder;
        };

        enum PrimitiveTopology
        {
            InvalidPrimitiveTopology,
            Points,
            Lines,
            LinesAdjacency,
            Triangles,
            TrianglesAdjacency,
            Quads,
            Isolines
        };

        /// convert from string
        static const PrimitiveTopology PrimitiveTopologyFromString(const TransientString& str)
        {
            if (str == "points")
                return PrimitiveTopology::Points;
            else if (str == "lines")
                return PrimitiveTopology::Lines;
            else if (str == "lines_adjacency")
                return PrimitiveTopology::LinesAdjacency;
            else if (str == "triangles")
                return PrimitiveTopology::Triangles;
            else if (str == "triangles_adjacency")
                return PrimitiveTopology::TrianglesAdjacency;
            else if (str == "quads")
                return PrimitiveTopology::Quads;
            else if (str == "isolines")
                return PrimitiveTopology::Isolines;

            return InvalidPrimitiveTopology;
        };


        enum PatchType
        {
            InvalidPatchType,
            IsolinePatch,
            TrianglePatch,
            QuadPatch
        };

        /// convert from string
        static const PatchType PatchTypeFromString(const TransientString& str)
        {
            if (str == "isolines")
                return PatchType::IsolinePatch;
            else if (str == "triangles")
                return PatchType::TrianglePatch;
            else if (str == "quads")
                return PatchType::QuadPatch;

            return InvalidPatchType;
        };


        enum PartitionMethod
        {
            InvalidPartitionMethod,
            IntegerSteps,
            FloatEven,
            FloatOdd           
        };
                /// convert from string
        static const PartitionMethod PartitionMethodFromString(const TransientString& str)
        {
            if (str == "steps" || str == "integer")
                return PartitionMethod::IntegerSteps;
            else if (str == "even" || str == "fract_even")
                return PartitionMethod::FloatEven;
            else if (str == "odd" || str == "fract_odd")
                return PartitionMethod::FloatOdd;

            return InvalidPartitionMethod;
        };


        enum PixelOrigin
        {
            InvalidPixelOrigin,
            Lower,      // lower left corner
            Upper,      // upper right corner
            Center      // pixel center
        };

        static const PixelOrigin PixelOriginFromString(const TransientString& str)
        {
            if (str == "lower_left" || str == "lower")
                return PixelOrigin::Lower;
            else if (str == "upper_left" || str == "upper")
                return PixelOrigin::Upper;
            else if (str == "center")
                return PixelOrigin::Center;

            return PixelOrigin::InvalidPixelOrigin;
        }

        enum ComputeDerivativeIndexing
        {
            NoDerivatives,
            DerivativeIndexLinear,
            DerivativeIndexQuad
        };


        struct ExecutionModifiers
        {
            unsigned int computeShaderWorkGroupSize[3] = {0};
            unsigned int groupSize = 0;
            unsigned int groupsPerWorkgroup = 0;
            uint32_t earlyDepth : 1 = 0;
            uint32_t depthAlwaysGreater : 1 = 0;
            uint32_t depthAlwaysLesser : 1 = 0;

            uint32_t invocations = 0;

            uint32_t maxOutputVertices = 0;
            PatchType patchType = PatchType::InvalidPatchType;

            WindingOrder windingOrder = WindingOrder::InvalidWindingOrder;
            PrimitiveTopology inputPrimitiveTopology = PrimitiveTopology::InvalidPrimitiveTopology;
            PrimitiveTopology outputPrimitiveTopology = PrimitiveTopology::InvalidPrimitiveTopology;

            PartitionMethod partitionMethod = PartitionMethod::InvalidPartitionMethod;
            PixelOrigin pixelOrigin = PixelOrigin::InvalidPixelOrigin;
            ComputeDerivativeIndexing computeDerivativeIndexing = ComputeDerivativeIndexing::NoDerivatives;

        } executionModifiers;

        bool isEntryPoint;
        bool isPrototype;
        
        PinnedSet<Symbol*> visibleSymbols;
    } functionResolved;
};

} // namespace GPULang
