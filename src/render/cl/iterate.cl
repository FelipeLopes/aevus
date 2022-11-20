typedef union SeedUnion {
    uint2 word;
    ulong value;
} SeedUnion;

typedef struct IterationState {
    float x, y, c;
    uchar xf;
    SeedUnion seed;
} IterationState;

enum { MWC64X_A = 4294883355u };

inline uint mwc64x(__global SeedUnion* s)
{
	uint c = s->word.y;
    uint x = s->word.x;
    s->value = x*((ulong)MWC64X_A) + c;
	return x^c;
}

__kernel void iterate(__global IterationState *state, __global uint *output) {
    int i = get_global_id(0);
    output[i] = mwc64x(&state[i].seed);
}
