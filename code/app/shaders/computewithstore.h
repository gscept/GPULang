namespace Computewithstore
{
struct Storage
{
    float vec[4];
    int i;
    unsigned int : 32;
    unsigned int : 32;
    unsigned int : 32;
    float vec2[4];
};

struct Output
{
    static const uint32_t BINDING = 0;
    static const uint32_t GROUP = 0;
    using STRUCT = Storage;
};

struct ComputeWithStore
{
    static inline const uint32_t WORKGROUP_SIZE[] = { 1, 1, 1 };
};

} // namespace Computewithstore
