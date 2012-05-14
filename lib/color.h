#ifndef COLOR_H
#define COLOR_H

#include <math.h>

typedef struct{
    float r,g,b;
}rgb_t;

typedef struct{
    float r,g,b;
}rgb8U_t;

rgb_t d2rgb(double x)
{
    if(x<0) x += -floor(x)+1;
    if(x>=1) x -= floor(x);

    rgb_t obj;
    if(0<= x && x<1.0/6.0){
        obj.r = 0;
        obj.g = 6*x;
        obj.b = 1;
    }
    if(1.0/6.0<= x && x<2.0/6.0){
        obj.r = 0;
        obj.g = 1;
        obj.b = 2-6*x;
    }
    if(2.0/6.0<= x && x<3.0/6.0){
        obj.r = 6*x-2;
        obj.g = 1;
        obj.b = 0;
    }
    if(3.0/6.0<= x && x<4.0/6.0){
        obj.r = 1;
        obj.g = 4-6*x;
        obj.b = 0;
    }
    if(4.0/6.0<= x && x<5.0/6.0){
        obj.r = 1;
        obj.g = 0;
        obj.b = 6*x-4;
    }
    if(5.0/6.0<= x && x<6.0/6.0){
        obj.r = 6-6*x;
        obj.g = 0;
        obj.b = 1;
    }
    return obj;
}

double rgb2d(rgb_t c)
{
    double d = sqrt(c.r*c.r+c.g*c.g+c.b*c.b);
    if(c.r==1 && c.g==0) d=(c.b+4)/6;
    if(c.g==1 && c.b==0) d=(c.r+2)/6;
    if(c.b==1 && c.r==0) d=(c.g+0)/6;
    if(c.r==1 && c.b==0) d=(4-c.g)/6;
    if(c.g==1 && c.r==0) d=(2-c.b)/6;
    if(c.b==1 && c.g==0) d=(6-c.r)/6;
    return d;
}

double rgb8U2d(rgb8U_t c)
{
    double d = sqrt(c.r*c.r+c.g*c.g+c.b*c.b)/255.0f;
    if(c.r==255 && c.g==0) d=(c.b/255.0+4)/6;
    if(c.g==255 && c.b==0) d=(c.r/255.0+2)/6;
    if(c.b==255 && c.r==0) d=(c.g/255.0+0)/6;
    if(c.r==255 && c.b==0) d=(4-c.g/255.0)/6;
    if(c.g==255 && c.r==0) d=(2-c.b/255.0)/6;
    if(c.b==255 && c.g==0) d=(6-c.r/255.0)/6;
    return d;
}

rgb8U_t d2rgb8U(double x)
{
    if(x<0) x += -floor(x)+1;
    if(x>=1) x -= floor(x);

    rgb8U_t obj;
    if(0<= x && x<1.0/6.0){
        obj.r = 0;
        obj.g = floor((6*x)*255);
        obj.b = 255;
    }
    if(1.0/6.0<= x && x<2.0/6.0){
        obj.r = 0;
        obj.g = 255;
        obj.b = floor((2-6*x)*255);
    }
    if(2.0/6.0<= x && x<3.0/6.0){
        obj.r = floor((6*x-2)*255);
        obj.g = 255;
        obj.b = 0;
    }
    if(3.0/6.0<= x && x<4.0/6.0){
        obj.r = 255;
        obj.g = floor((4-6*x)*255);
        obj.b = 0;
    }
    if(4.0/6.0<= x && x<5.0/6.0){
        obj.r = 255;
        obj.g = 0;
        obj.b = floor((6*x-4)*255);
    }
    if(5.0/6.0<= x && x<6.0/6.0){
        obj.r = floor((6-6*x)*255);
        obj.g = 0;
        obj.b = 255;
    }
    return obj;
}


#endif
