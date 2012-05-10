#ifndef COMPLEX_T
#define COMPLEX_T

#include <math.h>

typedef struct{
   double re,im;
} complex_t;

static const complex_t COMPLEX_ZERO = {.re = 0, .im = 0};

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
   sqrt((a).re*(a).re+(a).im*(a).im)


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


complex_t scalardot(complex_t *x, complex_t *y, int n)
{
    complex_t tmp;
    tmp.re = tmp.im = 0;
    int i;
    for(i = 0; i < n; i++){
        x[n].im = -x[n].im;
        _complex_mul(tmp,x[n],y[n]);
    }
    return tmp;
}

double mod(complex_t *x, int n)
{
    double tmp = 0;
    int i;
    for(i = 0; i < n; i++)
        tmp += x[i].re * x[i].re + x[i].im * x[i].im;
    return sqrt(tmp);
}

void normalize(complex_t *matrix, int n)
{
    double tmp = mod(matrix,n);
    int i;
    for(i = 0; i < n; i++){
        matrix[i].re /= tmp;
        matrix[i].im /= tmp;
    }
}


#endif

