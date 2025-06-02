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

extern std::vector<Symbol*> DefaultIntrinsics;
struct Variable;
struct Type;
struct Statement;
struct Function : public Symbol
{
    /// constructor
    Function();
    /// destructor
    virtual ~Function();

    Statement* ast;
    Type::FullType returnType;
    Symbol::Location returnTypeLocation;
    bool hasBody;
    bool compileTime;
    std::vector<Variable*> parameters;
    _IMPLEMENT_ANNOTATIONS()
    _IMPLEMENT_ATTRIBUTES()

    static Symbol* MatchOverload(Compiler* compiler, const std::vector<Symbol*>& functions, const std::vector<Type::FullType>& args, bool allowImplicitConversion = false);

    static void SetupIntrinsics();

    /// returns true if functions are compatible
    bool IsCompatible(Function* otherFunction, bool checkReturnType = false);

    struct __Resolved : public Symbol::__Resolved
    {
        virtual ~__Resolved() {};
        Type* returnTypeSymbol;
        Storage returnValueStorage;
        Scope scope;

        std::string name;
        std::string nameWithVarNames;
        std::string signature;
        bool hasExplicitReturn;
        
        static const uint8_t INVALID_SIZE = 0xF;

        enum WindingOrder
        {
            InvalidWindingOrder,
            Clockwise,
            CounterClockwise
        };

        /// convert from string
        static const WindingOrder WindingOrderFromString(const std::string& str)
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
        static const PrimitiveTopology PrimitiveTopologyFromString(const std::string& str)
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
        static const PatchType PatchTypeFromString(const std::string& str)
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
        static const PartitionMethod PartitionMethodFromString(const std::string& str)
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

        static const PixelOrigin PixelOriginFromString(const std::string& str)
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
            unsigned int computeShaderWorkGroupSize[3];
            unsigned int groupSize;
            unsigned int groupsPerWorkgroup;
            uint32_t earlyDepth : 1;
            uint32_t depthAlwaysGreater : 1;
            uint32_t depthAlwaysLesser : 1;

            uint32_t invocations;

            uint32_t maxOutputVertices;
            PatchType patchType;

            WindingOrder windingOrder;
            PrimitiveTopology inputPrimitiveTopology;
            PrimitiveTopology outputPrimitiveTopology;

            PartitionMethod partitionMethod;
            PixelOrigin pixelOrigin;
            ComputeDerivativeIndexing computeDerivativeIndexing;

        } executionModifiers;

        bool isEntryPoint;
        bool isPrototype;

        PinnedSet<Function*> visibilityMap = 0xFFFF;
    };

    

};

} // namespace GPULang
