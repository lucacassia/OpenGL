#ifndef COLOR_H
#define COLOR_H

class rgb_t
{
    public:
        float r,g,b;
        rgb_t set(double);
        rgb_t(double x=0){set(x);};
        rgb_t(const rgb_t& arg){r=arg.r;g=arg.g;b=arg.b;};
        rgb_t(double R,double G,double B){r=R;g=G;b=B;};
        ~rgb_t(){};
};

rgb_t rgb_t::set(double x)
{
    r = g = b = 0;
    if(x<0) x += -int(x)+1;
    if(x>=1) x = x-int(x);
    if(0<= x && x<1.0/6.0){
        r = 0;
        g = 6*x;
        b = 1;
    }
    if(1.0/6.0<= x && x<2.0/6.0){
        r = 0;
        g = 1;
        b = 2-6*x;
    }
    if(2.0/6.0<= x && x<3.0/6.0){
        r = 6*x-2;
        g = 1;
        b = 0;
    }
    if(3.0/6.0<= x && x<4.0/6.0){
        r = 1;
        g = 4-6*x;
        b = 0;
    }
    if(4.0/6.0<= x && x<5.0/6.0){
        r = 1;
        g = 0;
        b = 6*x-4;
    }
    if(5.0/6.0<= x && x<6.0/6.0){
        r = 6-6*x;
        g = 0;
        b = 1;
    }
    return *this;
}

#include<cmath>
double rgb2d(rgb_t c)
{
    double d=sqrt(c.r*c.r+c.g*c.g+c.b*c.b);
    if(c.r==1 && c.g==0) d=(c.b+4)/6;
    if(c.g==1 && c.b==0) d=(c.r+2)/6;
    if(c.b==1 && c.r==0) d=(c.g+0)/6;
    if(c.r==1 && c.b==0) d=(4-c.g)/6;
    if(c.g==1 && c.r==0) d=(2-c.b)/6;
    if(c.b==1 && c.g==0) d=(6-c.r)/6;
    return d;
}

#endif
