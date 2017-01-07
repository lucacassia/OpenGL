#include "sheet.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

sheet::sheet(int w,int h)
{
    srand(time(NULL));
    width=w;
    height=h;
    pixels.resize(w*h);
    source.resize(w*h,0);
    for(int n=0;n<w*h;n++){
        pixels[n] = rand()*1.0/RAND_MAX;
    }
    old=pixels;
}

sheet::sheet(const sheet& arg)
{
    width=arg.w();
    height=arg.h();
    pixels.resize(width*height);
    source.resize(width*height,0);
    for(int n=0;n<width*height;n++){
        pixels[n]=arg[n];
        source[n]=arg.source[n];
    }
    old=pixels;    
}

sheet& sheet::operator =(const sheet& arg)
{
    width=arg.w();
    height=arg.h();
	pixels=arg.pixels;
	source=arg.source;
    old=pixels;
    return *this;
}

sheet::~sheet(){}
int sheet::w()const{return width;}
int sheet::h()const{return height;}
int sheet::size()const{return width*height;}
pthread_t sheet::id(int i)const{return ids[i];}
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
    if(n+width<size())
        env+=old[n+width];
    else env+=old[n];
    return pixels[n]+=0.5*(env/4-old[n])+source[n];
}

void* thread(void* arg)
{
    int w=((sheet*)arg)->w();
    int h=((sheet*)arg)->h();
    int id;
    for(int i=0;i<4;i++)
        if(pthread_equal(pthread_self(),((sheet*)arg)->id(i)))
            id=i;
    for(int n=0;n<w*h/4;n++)
        ((sheet*)arg)->compute(id*w*h/4+n);
	return NULL;
}

void sheet::operator++(int)
{
    pthread_create(&ids[0],NULL,thread,(void*)this);
    pthread_create(&ids[1],NULL,thread,(void*)this);
    pthread_create(&ids[2],NULL,thread,(void*)this);
    pthread_create(&ids[3],NULL,thread,(void*)this);
    for(int i=0;i<4;i++)
        pthread_join(ids[i],NULL);
    old=pixels;
}

