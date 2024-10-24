namespace Basicgraphics
{
struct Albedo
{
    static const uint32_t BINDING = 0;
    static const uint32_t GROUP = 0;
};

struct Material
{
    static const uint32_t BINDING = 1;
    static const uint32_t GROUP = 0;
};

struct Sampler
{
    static const uint32_t BINDING = 2;
    static const uint32_t GROUP = 0;
};

struct Camera
{
    float viewProjection[4][4];
    float position[4];
};

struct camera
{
    static const uint32_t BINDING = 3;
    static const uint32_t GROUP = 0;
    using STRUCT = Camera;
};

struct Object
{
    float model[4][4];
};

struct object
{
    static const uint32_t BINDING = 4;
    static const uint32_t GROUP = 0;
    using STRUCT = Object;
};

struct BasicVertex
{
    static const uint32_t Position_BINDING = 0;
    static const uint32_t Position_OFFSET = 0;
    static const uint32_t Position_SIZE = 12;
    static const uint32_t UV_BINDING = 1;
    static const uint32_t UV_OFFSET = 12;
    static const uint32_t UV_SIZE = 8;
    static const uint32_t VERTEX_STRIDE = 20;
};

struct BasicPixel
{
};

} // namespace Basicgraphics
