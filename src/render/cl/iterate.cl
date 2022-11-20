typedef union SeedUnion {
    uint2 word;
    ulong value;
} SeedUnion;

typedef struct IterationState {
    float x, y, c;
    uchar xf;
    SeedUnion seed;
} IterationState;

__kernel void iterate(__global IterationState *state, __global ulong *output) {
    int i = get_global_id(0);
    output[i] = state[i].seed.value;
}
