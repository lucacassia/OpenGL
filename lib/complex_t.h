#ifndef COMPLEX_T
#define COMPLEX_T

#include <math.h>

typedef struct{
   double re,im;
} complex_t;

static const complex_t COMPLEX_ZERO = {.re = 0.0, .im = 0.0};
static const complex_t COMPLEX_ONE = {.re = 1.0, .im = 0.0};
static const complex_t COMPLEX_I = {.re = 0.0, .im = 1.0};

/*
* a=b*c (a,b,c complex)
*/

#define _complex_mul(a,b,c) \
   (a).re=((b).re*(c).re-(b).im*(c).im); \
   (a).im=((b).re*(c).im+(b).im*(c).re)


/*
* a=b+c (a,b,c complex)
*/

#define _complex_add(a,b,c) \
   (a).re=((b).re+(c).re); \
   (a).im=((b).im+(c).im)


/*
* a=b-c (a,b,c complex)
*/

#define _complex_sub(a,b,c) \
   (a).re=((b).re-(c).re); \
   (a).im=((b).im-(c).im)


/*
* a=b/c (a,b,c complex)
*/

#define _complex_div(a,b,c) \
   (a).re=(((b).re*(c).re+(b).im*(c).im)/((c).re*(c).re+(c).im*(c).im)); \
   (a).im=(((b).im*(c).re-(b).re*(c).im)/((c).re*(c).re+(c).im*(c).im))


/*
* mod(a) (a complex)
*/

#define _complex_mod(a) \
   hypot((a).re,(a).im)


/*
* arg(a) (a complex)
*/

#define _complex_arg(a) \
   atan2((a).im,(a).re)


/*
* a=sin(b) (a,b complex)
*/

#define _complex_sin(a,b) \
   (a).re=(exp(-(b).im)*sin((b).re)-exp((b).im)*sin(-(b).re))/2; \
   (a).im=-(exp(-(b).im)*cos((b).re)-exp((b).im)*cos(-(b).re))/2


/*
* a=cos(b) (a,b complex)
*/

#define _complex_cos(a,b) \
   (a).re=(exp(-(b).im)*cos((b).re)+exp((b).im)*cos(-(b).re))/2; \
   (a).im=(exp(-(b).im)*sin((b).re)+exp((b).im)*sin(-(b).re))/2


/*
* a=exp(b) (a,b complex)
*/

#define _complex_exp(a,b) \
   (a).re=exp((b).re)*cos((b).im); \
   (a).im=exp((b).re)*sin((b).im)


complex_t scalar(complex_t *x, complex_t *y, int n)
{
    complex_t tmp = COMPLEX_ZERO;
    int i;
    for(i = 0; i < n; i++){
        tmp.re += x[i].re * y[i].re + x[i].im * y[i].im;
        tmp.im += x[i].re * y[i].im - x[i].im * y[i].re;
    }
    return tmp;
}

double norm(complex_t *x, int n)
{
    double tmp = 0.0;
    int i;
    for(i = 0; i < n; i++)
        tmp += x[i].re * x[i].re + x[i].im * x[i].im;
    return sqrt(tmp);
}

void normalize(complex_t *x, int n)
{
    double tmp = norm(x,n);
    int i;
    for(i = 0; i < n; i++){
        x[i].re /= tmp;
        x[i].im /= tmp;
    }
}

complex_t laplacian2d(complex_t *matrix, int w, int h, int k)
{
    complex_t env = COMPLEX_ZERO;
    _complex_add(env, env, matrix[(k/w)*w+(k%w+1)%w]);
    _complex_add(env, env, matrix[(k/w)*w+(k%w+w-1)%w]);
    _complex_add(env, env, matrix[((k/w+1)%h)*w+k%w]);
    _complex_add(env, env, matrix[((k/w+h-1)%h)*w+k%w]);
    env.re = (env.re / 4.0 - matrix[k].re);
    env.im = (env.im / 4.0 - matrix[k].im);
    return env;
}


#endif
