namespace Computewithstore
{
struct Storage
{
    static const uint32_t SIZE = 4;
    static const uint32_t i_OFFSET = 0;

    int i;
};

struct MutableBuffer_Output
{
    static const uint32_t SIZE = 4;
    static const uint32_t i_OFFSET = 0;

    int i;
};

struct Output
{
    static const uint32_t BINDING = 0;
    static const uint32_t GROUP = 0;
    using STRUCT = MutableBuffer_Output;
};

struct ComputeWithStore
{
    static inline const uint32_t WORKGROUP_SIZE[] = { 1, 1, 1 };
};

} // namespace Computewithstore
