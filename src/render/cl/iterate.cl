#define BUCKET_FACTOR (1.0f-FLT_EPSILON)

enum {
    MAX_VARIATIONS = 10,
    MWC64X_A = 4294883355u,
    XFORM_DISTRIBUTION_GRAINS = 16384,
    XFORM_DISTRIBUTION_GRAINS_M1 = 16383
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

inline int histogramIndex(FlameCL* flame, float2 p) {
    float2 tl, prop;
    prop.x = flame->width/flame->scale;
    prop.y = flame->height/flame->scale;
    tl.x = flame->cx - prop.x/2;
    tl.y = flame->cy + prop.y/2;
    if (p.x - tl.x < 0 || p.x - tl.x > prop.x) {
        return -1;
    } else if (tl.y - p.y < 0 || tl.y - p.y > prop.y) {
        return -1;
    }
    int iPos = (tl.y-p.y)*BUCKET_FACTOR*flame->scale;
    int jPos = (p.x-tl.x)*BUCKET_FACTOR*flame->scale;
    return iPos*flame->width+jPos;
}

inline float2 calcXform(__global const XFormCL* xform, int idx, __global IterationState* state) {
    float2 t, acc, ans;
    t.x = xform[idx].a*state->x + xform[idx].b*state->y + xform[idx].c;
    t.y = xform[idx].d*state->x + xform[idx].e*state->y + xform[idx].f;
    acc.x = 0;
    acc.y = 0;
    int i = 0;
    while (i < MAX_VARIATIONS && xform[idx].varData[i].id != NO_VARIATION) {
        switch (xform[idx].varData[i].id) {
            case LINEAR: acc += xform[idx].varData[i].weight*linear(t); break;
            case SPHERICAL: acc += xform[idx].varData[i].weight*spherical(t); break;
            default: break;
        }
        i++;
    }
    ans.x = xform[idx].pa*acc.x + xform[idx].pb*acc.y + xform[idx].pc;
    ans.y = xform[idx].pd*acc.x + xform[idx].pe*acc.y + xform[idx].pf;
    state->x = ans.x;
    state->y = ans.y;
    state->xf = idx;
    return ans;
}

__kernel void iterate(
    FlameCL flameCL,
    __global IterationState *state,
    __global const XFormCL *xform,
    __global uchar *xformDist,
    __global float4 *palette,
    __global int *output)
{
    int i = get_global_id(0);
    int rand = mwc64x(&state[i].seed) & XFORM_DISTRIBUTION_GRAINS_M1;
    int xfIdx = xformDist[state[i].xf*XFORM_DISTRIBUTION_GRAINS+rand];
    float2 p = calcXform(xform, xfIdx, &state[i]);
    output[i] = histogramIndex(&flameCL, p);
}
