#include "complex_t.h"
#include <stdlib.h>

/* Modes:
 *   Norm = 0
 *   Conj = 1
 *    Abs = 2
 *   Iabs = 3
 */

    complex_t c;
    complex_t focus;
    double zoom;
    double shift;
    double divergence;
    double jexp;
    int halt;
    int mode;
    int width;
    int height;
    unsigned int* pixels;
    float colorPhase[3];
    float intensity;

unsigned int innerLoop(int i, int j)
{
    unsigned int k;
    complex_t z, c1, tmp;

    z.re = 0.0;
    z.im = 0.0;

    c1.re = focus.re + ( j + ( 1 - width ) / 2 ) / zoom;
    c1.im = focus.im + ( i + ( 1 - height) / 2 ) / zoom;

    for ( k = 0; ( k < halt ) * ( _complex_mod(z) < divergence ); k++ )
    {
        if ( mode == 1 )
            z.im = -z.im;
        if ( mode == 2 )
        {
            z.re = fabs(z.re);
            z.im = fabs(z.im);
        }
        if ( mode == 3 )
            z.im = fabs(z.im);

/*
 *      z = c + z ^ jexp
 */

        tmp.re = pow( _complex_mod(z), jexp );
        tmp.im = _complex_arg(z) * jexp;

        z.re = c1.re + tmp.re * cos( tmp.im );
        z.im = c1.im + tmp.re * sin( tmp.im );
/*
        tmp.re = z.re;
        tmp.im = z.im;

        z.re = c.re * exp(tmp.re)*cos(tmp.im) - c.im * exp(tmp.re)*sin(tmp.im);
        z.im = c.re * exp(tmp.re)*sin(tmp.im) + c.im * exp(tmp.re)*cos(tmp.im);
*/
    }

    return k;
}

void compute()
{
    int n;
    #pragma omp parallel for
    for(n = 0; n < height * width; n++)
        pixels[n] = innerLoop(n/width, n%width);
}

void resetView()
{
    colorPhase[0] = 0;
    colorPhase[1] = 0;
    colorPhase[2] = 0;
    intensity = 1.0;
    focus.re = 0;
    focus.im = 0;
    zoom = 256;
    shift = 0.25;
}

void clear()
{
    if(pixels)
        free(pixels);
    pixels = NULL;
}

void updateSize(int w, int h)
{
    width = w;
    height = h;
    clear();
    pixels = (unsigned int*) malloc( width * height * sizeof(unsigned int) );
    compute();
}

void init()
{
    pixels = NULL;
    c.re = 0.0;
    c.im = 0.0;
    resetView();
    divergence = 1000000000;
    jexp = 2;
    halt = 100;
    mode = 0;
    updateSize(1024,768);
}

