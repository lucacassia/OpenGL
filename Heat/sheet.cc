#include "sheet.h"
#include <cmath>

sheet::sheet(int w,int h)
{
    width=w;
    height=h;
    pixels.resize(w*h);
    for(int n=0; n<w*h; n++)
        if((n%width-width/2)*(n%width-width/2)+(n/width-height/2)*(n/width-height/2)<10000)
            pixels[n]=1e9;
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
        if(n%width-1>=0)
            env+=old[n-1];
        if(n-width>=0){
            env+=old[n-width];
            if((n-width)%width+1<width)
                env+=old[n-width+1];
            if((n-width)%width-1>=0)
                env+=old[n-width-1];
        }
        if(n+width<size()){
            env+=old[n+width];
            if((n+width)%width+1<width)
                env+=old[n+width+1];
            if((n+width)%width-1>=0)
                env+=old[n+width-1];
        }
        pixels[n]+=0.5*(env/8-old[n]);
    }
    old=pixels;
}

