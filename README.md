GPULang
=====

GPULang is a frontend shader langauge meant to serve as a common shader interface for multiple backends. GPULang targets backend platforms directly, by aiming to translate the relatively low level frontend langauge to SPIR-V, DXIL, WGSL and Metal. 

GPULang also offers a reflection API, which allows a title to reason about the shader resource layout, allowing for a dynamic setup of desciptor sets/root signatures/bind groups/etc...

GPULang uses the following libraries (all of which are included), all of which follows either the BSD or GNU lesser general license:

* mcpp
* antlr3c

GPULang also allows for compile time linking of shader programs, allowing outputting of shader binaries for graphics APIs that support them.

Objects are annotatable, which allows for the runtime to read information about certain bindings at load time. This can include tooling hints, shader stage visibility, shader program labeling, etc. 

Below is a text clump showing the syntax of the language:

```c
//------------------------------------------------------------------------------
//  @file completeshader.fx
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------

const int arr[5][2] = { {1,1},{2,2},{3,3},{4,4},{5,5} };
const float4 ARRAY_VEC4[] = { float4(1) };
struct MyConstantBuffer
{
    int NumWorkGroups;
    float2 ElementPaddedArray[10];
};

@Visibility("CS")
uniform MyConstantBuffer* myConstantBuffer;

alias textureHandle as int;
const textureHandle foobar = 5;

const int foo = 5;
const int ARRAY_INIT[][] = { {1}, {foo}, {3} };

read rgba32f readWriteTexture2D* myReadImage;
write rgb10_a2 readWriteTexture2D* myWriteImage;
binding(0) rg16f read_write readWriteTexture2D* myReadWriteImage;
volatile r8u readWriteTexture2D* myVolatileImage[5];
atomic r32i readWriteTexture2D* myAtomicImage[];

@Visibility("CS")
group(0) sampler* MyDynamicSampler;

struct MyStruct
{
    int i;  
    textureHandle tex;
};

struct MyStorageBuffer
{
    int ProvokePadding;
    float3 Output;  
};
mutable MyStorageBuffer* myStorageBuffer;

const int NUM_FOO = 5;
const float4 NUM_BLORF[2] = { float4(1,1,1,1), float4(2,2,2,2) };

// sampler_states gets converted to ordinary sampler objects, 
// but is supposed to be read on the receiving side
sampler_state MyImmutableSampler
{
    AddressU = Wrap;
    AddressV = Clamp;
    AddressW = Mirror;
    Filter = Linear;
    AnisotropicEnabled = true;
    MaxAnisotropy = 16;
};

render_state MyRenderState
{
    FrontStencil.ReferenceMask = 0xFF;
    BlendEnabled[0] = true;
    DepthTestFunction = Greater;
    DepthWriteEnabled = false;
};

//------------------------------------------------------------------------------
/**
    Test statements
*/
int
MegaFunction()
{
    int foo = 5;
    {
        int foo = 5;
    }

    const int arr[5][2] = { {1,1},{2,2},{3,3},{4,4},{5,5} };

    const float4 vecarray[5] = { float4(1), float4(2), float4(3), float4(4), float4(5) };

    float3 foobar = vecarray[1].xyz;

    if (foo > 0)
    {
        int bar = 5;
        if (foo != 0)
            return bar;
        else
            return bar;
    }
    else
    {
        int bar = 5;
    }

    while (true)
    {
        if (foo == 0)
            break;
        foo--;
    }

    foo = 5;
    do 
    {
        foo--;
    } while (foo > 0);

    foo = 6;
    for (int i = 0, j = 0; i < foo; i++, ++j)
    {
        if ((i % 2) == 0)
            continue;
    }

    return foo == 6 ? 1 : 0;
}

//------------------------------------------------------------------------------
/**
*/
float4 
Function(in float f)
{
    return float4(f);
}

//------------------------------------------------------------------------------
/**
    Overload function
*/
float4
Function(in int i)
{
    return float4(i);
}

//------------------------------------------------------------------------------
/**
    Per-program bind function
*/
prototype float4 DynamicFunction(in int i);

//------------------------------------------------------------------------------
/**
*/
void
MyComputeShader()
{
    textureStore(myReadWriteImage, int2(0, 0), Function(myConstantBuffer->NumWorkGroups));
}

//------------------------------------------------------------------------------
/**
*/
@Mask("MyProgram")
program MyProgram
{
    ComputeShader = MyComputeShader;
};

//------------------------------------------------------------------------------
/**
*/
void
MyVertexShader(
    binding(0) in float4 position
    , binding(0) out float4 Position
)
{
    Position = position;
}

//------------------------------------------------------------------------------
/**
*/
void
MyPixelShader(
    binding(0) in float4 position
    , binding(0) out float4 Color
)
{
    Color = position;
}

//------------------------------------------------------------------------------
/**
*/
@Mask("MyGraphicsProgram")
program MyGraphicsProgram
{
    DynamicFunction = Function;
    VertexShader = MyVertexShader;
    PixelShader = MyPixelShader;
};

```
