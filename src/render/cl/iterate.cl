enum {
    MAX_VARIATIONS = 10,
    MWC64X_A = 4294883355u,
    XFORM_DISTRIBUTION_GRAINS = 16384
};

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

typedef struct FlameCL {
    float cx, cy, scale;
    int width, height;
} FlameCL;

inline uint mwc64x(__global SeedUnion* s)
{
	uint c = s->word.y;
    uint x = s->word.x;
    s->value = x*((ulong)MWC64X_A) + c;
	return x^c;
}

inline float zeps(float f) {
    return f == 0.0f ? FLT_EPSILON : f;
}

inline float2 linear(float2 p) {
    return p;
}

inline float2 spherical(float2 p) {
    float invR2 = 1.0f/zeps(p.x*p.x + p.y*p.y);
    float2 ans;
    ans.x = invR2*p.x;
    ans.y = invR2*p.y;
    return ans;
}

inline float2 calcXform(__global const XFormCL* xform, __global IterationState* state) {
    float2 t, acc, ans;
    t.x = xform->a*state->x + xform->b*state->y + xform->c;
    t.y = xform->d*state->x + xform->e*state->y + xform->f;
    acc.x = 0;
    acc.y = 0;
    int i = 0;
    while (i < MAX_VARIATIONS && xform->varData[i].id != NO_VARIATION) {
        switch (xform->varData[i].id) {
            case LINEAR: acc += xform->varData[i].weight*linear(t); break;
            case SPHERICAL: acc += xform->varData[i].weight*spherical(t); break;
            default: break;
        }
        i++;
    }
    ans.x = xform->pa*acc.x + xform->pb*acc.y + xform->pc;
    ans.y = xform->pd*acc.x + xform->pe*acc.y + xform->pf;
    state->x = ans.x;
    state->y = ans.y;
    return ans;
}

__kernel void iterate(
    FlameCL flameCL,
    __global IterationState *state,
    __global const XFormCL *xform,
    __global uchar *xformDist,
    __global float4 *palette,
    __global float2 *output)
{
    int i = get_global_id(0);
    float2 ans = calcXform(&xform[2], &state[i]);
    ans.x = palette[127].z;
    output[i] = ans;
}
