#ifndef COMPLEX_T
#define COMPLEX_T

#include <math.h>

typedef struct{
   double re,im;
} complex_t;

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


#endif

