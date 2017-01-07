#include"../lib/complex_t.h"
#include <stdlib.h>

/* modes Norm = 0
 *       Conj = 1
 *        Abs = 2
 *       Iabs = 3
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

unsigned int innerLoop(int i, int j)
{
    unsigned int k;
    complex_t z, tmp;

    z.re = focus.re + ( j + ( 1 - width ) / 2 ) / zoom;
    z.im = focus.im + ( i + ( 1 - height) / 2 ) / zoom;

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
        {
            z.re = z.re;
            z.im = fabs(z.im);
        }

    /*  z = c + z ^ jexp */

        tmp.re = pow( _complex_mod(z), jexp );
        tmp.im = _complex_arg(z) * jexp;

        z.re = c.re + tmp.re * cos( tmp.im );
        z.im = c.im + tmp.re * sin( tmp.im );
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
    focus.re = -c.re;
    focus.im =  c.im;
    zoom = 256;
    shift = 0.5;
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
    c.re = 1.0;
    c.im = 0.3;
    resetView();
    divergence = 5;
    jexp = 6;
    halt = 100;
    mode = 1;
    updateSize(600,600);
}

void zoomIn()
{
    zoom *= 2;
    shift /= 2;
    compute();
}

void zoomOut()
{
    zoom /= 2;
    shift *= 2;
    compute();
}

void moveLeft()
{
    focus.re -= shift;
    compute();
}

void moveRight()
{
    focus.re += shift;
    compute();
}

void moveUp()
{
    focus.im += shift;
    compute();
}

void moveDown()
{
    focus.im -= shift;
    compute();
}

void a()
{
    c.re /= 1.01;
    compute();
}

void d()
{
    c.re *= 1.01;
    compute();
}

void w()
{
    c.im *= 1.01;
    compute();
}

void s()
{
    c.im /= 1.01;
    compute();
}

