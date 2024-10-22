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
    static const uint32_t SIZE = 80;
    static const uint32_t viewProjection_OFFSET = 0;
    static const uint32_t position_OFFSET = 64;

    float viewProjection[4][4];
    float position[4];
};

struct UniformBuffer_camera
{
    static const uint32_t SIZE = 80;
    static const uint32_t viewProjection_OFFSET = 0;
    static const uint32_t position_OFFSET = 64;

    float viewProjection[4][4];
    float position[4];
};

struct camera
{
    static const uint32_t BINDING = 3;
    static const uint32_t GROUP = 0;
    using STRUCT = UniformBuffer_camera;
};

struct Object
{
    static const uint32_t SIZE = 64;
    static const uint32_t model_OFFSET = 0;

    float model[4][4];
};

struct UniformBuffer_object
{
    static const uint32_t SIZE = 64;
    static const uint32_t model_OFFSET = 0;

    float model[4][4];
};

struct object
{
    static const uint32_t BINDING = 4;
    static const uint32_t GROUP = 0;
    using STRUCT = UniformBuffer_object;
};

} // namespace Basicgraphics
