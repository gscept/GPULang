/**
 * GPULang Syntax Loader
 * Shared module for loading syntax data and setting up Prism highlighting
 */

let syntaxData = null;

// Load syntax data and create custom Prism language
async function loadSyntaxData() {
    try {
        console.log('Attempting to load syntax.json...');
        const response = await fetch('syntax.json');
        console.log('Fetch response status:', response.status);
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        syntaxData = await response.json();
        console.log('Syntax data loaded successfully:', syntaxData);
        createGPULangPrismLanguage(syntaxData);
        
        // Re-highlight all code blocks after language is loaded
        if (typeof Prism !== 'undefined') {
            Prism.highlightAll();
        }
    } catch (error) {
        console.error('Error loading syntax data:', error);
        console.log('Falling back to embedded syntax data...');
        // Fallback to embedded syntax data - only what's actually in the file
        syntaxData = {
            "keywords": [
                "const", "var", "uniform", "mutable", "sampled", "literal", "in", "out", 
                "return", "break", "discard", "ray_ignore", "for", "while", "if", "else", 
                "switch", "case", "struct", "render_state", "sampler_state", "program", 
                "entry_point", "binding", "group", "enum", "generate", "#include", "#pragma"
            ],
            "types": [
                "f32", "u32", "i32", "b8", "f16", "u16", "i16",
                "f32x2", "u32x2", "i32x2", "b8x2", "f16x2", "u16x2", "i16x2",
                "f32x3", "u32x3", "i32x3", "b8x3", "f16x3", "u16x3", "i16x3",
                "f32x4", "u32x4", "i32x4", "b8x4", "f16x4", "u16x4", "i16x4",
                "f32x2x2", "f32x2x3", "f32x2x4", "f32x3x2", "f32x3x3", "f32x3x4",
                "f32x4x2", "f32x4x3", "f32x4x4", "f16x2x2", "f16x2x3", "f16x2x4",
                "f16x3x2", "f16x3x3", "f16x3x4", "f16x4x2", "f16x4x3", "f16x4x4",
                "texture1D", "texture1DArray", "texture2D", "texture2DMS", "texture2DMSArray",
                "texture2DArray", "texture3D", "textureCube", "textureCubeArray",
                "pixelCache", "pixelCacheMS", "sampler", "Function", "accelerationStructure",
                "Void", "CompareMode", "StencilOp", "ExecutionScope", "MemorySemantics",
                "PolygonMode", "CullFace", "WindingOrder", "LogicOp", "BlendFactor",
                "BlendOperation", "BlendColorMask", "FilterMode", "AddressMode", "Color",
                "StencilState", "RenderState", "SamplerState", "Program"
            ],
            "builtin_functions": [
                // Type constructors (same as types)
                "f32", "u32", "i32", "b8", "f16", "u16", "i16",
                "f32x2", "u32x2", "i32x2", "b8x2", "f16x2", "u16x2", "i16x2",
                "f32x3", "u32x3", "i32x3", "b8x3", "f16x3", "u16x3", "i16x3",
                "f32x4", "u32x4", "i32x4", "b8x4", "f16x4", "u16x4", "i16x4",
                "f32x2x2", "f32x2x3", "f32x2x4", "f32x3x2", "f32x3x3", "f32x3x4",
                "f32x4x2", "f32x4x3", "f32x4x4", "f16x2x2", "f16x2x3", "f16x2x4",
                "f16x3x2", "f16x3x3", "f16x3x4", "f16x4x2", "f16x4x3", "f16x4x4",
                // Math functions
                "acos", "acosh", "asin", "asinh", "atan", "atanh", "cos", "cosh",
                "exp", "exp2", "invSqrt", "log", "log2", "sin", "sinh", "sqrt", "tan", "tanh",
                "pow", "mad", "dot", "reflect", "refract", "cross", "length", "normalize",
                "distance", "min", "max", "clamp", "lerp", "step", "smoothstep",
                "ceil", "floor", "fract", "saturate", "trunc", "ddx", "ddy", "fwidth",
                "sign", "abs", "any", "all", "transpose", "inverse",
                // Cast functions
                "castToF16", "castToU16", "castToI16", "castToF32", "castToU32", "castToI32",
                // Vertex functions
                "vertexGetOutputLayer", "vertexGetOutputViewport", "vertexGetIndex",
                "vertexGetInstanceIndex", "vertexGetBaseIndex", "vertexGetBaseInstanceIndex",
                "vertexGetDrawIndex", "vertexSetOutputLayer", "vertexSetOutputViewport",
                "vertexExportCoordinates",
                // Geometry functions
                "geometryExportVertex", "geometryExportPrimitive",
                // Pixel functions
                "pixelGetCoordinates", "pixelGetDepth", "pixelSetDepth", "pixelExportColor",
                // Compute functions
                "computeGetLocalThreadIndices", "computeGetGlobalThreadIndices",
                "computeGetWorkgroupIndices", "computeGetWorkGroupDimensions", "computeGetIndexInWorkgroup",
                // Subgroup functions
                "subgroupGetId", "subgroupGetSize", "subgroupGetNum", "subgroupGetThreadMask",
                "subgroupGetThreadAndLowerMask", "subgroupGetLowerMask", "subgroupGetThreadAndGreaterMask",
                "subgroupGetGreaterMask", "subgroupGetFirstActiveThread", "subgroupBroadcastFirstActiveThread",
                "subgroupBallot", "subgroupInverseBallot", "subgroupBallotBitCount",
                "subgroupBallotFirstOne", "subgroupBallotLastOne", "subgroupBallotBit",
                "subgroupSwapDiagonal", "subgroupSwapVertical", "subgroupSwapHorizontal",
                // Atomic functions
                "atomicLoad", "atomicIncrement", "atomicDecrement", "atomicStore", "atomicExchange",
                "atomicAdd", "atomicSubtract", "atomicAnd", "atomicOr", "atomicXor", "atomicCompareExchange",
                // Bit functions
                "bitInsert", "bitExtract", "bitReverse", "bitCount",
                // Execution and memory barriers
                "executionBarrier", "executionBarrierSubgroup", "executionBarrierWorkgroup",
                "memoryBarrier", "memoryBarrierBuffer", "memoryBarrierTexture",
                "memoryBarrierAtomic", "memoryBarrierSubgroup", "memoryBarrierWorkgroup",
                // Texture functions
                "textureGetSize", "textureGetSizeMip", "textureGetMips", "textureGetSamples",
                "textureGetSampledMip", "textureLoad", "textureLoadMip", "textureStore",
                "textureStoreMip", "textureFetch", "textureFetchSample", "textureGather",
                "textureGatherOffset", "texturePixelCacheLoad", "textureSample", "textureSampleOffset",
                "textureSampleProj", "textureSampleProjOffset", "textureSampleCompare",
                "textureSampleCompareOffset", "textureSampleProjCompare", "textureSampleProjCompareOffset",
                "textureSampleLod", "textureSampleLodOffset", "textureSampleLodProj",
                "textureSampleLodProjOffset", "textureSampleLodCompare", "textureSampleLodCompareOffset",
                "textureSampleLodProjCompare", "textureSampleLodProjCompareOffset",
                "textureSampleGrad", "textureSampleGradOffset", "textureSampleGradProj",
                "textureSampleGradProjOffset", "textureSampleGradCompare", "textureSampleGradCompareOffset",
                "textureSampleGradProjCompare", "textureSampleGradProjCompareOffset",
                "textureSampleBias", "textureSampleBiasOffset", "textureSampleBiasProj",
                "textureSampleBiasProjOffset", "textureSampleBiasCompare", "textureSampleBiasCompareOffset",
                "textureSampleBiasProjCompare", "textureSampleBiasProjCompareOffset"
            ]
        };
        console.log('Using embedded syntax data');
        createGPULangPrismLanguage(syntaxData);
        
        // Re-highlight all code blocks after language is loaded
        if (typeof Prism !== 'undefined') {
            Prism.highlightAll();
        }
    }
}

// Create dynamic Prism language definition from loaded syntax data
function createGPULangPrismLanguage(syntaxData) {
    console.log('Creating GPULang Prism language...');
    
    if (!syntaxData) {
        console.warn('No syntax data available, using basic highlighting');
        createBasicGPULangPrismLanguage();
        return;
    }

    console.log('Syntax data available, creating dynamic language definition');

    // Define the GPULang language for Prism
    if (typeof Prism !== 'undefined') {
        Prism.languages.gpulang = {
            'comment': [
                {
                    pattern: /(^|[^\\])\/\*[\s\S]*?(?:\*\/|$)/,
                    lookbehind: true,
                    greedy: true
                },
                {
                    pattern: /(^|[^\\:])\/\/.*/,
                    lookbehind: true,
                    greedy: true
                }
            ],
            'string': {
                pattern: /"(?:\\.|[^\\"\r\n])*"/,
                greedy: true
            },
            'directive': {
                pattern: /#\w+/,
                alias: 'keyword'
            },
            'keyword': {
                pattern: new RegExp(`\\b(?:${syntaxData.keywords.join('|')})\\b`),
                alias: 'keyword'
            },
            'builtin-function': {
                pattern: new RegExp(`\\b(?:${syntaxData.builtin_functions.join('|')})(?=\\s*\\()`),
                alias: 'function'
            },
            'type': {
                pattern: new RegExp(`\\b(?:${syntaxData.types.join('|')})\\b`),
                alias: 'type'
            },
            'function:': {
                pattern: /\b[a-zA-Z_]\w*(?=\s*\()/,
                alias: 'function'
            },
            'annotation': {
                pattern: /@\w+/,
                alias: 'symbol'
            },
            'number': /\b0x[\da-f]+\b|(?:\b\d+(?:\.\d*)?|\B\.\d+)(?:e[+-]?\d+)?[fl]?\b/i,
            'operator': /[+\-*\/=!<>&|]+/,
            'punctuation': /[{}[\];(),.:]/
        };

        // Register alternative names
        Prism.languages.gpul = Prism.languages.gpulang;
        console.log('GPULang Prism language created successfully');
    }
}

// Fallback basic syntax highlighting
function createBasicGPULangPrismLanguage() {
    if (typeof Prism !== 'undefined') {
        Prism.languages.gpulang = {
            'comment': [
                {
                    pattern: /(^|[^\\])\/\*[\s\S]*?(?:\*\/|$)/,
                    lookbehind: true,
                    greedy: true
                },
                {
                    pattern: /(^|[^\\:])\/\/.*/,
                    lookbehind: true,
                    greedy: true
                }
            ],
            'string': {
                pattern: /"(?:\\.|[^\\"\r\n])*"/,
                greedy: true
            },
            'keyword': /\b(?:struct|uniform|sampler_state|render_state|entry_point|program|binding|in|out|const|if|else|while|for|return|discard|void|let)\b/,
            'builtin': /\b(?:f32|f32x2|f32x3|f32x4|i32|u32|bool|texture2D|textureCube)\b/,
            'function': /\b(?:textureSample|normalize|dot|cross|length|mix|clamp|vertexExportCoordinates|pixelExportColor)\b/,
            'annotation': /@\w+/,
            'number': /\b0x[\da-f]+\b|(?:\b\d+(?:\.\d*)?|\B\.\d+)(?:e[+-]?\d+)?[fl]?\b/i,
            'operator': /[+\-*\/=<>!&|^%]+/,
            'punctuation': /[{}[\];(),.:]/
        };

        Prism.languages.gpul = Prism.languages.gpulang;
    }
}

// Initialize syntax highlighting when DOM is ready
function initGPULangSyntax() {
    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', loadSyntaxData);
    } else {
        loadSyntaxData();
    }
}

// Export functions for external use
if (typeof module !== 'undefined' && module.exports) {
    module.exports = {
        loadSyntaxData,
        createGPULangPrismLanguage,
        createBasicGPULangPrismLanguage,
        initGPULangSyntax,
        getSyntaxData: () => syntaxData
    };
}

// Auto-initialize if script is loaded directly
if (typeof window !== 'undefined') {
    initGPULangSyntax();
}
