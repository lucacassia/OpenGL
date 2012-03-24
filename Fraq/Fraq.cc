#include "Fraq.h"
#include "../lib/Timer.h"
#include <omp.h>

Fraq::Fraq()
{
    resetView();
    updateSize(600,600);
}

Fraq::~Fraq(){}

int Fraq::W()const
{
    return width;
}

int Fraq::H()const
{
    return height;
}

Fraq Fraq::operator ++(int)
{
    zoom*=2;
    shift/=2;
    compute();
    return *this;
}

Fraq Fraq::operator --(int)
{
    zoom/=2;
    shift*=2;
    compute();
    return *this;
}

void Fraq::left()
{
    focus.real-=shift;
    compute();
}

void Fraq::right()
{
    focus.real+=shift;
    compute();
}

void Fraq::up()
{
    focus.imag+=shift;
    compute();
}

void Fraq::down()
{
    focus.imag-=shift;
    compute();
}

void Fraq::resetView()
{
    focus.Set(0.0,0.0);
    zoom = 256;
    shift = 0.5;
}

void Fraq::updateSize(int w, int h)
{
    width = w;
    height = h;
    pixels.resize(w * h);
    compute();
}

short Fraq::innerLoop(int i,int j)
{
    short k;
    complex z(focus.Re()+(j+(1-width)/2)/zoom,focus.Im()+(i+(1-height)/2)/zoom);
    z=newComplex(1.0,0.0)/z;
    z=sinc(z)/cosc(z);
    return 100*z.Mod();
}

void Fraq::compute()
{
    Timer timer;
    timer.start();
#pragma omp parallel for        // OpenMP
    for(int n=0; n<height*width; n++)
    {
        pixels[n]=innerLoop(n/width,n%width);
    }
    timer.stop();
    std::cout<<"Compute: "<<timer.getElapsedTime()<<std::endl;
}

