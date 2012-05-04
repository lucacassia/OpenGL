#ifndef DISTRIBUTION
#define DISTRIBUTION

#include <stdlib.h>
#include <string.h>
#include "complex_t.h"

#define WAVE
#define NEUMANN

typedef struct{
    int width,height,size;
    complex_t *pixels;
    complex_t *old;
    complex_t *v;
    complex_t *source;
}distribution;

distribution *distribution_alloc(int w, int h)
{
    distribution *obj = (distribution*)malloc(sizeof(distribution));
    obj->width = w;
    obj->height = h;
    obj->size = w*h;
    obj->pixels = (complex_t*)malloc(w*h*sizeof(complex_t));
    obj->v = (complex_t*)malloc(w*h*sizeof(complex_t));
    obj->source = (complex_t*)malloc(w*h*sizeof(complex_t));
    obj->old = (complex_t*)malloc(w*h*sizeof(complex_t));
    int i; for(i = 0; i < w*h; i++)
        obj->source[i].re = obj->source[i].im = obj->v[i].re = obj->v[i].im = obj->old[i].re = obj->old[i].im = obj->pixels[i].re = obj->pixels[i].im = 0;
    return obj;
}

void distribution_free(distribution *obj)
{
    free(obj->pixels);
    free(obj->v);
    free(obj->source);
    free(obj->old);
    free(obj);
}

complex_t laplacian(distribution *obj, int n)
{
    complex_t env;
    env.re = env.im = 0;
    if(n%obj->width+1 < obj->width){
        _complex_add(env, env, obj->old[n+1]);
    }
#ifdef NEUMANN
    else{ _complex_add(env, env, obj->old[n]);}
#endif
    if(n%obj->width-1 >= 0){
        _complex_add(env, env, obj->old[n-1]);
    }
#ifdef NEUMANN
    else{ _complex_add(env, env, obj->old[n]);}
#endif
    if(n-obj->width >= 0){
        _complex_add(env, env, obj->old[n-obj->width]);
    }
#ifdef NEUMANN
    else{ _complex_add(env, env, obj->old[n]);}
#endif
    if(n+obj->width < obj->size){
        _complex_add(env, env, obj->old[n+obj->width]);
    }
#ifdef NEUMANN
    else{ _complex_add(env, env, obj->old[n]);}
#endif
    env.re /= 4;
    env.im /= 4;
    _complex_sub(env, env, obj->old[n]);
    return env;
}

void distribution_compute(distribution *obj)
{
    int n;
    for(n = 0; n < obj->size; n++){
        complex_t tmp;
        tmp = laplacian(obj,n);
#ifdef WAVE
        _complex_add(obj->v[n], obj->v[n], tmp);
        _complex_add(obj->v[n], obj->v[n], obj->source[n]);
        _complex_add(obj->pixels[n], obj->pixels[n], obj->v[n]);
#endif
#ifdef QUANTUM
        double swp = tmp.re;
        tmp.re = -tmp.im*1e-2;
        tmp.im = swp*1e-2;
        _complex_add(obj->pixels[n], obj->pixels[n], tmp);
        _complex_add(obj->pixels[n], obj->pixels[n], obj->source[n]);
#endif
    }
    memcpy(obj->old, obj->pixels, obj->size*sizeof(complex_t));
}

#endif
