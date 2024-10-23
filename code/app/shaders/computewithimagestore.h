namespace Computewithimagestore
{
struct InputImage
{
    static const uint32_t BINDING = 0;
    static const uint32_t GROUP = 0;
};

struct OutputImage
{
    static const uint32_t BINDING = 1;
    static const uint32_t GROUP = 0;
};

struct ComputeWithStore
{
    static inline const uint32_t WORKGROUP_SIZE[] = { 1, 1, 1 };
};

} // namespace Computewithimagestore
