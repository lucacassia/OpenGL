#include "sheet.h"

sheet::sheet(int w,int h)
{
    width=w;
    height=h;
    pixels.resize(w*h);
    old=pixels;
    compute();
}

sheet::~sheet(){}
int sheet::w()const{return width;}
int sheet::h()const{return height;}

void sheet::compute()
{
    int size=height*width;
    for(int n=0; n<size; n++)
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
        if(n+width<size){
            env+=old[n+width];
            if((n+width)%width+1<width)
                env+=old[n+width+1];
            if((n+width)%width-1>=0)
                env+=old[n+width-1];
        }
        pixels[n]+=0.5*(env/8.0-old[n]);
    }
    pixels[width/2]=1000000;
    pixels[0]=1000000;
    pixels[width-1]=1000000;
    old=pixels;
}

