#include <stdlib.h>
#include <stdio.h>
#include "distribution.h"
#include "color.h"

int main(int argc, char *argv[])
{
    distribution* Sheet = distribution_alloc(600,600);
    int n,k;    
    double kx = 1e2, ky = 0;
    for(n = 0; n < Sheet->size; n++){
        double rx = (Sheet->width/2-n%Sheet->width), ry = (Sheet->height/2-n/Sheet->width);
        Sheet->pixels[n].re += exp(-(rx*rx+ry*ry)/10000)*cos(rx*kx+ry*ky);
        Sheet->pixels[n].im += exp(-(rx*rx+ry*ry)/10000)*sin(rx*kx+ry*ky);
    }

    int runTime = atoi(argv[1]), fps = 30;
    char *frame = (char*)malloc(3*(Sheet->size)*sizeof(char));
    for(n = 0; n < fps*runTime; n++){
        distribution_compute(Sheet);
        for(k = 0; k < Sheet->size; k++){
            rgb8U_t color = d2rgb8U(_complex_mod(Sheet->pixels[k]));
            frame[3*k+0] = color.r;
            frame[3*k+1] = color.g;
            frame[3*k+2] = color.b;
        }
        fwrite(frame, sizeof(char), 3*(Sheet->size), stdout);
    }
    free(frame);
    distribution_free(Sheet);
    return 0;
}
