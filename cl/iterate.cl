#define BUCKET_FACTOR (1.0f-FLT_EPSILON)
#define PI (3.141592653589793f)

enum {
    MWC64X_A = 4294883355u,
    XFORM_DISTRIBUTION_GRAINS = 16384,
    XFORM_DISTRIBUTION_GRAINS_M1 = 16383,
    COLORMAP_LENGTH = 256
};

inline void atomic_add_f(global float* addr, const float val) {
    union {
        uint  u32;
        float f32;
    } next, expected, current;
    current.f32 = *addr;
    do {
        next.f32 = (expected.f32=current.f32)+val;
        current.u32 = atomic_cmpxchg((volatile global uint*)addr, expected.u32, next.u32);
    } while(current.u32!=expected.u32);
}

inline void atomic_add_f4(global float4* addr, const float4 val) {
    global float* p = (global float*)addr;
    atomic_add_f(&p[0], val.x);
    atomic_add_f(&p[1], val.y);
    atomic_add_f(&p[2], val.z);
    atomic_add_f(&p[3], val.w);
}

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
    SPHERICAL = 2,
    POLAR = 5,
    HYPERBOLIC = 10,
    DIAMOND = 11,
    PDJ = 24,
    EYEFISH = 27,
    CYLINDER = 29,
    SQUARE = 43
} VariationID;

typedef struct VariationCL {
    VariationID id;
    float weight;
    int paramBegin;
} VariationCL;

typedef struct XFormCL {
    float a, b, c, d, e, f;
    float pa, pb, pc, pd, pe, pf;
    float color, colorSpeed;
    int varBegin, varEnd;
} XFormCL;

typedef struct FlameCL {
    float cx, cy, scale;
    int width, height;
} FlameCL;

inline bool badval(float2 p) {
    return (p.x - p.x != 0) || (p.y - p.y != 0);
}

inline uint mwc64x(__global SeedUnion* s) {
    uint c = s->word.y;
    uint x = s->word.x;
    s->value = x*((ulong)MWC64X_A) + c;
    return x^c;
}

inline float mwc64x01(__global SeedUnion* s) {
    return mwc64x(s) * (1.0f / 4294967296.0f);
}

inline float mwc64xn11(__global SeedUnion* s) {
    return -1.0f + (mwc64x(s) * (2.0f / 4294967296.0f));
}

inline void resetPoint(__global IterationState* state) {
    state->x = mwc64xn11(&state->seed);
    state->y = mwc64xn11(&state->seed);
}

inline float4 lookupColor(__global float4* palette, float val) {
    val = clamp(val, 0.0f, BUCKET_FACTOR);
    return palette[(int)(val*COLORMAP_LENGTH)];
}

float2 linear(float2 p) {
    return p;
}

float2 spherical(float2 p) {
    float invR2 = 1.0f/(p.x*p.x + p.y*p.y);
    float2 ans;
    ans.x = invR2*p.x;
    ans.y = invR2*p.y;
    return ans;
}

float2 polar(float2 p) {
    float2 ans;
    ans.x = atan2(p.x,p.y) / PI;
    ans.y = sqrt(p.x*p.x + p.y*p.y) - 1;
    return ans;
}

float2 hyperbolic(float2 p) {
    float2 ans;
    float a = atan2(p.x, p.y);
    float r = sqrt(p.x*p.x + p.y*p.y);
    ans.x = sin(a)/r;
    ans.y = cos(a)*r;
    return ans;
}

float2 diamond(float2 p) {
    float2 ans;
    float a = atan2(p.x, p.y);
    float r = sqrt(p.x*p.x + p.y*p.y);
    ans.x = sin(a)*cos(r);
    ans.y = cos(a)*sin(r);
    return ans;
}

float2 pdj(float2 p, __global const float* params) {
    float2 ans;
    ans.x = sin(params[0]*p.y) - cos(params[1]*p.x);
    ans.y = sin(params[2]*p.x) - cos(params[3]*p.y);
    return ans;
}

float2 eyefish(float2 p) {
    float2 ans;
    float k = 2.0f/(sqrt(p.x*p.x+p.y*p.y)+1.0f);
    ans.x = k*p.x;
    ans.y = k*p.y;
    return ans;
}

float2 cylinder(float2 p) {
    float2 ans;
    ans.x = sin(p.x);
    ans.y = p.y;
    return ans;
}

float2 square(__global SeedUnion* s) {
    float2 ans;
    ans.x = mwc64x01(s) - 0.5f;
    ans.y = mwc64x01(s) - 0.5f;
    return ans;
}

int histogramIndex(FlameCL* flame, float2 p) {
    float2 tl, prop;
    prop.x = flame->width/flame->scale;
    prop.y = flame->height/flame->scale;
    tl.x = flame->cx - prop.x/2;
    tl.y = flame->cy - prop.y/2;
    if (p.x - tl.x < 0 || p.x - tl.x > prop.x) {
        return -1;
    } else if (p.y - tl.y < 0 || p.y - tl.y > prop.y) {
        return -1;
    }
    int iPos = (p.y-tl.y)*BUCKET_FACTOR*flame->scale;
    int jPos = (p.x-tl.x)*BUCKET_FACTOR*flame->scale;
    return iPos*flame->width+jPos;
}

float2 calcXform(__global const XFormCL* xform, __global const VariationCL* vars,
    __global const float* params, int idx, float2 p, __global SeedUnion* seed)
{
    float2 t, acc, ans;
    t.x = xform[idx].a*p.x + xform[idx].b*p.y + xform[idx].c;
    t.y = xform[idx].d*p.x + xform[idx].e*p.y + xform[idx].f;
    acc.x = 0;
    acc.y = 0;
    for (int i=xform[idx].varBegin; i<xform[idx].varEnd; i++) {
        switch (vars[i].id) {
            case LINEAR: acc += vars[i].weight*linear(t); break;
            case SPHERICAL: acc += vars[i].weight*spherical(t); break;
            case POLAR: acc += vars[i].weight*polar(t); break;
            case HYPERBOLIC: acc += vars[i].weight*hyperbolic(t); break;
            case DIAMOND: acc += vars[i].weight*diamond(t); break;
            case PDJ: acc += vars[i].weight*pdj(t, params+vars[i].paramBegin); break;
            case EYEFISH: acc += vars[i].weight*eyefish(t); break;
            case CYLINDER: acc +=vars[i].weight*cylinder(t); break;
            case SQUARE: acc += vars[i].weight*square(seed); break;
            default: break;
        }
    }
    ans.x = xform[idx].pa*acc.x + xform[idx].pb*acc.y + xform[idx].pc;
    ans.y = xform[idx].pd*acc.x + xform[idx].pe*acc.y + xform[idx].pf;
    return ans;
}

float2 iterateXform(__global const XFormCL* xform, __global const VariationCL* vars,
    __global const float* params, int idx, __global IterationState* state, int finalId)
{
    float2 stateP = (float2)(state->x, state->y);
    float2 ans = calcXform(xform, vars, params, idx, stateP, &state->seed);

    state->x = ans.x;
    state->y = ans.y;
    state->xf = idx;

    float s = xform[idx].colorSpeed;
    state->c = s*xform[idx].color + (1-s)*state->c;

    return ans;
}

__kernel void iterate(
    FlameCL flameCL,
    __global IterationState *state,
    __global const XFormCL *xform,
    __global const VariationCL *vars,
    __global const float *params,
    __global uchar *xformDist,
    __global float4 *palette,
    __global float4 *hist,
    int posFinalXForm,
    float threshold,
    int iters)
{
    int i = get_global_id(0);
    for (int j=0; j<iters; j++) {
        int rand = mwc64x(&state[i].seed) & XFORM_DISTRIBUTION_GRAINS_M1;
        int xfIdx = xformDist[state[i].xf*XFORM_DISTRIBUTION_GRAINS+rand];
        float2 p = iterateXform(xform, vars, params, xfIdx, &state[i], posFinalXForm);
        if (badval(p)) {
            resetPoint(&state[i]);
        } else {
            int idx = histogramIndex(&flameCL, p);
            if (idx != -1) {
                float4 color = lookupColor(palette, state[i].c);
                if (hist[idx].w < threshold) {
                    atomic_add_f4(&hist[idx], color);
                }
            }
        }
    }
}
