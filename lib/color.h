#ifndef COLOR_H
#define COLOR_H

class rgb_t
{
    public:
        float r,g,b;
        rgb_t set(double);
        rgb_t(double x=0){set(x);};
        rgb_t(const rgb_t& arg){r=arg.r;g=arg.g;b=arg.b;};
        ~rgb_t(){};
};

rgb_t rgb_t::set(double x)
{
    r = g = b = 0;
    if(x<0)
        x+=-int(x)+1;
    if(x>=1)
        x=x-int(x);
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

#endif
