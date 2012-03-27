#include "sheet.h"
#include <pthread.h>

#include <cstdlib>
#include <ctime>

sheet::sheet(int w,int h)
{
    srand(time(NULL));
    width=w;
    height=h;
    pixels.resize(w*h);
    for(int n=0;n<size();n++)
        if(rand()%2)
            pixels[n] = rand()*0.1/RAND_MAX;
        else
            pixels[n] = 0.9+rand()*0.1/RAND_MAX;
    old=pixels;
}

sheet::~sheet(){}
int sheet::w()const{return width;}
int sheet::h()const{return height;}
int sheet::size()const{return width*height;}
double& sheet::operator[](int index){return pixels[index];}
const double& sheet::operator[](int index)const{return pixels[index];}

double sheet::compute(int n)
{
    double env=0;
    if(n%width+1<width)
        env+=old[n+1];
    else env+=old[n];
    if(n%width-1>=0)
        env+=old[n-1];
    else env+=old[n];
    if(n-width>=0)
        env+=old[n-width];
    else env+=old[n];
    if(((n-width)%width+1<width)&&(n-width>=0))
        env+=old[n-width+1];
    else env+=old[n];
    if(((n-width)%width-1>=0)&&(n-width>=0))
        env+=old[n-width-1];
    else env+=old[n];
    if(n+width<size())
        env+=old[n+width];
    else env+=old[n];
    if(((n+width)%width+1<width)&&(n+width<size()))
        env+=old[n+width+1];
    else env+=old[n];
    if(((n+width)%width-1>=0)&&(n+width<size()))
        env+=old[n+width-1];
    else env+=old[n];
    return pixels[n]+=0.5*(env/8-old[n]);
}

void* thread0(void* arg)
{
    int w=((sheet*)arg)->w();
    int h=((sheet*)arg)->h();
    for(int n=0;n<w*h/4;n++)
        ((sheet*)arg)->compute(n);
	return NULL;
}

void* thread1(void* arg)
{
    int w=((sheet*)arg)->w();
    int h=((sheet*)arg)->h();
    for(int n=0;n<w*h/4;n++)
        ((sheet*)arg)->compute(w*h/4+n);
	return NULL;
}

void* thread2(void* arg)
{
    int w=((sheet*)arg)->w();
    int h=((sheet*)arg)->h();
    for(int n=0;n<w*h/4;n++)
        ((sheet*)arg)->compute(w*h/2+n);
	return NULL;
}

void* thread3(void* arg)
{
    int w=((sheet*)arg)->w();
    int h=((sheet*)arg)->h();
    for(int n=0;n<w*h/4;n++)
        ((sheet*)arg)->compute(3*w*h/4+n);
	return NULL;
}

void sheet::operator++(int)
{
    pthread_t ids[4];
    pthread_create(&ids[0],NULL,thread0,(void*)this);
    pthread_create(&ids[1],NULL,thread1,(void*)this);
    pthread_create(&ids[2],NULL,thread2,(void*)this);
    pthread_create(&ids[3],NULL,thread3,(void*)this);
    for(int i=0;i<4;i++)
        pthread_join(ids[i],NULL);
    old=pixels;
}

