/*
 * PROBABILITY DISTRIBUTIONS AND OTHER NUMBER GENS
 */

#ifndef OBH_NOISE_COLLECTION
#define OBH_NOISE_COLLECTION

#ifndef STB_PERLIN_IMPLEMENTATION
#define STB_PERLIN_IMPLEMENTATION
#include "../external/stb/stb_perlin.h"
#endif

/* Function to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0]
 */
float 
interpolate(float a0, float a1, float w)
{
    /* // You may want clamping by inserting:
     * if (0.0 > w) return a0;
     * if (1.0 < w) return a1;
     */
    return (a1 - a0) * w + a0;
    /* // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
     * return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
     *
     * // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
     * return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
     */
}

double 
randClamped()
{
    return (double)rand() / (double)RAND_MAX;
}

int
randGaussian(int mean, int var)
{
    double r1 = randClamped();
    double r2 = randClamped();
    double gaussian_double = sqrt(-2 * log(r1)) * cos(2 * M_PI * r2);
    // printf("%f %f: %f, ", r1, r2, gaussian_double);
    return (int) (((double)mean + gaussian_double) * (double)var);
}

/* Create pseudorandom direction vector
 */
void 
randomGradient(vec2 dest, int ix, int iy)
{
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; // rotation width
    unsigned a = ix, b = iy;
    a *= 3284157443; b ^= a << s | a >> w-s;
    b *= 1911520717; a ^= b << s | b >> w-s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    dest[0] = cos(random); dest[1] = sin(random);
}

// Computes the dot product of the distance and gradient vectors.
float 
dotGridGradient(int ix, int iy, float x, float y)
{
    // Get gradient from integer coordinates
    vec2 gradient;
    randomGradient(gradient, ix, iy);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx*gradient[0] + dy*gradient[1]);
}

// Compute Perlin noise at coordinates x, y
float 
perlin(float x, float y)
{
    // Determine grid cell coordinates
    int x0 = (int)floor(x);
    int x1 = x0 + 1;
    int y0 = (int)floor(y);
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
}

#endif
