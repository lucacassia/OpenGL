#ifndef DISTRIBUTION
#define DISTRIBUTION

#include <stdlib.h>
#include <string.h>
#include "ccomplex.h"

#define WAVE
#define NEUMANN

typedef struct{
    int width,height,size;
    complex *pixels;
    complex *old;
    complex *v;
    complex *source;
}distribution;

distribution *distribution_alloc(int w, int h)
{
    distribution *obj = (distribution*)malloc(sizeof(distribution));
    obj->width = w;
    obj->height = h;
    obj->size = w*h;
    obj->pixels = (complex*)malloc(w*h*sizeof(complex));
    obj->v = (complex*)malloc(w*h*sizeof(complex));
    obj->source = (complex*)malloc(w*h*sizeof(complex));
    obj->old = (complex*)malloc(w*h*sizeof(complex));
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

void distribution_compute(distribution *obj)
{
    int n;
    for(n = 0; n < obj->size; n++){
        complex env;
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

#ifdef WAVE
//        env.re *= 2;
//        env.im *= 2;
        _complex_add(env, env, obj->source[n]);
        _complex_add(obj->v[n], obj->v[n], env);
        _complex_add(obj->pixels[n], obj->pixels[n], obj->v[n]);
#endif
#ifdef QUANTUM
        double tmp = env.re;
        env.re = -env.im*1e-2;
        env.im = tmp*1e-2;
        _complex_add(env, env, obj->source[n]);
        _complex_add(obj->pixels[n], obj->pixels[n], env);
#endif
    }
    memcpy(obj->old, obj->pixels, obj->size*sizeof(complex));
}

#endif
