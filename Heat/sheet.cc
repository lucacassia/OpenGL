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
    for(int n=0; n<size(); n++)
        pixels[n]=rand()*1.0/RAND_MAX;
    old=pixels;
    compute();
}

sheet::~sheet(){}
int sheet::w()const{return width;}
int sheet::h()const{return height;}
int sheet::size()const{return width*height;}
double& sheet::operator[](int index){return pixels[index];}
const double& sheet::operator[](int index)const{return pixels[index];}

void sheet::compute()
{
    for(int n=0; n<size(); n++)
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
        pixels[n]+=0.5*(env/8-old[n]);
        pixels[size()/2+width/2]=0;
    }
    old=pixels;
}

