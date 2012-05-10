#include <stdlib.h>
#include <stdio.h>
#include "distribution.h"
#include "color.h"

int main(int argc, char *argv[])
{
    distribution* pixels = distribution_alloc(600,600);
    int n,k;    
    double kx = 1e2, ky = 0;
    for(n = 0; n < pixels->size; n++){
        double rx = (pixels->width/2-n%pixels->width), ry = (pixels->height/2-n/pixels->width);
        pixels->psi[n].re += exp(-(rx*rx+ry*ry)/10000)*cos(rx*kx+ry*ky);
        pixels->psi[n].im += exp(-(rx*rx+ry*ry)/10000)*sin(rx*kx+ry*ky);
    }
    kx = 0; ky = 1e2;
    for(n = 0; n < pixels->size; n++){
        double rx = (pixels->width/2-n%pixels->width), ry = (pixels->height/2-n/pixels->width);
        pixels->psi[n].re += exp(-(rx*rx+ry*ry)/10000)*cos(rx*kx+ry*ky);
        pixels->psi[n].im += exp(-(rx*rx+ry*ry)/10000)*sin(rx*kx+ry*ky);
    }

    int runTime = atoi(argv[1]);
    char *frame = (char*)malloc(3*(pixels->size)*sizeof(char));
    for(n = 0; n < runTime; n++){
        distribution_compute(pixels);
        for(k = 0; k < pixels->size; k++){
            rgb8U_t color = d2rgb8U(_complex_mod(pixels->psi[k]));
            frame[3*k+0] = color.r;
            frame[3*k+1] = color.g;
            frame[3*k+2] = color.b;
        }
        fwrite(frame, sizeof(char), 3*(pixels->size), stdout);
    }
    free(frame);
    distribution_free(pixels);
    return 0;
}
