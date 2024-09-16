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
#include <vector>
namespace GPULang
{

struct Variable;
struct Type;
struct Statement;
struct Function : public Symbol
{
    /// constructor
    Function();
    /// destructor
    ~Function();

    Statement* ast;
    Type::FullType returnType;
    std::string body;
    bool hasBody;
    std::vector<Variable*> parameters;
    _IMPLEMENT_ANNOTATIONS()
    _IMPLEMENT_ATTRIBUTES()

    static Symbol* MatchOverload(Compiler* compiler, const std::vector<Symbol*>& functions, const std::vector<Type::FullType>& args, bool allowImplicitConversion = false);

    static std::vector<Symbol*> SetupIntrinsics();

    /// returns true if functions are compatible
    bool IsCompatible(Function* otherFunction, bool checkReturnType = false);

    struct __Resolved : public Symbol::__Resolved
    {
        Type* returnTypeSymbol;

        std::string name;
        std::string signature;

        unsigned int computeShaderWorkGroupSize[3];
        bool earlyDepth;

        static const uint8_t INVALID_SIZE = 0xF;
        uint32_t invocations;
        uint32_t maxOutputVertices;
        uint32_t patchSize;

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

        WindingOrder windingOrder;

        enum PrimitiveTopology
        {
            InvalidPrimitiveTopology,
            Points,
            Lines,
            LinesAdjacency,
            Triangles,
            TrianglesAdjacency,
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

            return InvalidPrimitiveTopology;
        };

        PrimitiveTopology inputPrimitiveTopology;
        PrimitiveTopology outputPrimitiveTopology;

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

        PatchType patchType;

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

        PartitionMethod partitionMethod;

        enum PixelOrigin
        {
            InvalidPixelOrigin,
            Lower,      // lower left corner
            Upper,      // upper right corner
            Center      // pixel center
        };
        PixelOrigin pixelOrigin;

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

        bool isShader;
        bool isPrototype;

        union ShaderUsage
        {
            struct
            {
                uint32_t isVertexShader : 1;
                uint32_t isHullShader : 1;
                uint32_t isDomainShader : 1;
                uint32_t isGeometryShader : 1;
                uint32_t isPixelShader : 1;
                uint32_t isComputeShader : 1;
                uint32_t isRayGenerationShader : 1;
                uint32_t isRayMissShader : 1;
                uint32_t isRayHitShader : 1;
                uint32_t isRayClosestHitShader : 1;
                uint32_t isRayAnyHitShader : 1;
                uint32_t isRayIntersectionShader : 1;
            } flags;
            uint32_t bits;
        };
        ShaderUsage shaderUsage;
    };

    

};

} // namespace GPULang
