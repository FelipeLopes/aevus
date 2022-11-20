#define MAX_VARIATIONS 10

typedef union SeedUnion {
    uint2 word;
    ulong value;
} SeedUnion;

typedef struct IterationState {
    float x, y, c;
    uchar xf;
    SeedUnion seed;
} IterationState;

typedef enum VariationID {
    NO_VARIATION = -1,
    LINEAR = 0,
    SPHERICAL = 2
} VariationID;

typedef struct VariationData {
    VariationID id;
    float weight;
} VariationData;

typedef struct XFormCL {
    VariationData varData[MAX_VARIATIONS];
    float a, b, c, d, e, f;
    float pa, pb, pc, pd, pe, pf;
} XFormCL;

enum { MWC64X_A = 4294883355u };

inline uint mwc64x(__global SeedUnion* s)
{
	uint c = s->word.y;
    uint x = s->word.x;
    s->value = x*((ulong)MWC64X_A) + c;
	return x^c;
}

inline float2 calcXform(__global XFormCL* xform, float2 p) {
    return p;
}

__kernel void iterate(
    __global IterationState *state,
    __global XFormCL *xform,
    __global float *output)
{
    int i = get_global_id(0);
    output[i] = xform->pc;
}
