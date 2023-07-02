__kernel void mapping(float a, float b, __global float4* hist) {
    int i = get_global_id(0);
    float4 color = (1);
    float x = hist[i].w;
    color *= a*log(1+b*x);
    hist[i] = color;
}