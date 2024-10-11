GPULang
=====

GPULang is a frontend shader language meant to serve as a common shader interface for multiple backends. GPULang targets backend platforms directly, by aiming to translate its relatively low level custom frontend language to SPIR-V, WGSL and Metal. GPULang is designed from the bottom up to support certain quality of life features such as enums, aliasing, and fp16, as well as exposing certain hardware level functionality on a native level, such as device addresses (through pointers), shader stage link validation and shader pipeline composition with depth/rasterization/blend states and shader stages combined and validated at compile time.

GPULang also offers a reflection API, which allows a title to reason about the shader resource layout, allowing for a dynamic setup of desciptor sets/root signatures/bind groups/etc...

GPULang uses the following libraries (all of which are automatically downloaded through fips), and all of which follows either the BSD or GNU lesser general license:

* mcpp
* antlr3c
* spv-tools

GPULang also allows for compile time linking of shader programs, allowing outputting of shader binaries for graphics APIs that support them.

Objects are annotatable, which allows for the runtime to read information about certain symbols. This can include tooling hints, shader stage visibility, shader program labeling, etc. 

Below is an example gpulang shader file showing the syntax of the language:

```rust
uniform Albedo : *texture2D;
uniform Sampler : *sampler;
struct Camera
{
    viewProjection : f32x4x4;
    position : f32x4;
};

uniform camera : *Camera;

struct Object
{
    model : f32x4x4;
};

uniform object : *Object;

entry_point
BasicVertex(
    in Position : f32x4
    , in UV : f32x2
    , out OutUV : f32x2
) void
{
    worldPos = object.model * Position;
    clipPos = camera.viewProjection * worldPos;

    gplExportVertexCoordinates(clipPos);
    OutUV = UV;
}

enum Framebuffer : i32
{
    Color
};

entry_point
BasicPixel(
    in UV : f32x2
) void
{
    color = textureSample(Albedo, Sampler, UV);
    gplExportColor(color, Framebuffer.Color);
}

program TestProgram
{
    VertexShader = BasicVertex;
    PixelShader = BasicPixel;
};
```
