#ifndef DISTRIBUTION
#define DISTRIBUTION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complex_t.h"

//#define WAVE
#define GLOBAL

#ifdef GLOBAL
double dt = (1);
double mass = 1;
double omega  = 1;
double hbar = 1;
double potential(double x,double y){ return (mass*omega*omega*((x)*(x)+(y)*(y))/2.0); }
#else
#define dt (1e-9)
#define mass 1.0
#define omega 1.0
#define hbar 1.0
#define potential(x,y) (mass*omega*omega*((x)*(x)+(y)*(y))/2.0)
#endif

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
    obj->size = w * h;
    obj->psi = (complex_t*)malloc(w * h * sizeof(complex_t));
    obj->phi = (complex_t*)malloc(w * h * sizeof(complex_t));
    obj->source = (complex_t*)malloc(w * h * sizeof(complex_t));
    int i; for(i = 0; i < w * h; i++)
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

complex_t hamiltonian(complex_t *matrix, int w, int h, int k)
{
    complex_t H = COMPLEX_ZERO;
    _complex_add(H, H, matrix[(k/w)*w+(k%w+1)%w]);
    _complex_add(H, H, matrix[(k/w)*w+(k%w+w-1)%w]);
    _complex_add(H, H, matrix[((k/w+1)%h)*w+k%w]);
    _complex_add(H, H, matrix[((k/w+h-1)%h)*w+k%w]);
    H.re = -hbar*hbar*(H.re - 4*matrix[k].re)/(2*mass) + potential((k/w)*2.0/h-1, (k%w)*2/w-1) * matrix[k].re;
    H.im = -hbar*hbar*(H.im - 4*matrix[k].im)/(2*mass) + potential((k/w)*2.0/h-1, (k%w)*2/w-1) * matrix[k].im;
    return H;
}

void evaluateM(complex_t *M, complex_t *x, int w, int h)
{
    int i;
    complex_t *A = malloc(w * h * sizeof(complex_t));
    complex_t *A2 = malloc(w * h * sizeof(complex_t));
    for(i = 0; i < w * h; i++)
        A[i] = hamiltonian(x,w,h,i);
    for(i = 0; i < w * h; i++)
        A2[i] = hamiltonian(A,w,h,i);
    for(i = 0; i < w * h; i++){
        M[i].re = x[i].re + A2[i].re * dt * dt / 4 / hbar / hbar;
        M[i].im = x[i].im + A2[i].im * dt * dt / 4 / hbar / hbar;
    }
    free(A);
    free(A2);
}

void distribution_compute(distribution *obj)
{
    int i, n = obj->size, w = obj->width, h = obj->height;
#ifdef WAVE
    complex_t tmp, *x = malloc(n * sizeof(complex_t));
    memcpy(x, obj->psi, n * sizeof(complex_t));
    for(i = 0; i < n; i++){
        tmp = laplacian2d(obj->psi,w,h,i);
        obj->phi[i].re += tmp.re + obj->source[i].re;
        obj->phi[i].im += tmp.im + obj->source[i].im;
        _complex_add(x[i], x[i], obj->phi[i]);
    }
    memcpy(obj->psi, x, n * sizeof(complex_t));
    free(x);
#else
    normalize(obj->psi,n);
    complex_t *M = malloc(n * sizeof(complex_t));
    complex_t *A = malloc(n * sizeof(complex_t));
    complex_t *A2 = malloc(n * sizeof(complex_t));
    complex_t *r = malloc(n * sizeof(complex_t));
    complex_t *p = malloc(n * sizeof(complex_t));
    for(i = 0; i < n; i++)
        A[i] = hamiltonian(obj->psi, w, h, i);
    for(i = 0; i < n; i++)
        A2[i] = hamiltonian(A, w, h, i);
    for(i = 0; i < n; i++){
        M[i].re = obj->psi[i].re + A2[i].re * dt * dt / 4 / hbar / hbar;
        M[i].im = obj->psi[i].im + A2[i].im * dt * dt / 4 / hbar / hbar;
    }
    for(i = 0; i < n; i++){
        r[i].re = obj->psi[i].re + A[i].im * dt / hbar - A2[i].re * dt * dt / 4 / hbar / hbar;
        r[i].im = obj->psi[i].im - A[i].re * dt / hbar - A2[i].im * dt * dt / 4 / hbar / hbar;
    }
    double b = norm(r,n);
    printf("\n norm(b) = %14.10e\n",b);

    for(i = 0; i < n; i++){
        _complex_sub(r[i], r[i], M[i]);
        p[i] = r[i];
    }

    double rs = norm(r,n);
    printf("\n rsold = %14.10e\n",rs);
    while(1){
        static complex_t alpha, beta, rAr, tmp;
        evaluateM(A, r, w, h);
        rAr = scalar(r,A,n);
        evaluateM(M, p, w, h);
        tmp = scalar(M,M,n);
        _complex_div(alpha, rAr, tmp);
        for(i = 0; i < n; i++){
            _complex_mul(tmp, alpha, p[i]);
            _complex_add(obj->psi[i], obj->psi[i], tmp);
            _complex_mul(tmp, alpha, M[i]);
            _complex_sub(r[i], r[i], tmp);
        }
        rs = norm(r,n);
        printf(" rsnew = %14.10e\n",rs);
        if(rs/b < 1e-6) break;
        evaluateM(A2, r, w, h);
        tmp = scalar(r,A2,n);
        _complex_div(beta, tmp, rAr);
        for(i = 0; i < n; i++){
            _complex_mul(p[i], p[i], beta);
            _complex_add(p[i], p[i], r[i]);
            _complex_mul(M[i], beta, M[i]);
            _complex_add(M[i], M[i], A2[i]);
        }
    }
    free(M);
    free(A);
    free(A2);
    free(r);
    free(p);
#endif
}

#endif
