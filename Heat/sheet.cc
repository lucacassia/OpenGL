#include "sheet.h"
#include <cmath>
sheet::sheet(int w,int h)
{
    width=w;
    height=h;
    pixels.resize(w*h);
    for(int n=0; n<w*h; n++)
        pixels[n]=(n%width-width/2)*(n%width-width/2)+(n/width-height/2)*(n/width-height/2);
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
    old=pixels;
}

