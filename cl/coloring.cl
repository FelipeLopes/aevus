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
        float r = vibrancy*density[i].x;
        float g = vibrancy*density[i].y;
        float b = vibrancy*density[i].z;
        switch (mode) {
            case ALPHA: {
                a = min(a, 1.0f);
                r*=a;
                g*=a;
                b*=a;
                break;
            }
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
        r += (1.0f-vibrancy)*pow(density[i].w*density[i].x, invGamma);
        g += (1.0f-vibrancy)*pow(density[i].w*density[i].y, invGamma);
        b += (1.0f-vibrancy)*pow(density[i].w*density[i].z, invGamma);
        a = min(a, 1.0f);
        r += bg.x*(1-a);
        g += bg.y*(1-a);
        b += bg.z*(1-a);
        r = min(r, 1.0f);
        g = min(g, 1.0f);
        b = min(b, 1.0f);
        img[3*i] = r*255;
        img[3*i+1] = g*255;
        img[3*i+2] = b*255;
    }
}