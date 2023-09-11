__kernel void mapping(float a, float b, __global float4* hist) {
    int i = get_global_id(0);
    float d = hist[i].w;
    if (d > 0) {
        hist[i].x /= d;
        hist[i].y /= d;
        hist[i].z /= d;
        hist[i].w = a*log(1+b*d);
    }
}