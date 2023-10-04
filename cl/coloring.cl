typedef enum ClippingMode {
    ALPHA,
    CHANNEL,
    WHITE
} ClippingMode;

__kernel void coloring(__global float4* density, float4 bg,  float invGamma, float vibrancy,
    ClippingMode mode, int chunk, __global uchar* img)
{
    int idx = get_global_id(0)*chunk;
    for (int i=idx; i<idx+chunk; i++) {
        float a = pow(density[i].w, invGamma);
        float r = vibrancy*density[i].x+(1-vibrancy) *
            min(1.0f, pow(density[i].x*density[i].w, invGamma));
        float g = vibrancy*density[i].y+(1-vibrancy) *
            min(1.0f, pow(density[i].y*density[i].w, invGamma));
        float b = vibrancy*density[i].z+(1-vibrancy) *
            min(1.0f, pow(density[i].z*density[i].w, invGamma));
        if (a <= 1.0f) {
            r = r*a + bg.x*(1-a);
            g = g*a + bg.y*(1-a);
            b = b*a + bg.z*(1-a);
        }
        switch (mode) {
            case CHANNEL: {
                a = min(a, 1/r);
                a = min(a, 1/g);
                a = min(a, 1/b);
                r*=a;
                g*=a;
                b*=a;
                break;
            }
            case WHITE: {
                r = min(r*a, 1.0f);
                g = min(g*a, 1.0f);
                b = min(b*a, 1.0f);
                break;
            }
            default: break;
        }
        img[3*i] = r*255;
        img[3*i+1] = g*255;
        img[3*i+2] = b*255;
    }
}