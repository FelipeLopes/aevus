__kernel void mapping(float a, float b, __global float4* hist) {
    int i = get_global_id(0);
    float x = hist[i].w;
    if (x > 0) {
        hist[i] *= (log(1+b*x)/x);
    }
}