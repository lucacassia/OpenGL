#include "Julia.h"
#include "../lib/Timer.h"

Julia::Julia()
{
    c.real = 0.3;
    c.imag = 0.3;
    resetView();
    div = 3;
    jexp = 2;
    halt = 512;
    mode = 2;
    updateSize(600,600);
}

Julia::~Julia(){}

int Julia::W()const
{
    return width;
}

int Julia::H()const
{
    return height;
}

Julia Julia::operator ++(int)
{
    zoom*=2;
    shift/=2;
    compute();
    return *this;
}

Julia Julia::operator --(int)
{
    zoom/=2;
    shift*=2;
    compute();
    return *this;
}

void Julia::left()
{
    focus.real-=shift;
    compute();
}

void Julia::right()
{
    focus.real+=shift;
    compute();
}

void Julia::up()
{
    focus.imag+=shift;
    compute();
}

void Julia::down()
{
    focus.imag-=shift;
    compute();
}

void Julia::a()
{
    c.real /= 1.01;
    compute();
}

void Julia::d()
{
    c.real *= 1.01;
    compute();
}

void Julia::w()
{
    c.imag *= 1.01;
    compute();
}

void Julia::s()
{
    c.imag /= 1.01;
    compute();
}


void Julia::resetView()
{
    focus = -~c;
    zoom = 256;
    shift = 0.5;
}

void Julia::updateSize(int w, int h)
{
    width = w;
    height = h;
    pixels.resize(w * h);
    compute();
}

unsigned Julia::innerLoop(int i,int j)
{
    unsigned k;
    complex z(focus.Re()+(j+(1-width)/2)/zoom,focus.Im()+(i+(1-height)/2)/zoom);
    for(k=0; (k<halt)and(z.Mod()<div); k++){
        if(mode==1)
            z=~z;
        if(mode==2)
            z.Set(fabs(z.Re()),fabs(z.Im()));
        if(mode==3)
            z.Set(z.Re(),fabs(z.Im()));
        z=c+z^jexp;
    }
    return k;
}

void Julia::compute()
{
    Timer timer;
    timer.start();
    #pragma omp parallel for
    for(int n=0; n<height*width; n++)
    {
        pixels[n]=innerLoop(n/width,n%width);
    }
    timer.stop();
    std::cout<<timer.getElapsedTime()<< " c: " << c.real << "," << c.imag << std::endl;
}

