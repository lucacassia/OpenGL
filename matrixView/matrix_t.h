#ifndef MATRIX_T
#define MATRIX_T

#include <stdlib.h>
#include "complex_t.h"

typedef struct{
    int width,height,size;
    complex_t focus;
    double *pixels, zoom, shift, phase[3];
}matrix_t;

void matrix_compute(matrix_t *obj)
{
    int i,j;
    for(i = 0; i < obj->height; i++)
        for(j = 0; j < obj->width; j++){
            complex_t z,tmp;
            z.re = obj->focus.re+(j+(1-obj->width)/2)/obj->zoom;
            z.im = obj->focus.im+(i+(1-obj->height)/2)/obj->zoom;
            tmp.re = 1;
            tmp.im = 0;
            _complex_div(z,tmp,z);
            _complex_cos(tmp,z);
            _complex_sin(z,z);
            _complex_div(z,z,tmp);
            obj->pixels[i*obj->width+j] = _complex_mod(z);
        }
}

void matrix_updateSize(matrix_t *obj, int w, int h)
{
    obj->width = w;
    obj->height = h;
    obj->size = w*h;
    if(obj->pixels!=NULL)
        free(obj->pixels);
    obj->pixels = (double*)malloc(w*h*sizeof(double));
    matrix_compute(obj);
}

void matrix_resetView(matrix_t *obj)
{
    obj->focus.re = obj->focus.im = 0;
    obj->zoom = 256;
    obj->shift = 0.5;
    obj->phase[0] = 0;
    obj->phase[1] = 3.14159265/3;
    obj->phase[2] = 2*3.14159265/3;
}

matrix_t *matrix_alloc(int w, int h)
{
    matrix_t *obj = (matrix_t*)malloc(sizeof(matrix_t));
    obj->pixels = NULL;
    matrix_updateSize(obj,w,h);
    matrix_resetView(obj);
    return obj;
}

void matrix_free(matrix_t *obj)
{
    free(obj->pixels);
    free(obj);
}

void matrix_pp(matrix_t *obj)
{
    obj->zoom*=2;
    obj->shift/=2;
    matrix_compute(obj);
}

void matrix_mm(matrix_t *obj)
{
    obj->zoom/=2;
    obj->shift*=2;
    matrix_compute(obj);
}

void matrix_left(matrix_t *obj)
{
    obj->focus.re -= obj->shift;
    matrix_compute(obj);
}

void matrix_right(matrix_t *obj)
{
    obj->focus.re += obj->shift;
    matrix_compute(obj);
}

void matrix_up(matrix_t *obj)
{
    obj->focus.im += obj->shift;
    matrix_compute(obj);
}

void matrix_down(matrix_t *obj)
{
    obj->focus.im -= obj->shift;
    matrix_compute(obj);
}

#endif
