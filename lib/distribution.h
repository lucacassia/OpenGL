#ifndef DISTRIBUTION
#define DISTRIBUTION

#include <stdlib.h>
#include <string.h>
#include "complex_t.h"

#define WAVE

typedef struct{
    int width,height,size;
    complex_t *psi;
    complex_t *phi;
    complex_t *source;
}distribution;

distribution *distribution_alloc(int w, int h)
{
    distribution *obj = (distribution*)malloc(sizeof(distribution));
    obj->width = w;
    obj->height = h;
    obj->size = w*h;
    obj->psi = (complex_t*)malloc(w*h*sizeof(complex_t));
    obj->phi = (complex_t*)malloc(w*h*sizeof(complex_t));
    obj->source = (complex_t*)malloc(w*h*sizeof(complex_t));
    int i; for(i = 0; i < w*h; i++)
        obj->source[i] = obj->phi[i] = obj->psi[i] = COMPLEX_ZERO;
    return obj;
}

void distribution_free(distribution *obj)
{
    free(obj->psi);
    free(obj->phi);
    free(obj->source);
    free(obj);
}

complex_t laplacian(complex_t *matrix, int w, int h, int n)
{
    complex_t env = COMPLEX_ZERO;
    _complex_add(env, env, matrix[(n/w)*w+(n%w+1)%w]);
    _complex_add(env, env, matrix[(n/w)*w+(n%w+w-1)%w]);
    _complex_add(env, env, matrix[((n/w+1)%h)*w+n%w]);
    _complex_add(env, env, matrix[((n/w+h-1)%h)*w+n%w]);
    env.re /= 4;
    env.im /= 4;
    _complex_sub(env, env, matrix[n]);
    return env;
}

complex_t hamiltonian(complex_t *matrix, int w, int h, int k)
{
    complex_t env = COMPLEX_ZERO;
    _complex_add(env, env, matrix[(k/w)*w+(k%w+1)%w]);
    _complex_add(env, env, matrix[(k/w)*w+(k%w+w-1)%w]);
    _complex_add(env, env, matrix[((k/w+1)%h)*w+k%w]);
    _complex_add(env, env, matrix[((k/w+h-1)%h)*w+k%w]);
    env.re = (env.re/4 - matrix[k].re) + (1e-8)*((k/w-h/2)*(k/w-h/2)+(k%w-w/2)*(k%w-w/2)) * matrix[k].re;
    env.im = (env.im/4 - matrix[k].im) + (1e-8)*((k/w-h/2)*(k/w-h/2)+(k%w-w/2)*(k%w-w/2)) * matrix[k].im;
    return env;
}

void distribution_compute(distribution *obj)
{
    int i,n;
    n = obj->size;
    complex_t tmp;
    complex_t *x = malloc(n*sizeof(complex_t));
    memcpy(x, obj->psi, n*sizeof(complex_t));
#ifdef WAVE
    for(i = 0; i < n; i++){
        tmp = laplacian(obj->psi,obj->width,obj->height,i);
        obj->phi[i].re += tmp.re + obj->source[i].re;
        obj->phi[i].im += tmp.im + obj->source[i].im;
        x[i].re += obj->phi[i].re;
        x[i].im += obj->phi[i].im;
    }
#endif
#ifdef QUANTUM
    complex_t *A = malloc(n*sizeof(complex_t));
    complex_t *b = malloc(n*sizeof(complex_t));
    complex_t *L = malloc(n*sizeof(complex_t));
    complex_t *L2 = malloc(n*sizeof(complex_t));
    complex_t *r = malloc(n*sizeof(complex_t));
    complex_t *p = malloc(n*sizeof(complex_t));
    for(i = 0; i < n; i++){
        tmp = laplacian(obj->psi,obj->width,obj->height,i);
        _complex_mul(L[i],obj->source[i],obj->psi[i]);
        _complex_add(L[i],L[i],tmp);
    }
    for(i = 0; i < n; i++){
        tmp = laplacian(L,obj->width,obj->height,i);
        _complex_mul(L2[i],obj->source[i],obj->psi[i]);
        _complex_add(L2[i],L2[i],tmp);
    }
    for(i = 0; i < n; i++){
        b[i].re = obj->psi[i].re + L[i].im*(1e-2)/2 + L2[i].re*(1e-2)/4;
        b[i].re = obj->psi[i].im - L[i].re*(1e-2)/2 + L2[i].im*(1e-2)/4;
    }
    for(i = 0; i < n; i++){
        L[i] = laplacian(x,obj->width,obj->height,i);
    }
    for(i = 0; i < n; i++){
        L2[i] = laplacian(L,obj->width,obj->height,i);
    }
    for(i = 0; i < n; i++){
        A[i].re = x[i].re + L2[i].re*(1e-2)/4;
        A[i].re = x[i].im + L2[i].im*(1e-2)/4;
    }
    for(i = 0; i < n; i++){
        r[i].re = b[i].re - A[i].re;
        r[i].im = b[i].im - A[i].im;
        p[i] = r[i];
    }

    while(mod(r,n) > 1e-8){
        complex_t alpha,beta,r2;
        r2 = scalardot(r,r,n);
        for(i = 0; i < n; i++){
            tmp = laplacian(p,obj->width,obj->height,i);
            _complex_mul(A[i],obj->source[i],p[i]);
            _complex_add(A[i],A[i],tmp);
        }
        _complex_div(alpha,r2,scalardot(A,p,n));
        for(i = 0; i < n; i++){
            _complex_mul(tmp,alpha,p[i]);
            x[i].re += tmp.re;
            x[i].im += tmp.im;
            _complex_mul(tmp,alpha,A[i]);
            r[i].re -= tmp.re;
            r[i].im -= tmp.im;
        }
        _complex_mul(beta,scalardot(r,r,n),r2);
        for(i = 0; i < n; i++){
            _complex_mul(tmp,beta,p[i]);
            p[i].re = r[i].re + tmp.re;
            p[i].im = r[i].im + tmp.im;
        }
    }
    free(A);
    free(b);
    free(L);
    free(L2);
    free(r);
    free(p);
    normalize(x,n);
#endif
    memcpy(obj->psi, x, n*sizeof(complex_t));
    free(x);
}

#endif
