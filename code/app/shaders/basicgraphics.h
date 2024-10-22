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
    static const uint32_t SIZE = 84;

    float viewProjection[4][4];
    int foo;
    float position[4];
};

struct UniformBuffer_camera
{
    static const uint32_t SIZE = 96;
    static const uint32_t BINDING = 3;
    static const uint32_t GROUP = 0;

    float viewProjection[4][4];
    int foo;
    unsigned int : 12;
    float position[4];
};

struct Object
{
    static const uint32_t SIZE = 64;

    float model[4][4];
};

struct UniformBuffer_object
{
    static const uint32_t SIZE = 64;
    static const uint32_t BINDING = 4;
    static const uint32_t GROUP = 0;

    float model[4][4];
};

} // namespace Basicgraphics
