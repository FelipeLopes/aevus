__kernel void vector_add(__global const int *a, __global const int *b, __global int *c,
    __global uchar* debug, __global int *debugSize) {
    int i = get_global_id(0);
    int j = get_local_id(0);
    c[i] = a[i] + b[i];
}
