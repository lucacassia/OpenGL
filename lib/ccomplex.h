#ifndef COMPLEX_H
#define COMPLEX_H

#include <math.h>

typedef struct{
   double re,im;
} complex;

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


#endif

