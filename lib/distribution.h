#ifndef DISTRIBUTION
#define DISTRIBUTION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complex_t.h"

#define WAVE

double dt = 1.0;
double mass = 1.0;
double omega  = 1.0;
double hbar = 1.0;

double harmonic(double x,double y){ return mass*omega*omega*((x)*(x)+(y)*(y))/2.0; }

double mix(double x,double y){
    if(hypot(x,y)<0.25)
        return -0.05;
    if(hypot(x,y)<0.5)
	return 0.05;
    else
        return -mass*omega*omega*((x)*(x)+(y)*(y))/2.0;
}

double well(double x,double y){
    if(hypot(x,y)<0.1)
        return -100;
    return 0;
}

double coscos(double x,double y){return -cos(x*4*atan(1))-cos(y*4*atan(1));}

double (*potential)(double,double) = coscos;

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

void distribution_init(distribution *obj, double s, double kx, double ky)
{
    double x,y;
    int i,j;
    for(i = 0; i < obj->height; i++)
        for(j = 0; j < obj->width; j++) {
            x = i*2.0/(obj->width-1)-1;
            y = j*2.0/(obj->height-1)-1;
            obj->psi[i*obj->width+j].re = cos(kx*x+ky*y)*expf(-(pow(y-0.5,2)+pow(x-0.5,2))/(2*s*s))/sqrt(2*3.1415926)/s;
            obj->psi[i*obj->width+j].im = sin(kx*x+ky*y)*expf(-(pow(y-0.5,2)+pow(x-0.5,2))/(2*s*s))/sqrt(2*3.1415926)/s;
    }
}

complex_t hamiltonian(complex_t *matrix, int w, int h, int k)
{
    complex_t H = COMPLEX_ZERO;
    _complex_add(H, H, matrix[(k/w)*w+(k%w+1)%w]);
    _complex_add(H, H, matrix[(k/w)*w+(k%w+w-1)%w]);
    _complex_add(H, H, matrix[((k/w+1)%h)*w+k%w]);
    _complex_add(H, H, matrix[((k/w+h-1)%h)*w+k%w]);
    H.re = -hbar*hbar*(H.re - 4.0*matrix[k].re)/(2.0*mass) + potential((k%w)*2.0/(w-1)-1.0, (k/w)*2.0/(h-1)-1.0) * matrix[k].re;
    H.im = -hbar*hbar*(H.im - 4.0*matrix[k].im)/(2.0*mass) + potential((k%w)*2.0/(w-1)-1.0, (k/w)*2.0/(h-1)-1.0) * matrix[k].im;
    return H;
}

void evaluateM(complex_t *M, complex_t *x, int w, int h)
{
    int i;
    complex_t *H = (complex_t*)malloc(w * h * sizeof(complex_t));
    for(i = 0; i < w * h; i++)
        H[i] = hamiltonian(x,w,h,i);
    for(i = 0; i < w * h; i++)
        M[i] = hamiltonian(H,w,h,i);
    for(i = 0; i < w * h; i++){
        M[i].re = x[i].re + M[i].re * dt * dt / 4 / hbar / hbar;
        M[i].im = x[i].im + M[i].im * dt * dt / 4 / hbar / hbar;
    }
    free(H);
}

void distribution_compute(distribution *obj)
{
    int i, n = obj->size, w = obj->width, h = obj->height;
#ifdef WAVE
    complex_t tmp, *x = (complex_t*)malloc(n * sizeof(complex_t));
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

    complex_t *M = (complex_t*)malloc(n * sizeof(complex_t));
    complex_t *A = (complex_t*)malloc(n * sizeof(complex_t));
    complex_t *A2 = (complex_t*)malloc(n * sizeof(complex_t));
    complex_t *r = (complex_t*)malloc(n * sizeof(complex_t));
    complex_t *p = (complex_t*)malloc(n * sizeof(complex_t));

    for(i = 0; i < n; i++)
        A[i] = hamiltonian(obj->psi, w, h, i);
    for(i = 0; i < n; i++)
        A2[i] = hamiltonian(A, w, h, i);
    for(i = 0; i < n; i++){
        obj->psi[i].re = r[i].re = obj->psi[i].re + A[i].im * dt / hbar - A2[i].re * dt * dt / 4 / hbar / hbar;
        obj->psi[i].im = r[i].im = obj->psi[i].im - A[i].re * dt / hbar - A2[i].im * dt * dt / 4 / hbar / hbar;
    }

    double b = norm(r,n);

    evaluateM(M, r, w, h);
    for(i = 0; i < n; i++){
        _complex_sub(r[i], r[i], M[i]);
        p[i] = r[i];
    }

    double rs = norm(r,n);
    evaluateM(M, p, w, h);
    while(1){
        static complex_t alpha, beta, rAr, tmp;
        evaluateM(A, r, w, h);
        rAr = scalar(r,A,n);
        tmp = scalar(M,M,n);
        _complex_div(alpha, rAr, tmp);
        for(i = 0; i < n; i++){
            _complex_mul(tmp, alpha, p[i]);
            _complex_add(obj->psi[i], obj->psi[i], tmp);
            _complex_mul(tmp, alpha, M[i]);
            _complex_sub(r[i], r[i], tmp);
        }
        rs = norm(r,n);
        if(rs/b < 1e-6) break;
        evaluateM(A2, r, w, h);
        tmp = scalar(r,A2,n);
        _complex_div(beta, tmp, rAr);
        for(i = 0; i < n; i++){
            _complex_mul(tmp, p[i], beta);
            _complex_add(p[i], r[i], tmp);
            _complex_mul(tmp, beta, M[i]);
            _complex_add(M[i], tmp, A2[i]);
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
