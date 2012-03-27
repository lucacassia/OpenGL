#include "Fraq.h"
#include "../lib/Timer.h"
#include <pthread.h>

Fraq::Fraq()
{
    resetView();
    updateSize(600,600);
}

Fraq::~Fraq(){}

int Fraq::w() const
{
    return width;
}

int Fraq::h() const
{
    return height;
}

int Fraq::size() const
{
    return width*height;
}

double & Fraq::operator [] (int index)
{
    return pixels[index];
}

const double & Fraq::operator [] (int index) const
{
    return pixels[index];
}

Fraq Fraq::operator ++ (int)
{
    zoom*=2;
    shift/=2;
    compute();
    return *this;
}

Fraq Fraq::operator -- (int)
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

void Fraq::innerLoop(int i,int j)
{
    complex z(focus.Re()+(j+(1-width)/2)/zoom,focus.Im()+(i+(1-height)/2)/zoom);
    z = newComplex(1.0,0.0)/z;
    z = sinc(z)/cosc(z);
    pixels[i*width+j] = z.Mod();
}

void* thread0(void* arg)
{
    int w=((Fraq*)arg)->w();
    int h=((Fraq*)arg)->h();
    for(int n=0;n<w*h/4;n++)
        ((Fraq*)arg)->innerLoop(n/w,n%w);
	return NULL;
}

void* thread1(void* arg)
{
    int w=((Fraq*)arg)->w();
    int h=((Fraq*)arg)->h();
    for(int n=0;n<w*h/4;n++)
        ((Fraq*)arg)->innerLoop((w*h/4+n)/w,(w*h/4+n)%w);
	return NULL;
}

void* thread2(void* arg)
{
    int w=((Fraq*)arg)->w();
    int h=((Fraq*)arg)->h();
    for(int n=0;n<w*h/4;n++)
        ((Fraq*)arg)->innerLoop((w*h/2+n)/w,(w*h/2+n)%w);
	return NULL;
}

void* thread3(void* arg)
{
    int w=((Fraq*)arg)->w();
    int h=((Fraq*)arg)->h();
    for(int n=0;n<w*h/4;n++)
        ((Fraq*)arg)->innerLoop((3*w*h/4+n)/w,(3*w*h/4+n)%w);
	return NULL;
}

void Fraq::compute()
{
    Timer timer;
    timer.start();
    pthread_t ids[4];
    pthread_create(&ids[0],NULL,thread0,(void*)this);
    pthread_create(&ids[1],NULL,thread1,(void*)this);
    pthread_create(&ids[2],NULL,thread2,(void*)this);
    pthread_create(&ids[3],NULL,thread3,(void*)this);
    for(int i=0;i<4;i++)
        pthread_join(ids[i],NULL);
    timer.stop();
    std::cout<<"Compute: "<<timer.getElapsedTime()<<std::endl;
}

