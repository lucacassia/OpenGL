#ifndef VDP
#define VDP

#include<cstdlib>
#include<vector>
#include"../lib/point.h"

#define dt (1e-3)

class vdp{
    public:
        double x;
        double v;
        std::vector<point> tail;

    public:
        vdp(double,double);
        vdp(const vdp&);
        vdp(const point&);
        ~vdp(){};
        vdp& operator =(const vdp&);
        float tailLength();
        point pos()const;
        void operator++(int);

};

vdp::vdp(double X,double V){
    x=X;
    v=V;
    tail.resize(1,newPoint(x,v));
}

vdp::vdp(const point& p){
    x=p.x;
    v=p.y;
    tail.resize(1,p);
}

vdp::vdp(const vdp& arg){
    x=arg.x;
    v=arg.v;
    tail=arg.tail;
}

vdp& vdp::operator =(const vdp& arg)
{
    x=arg.x;
    v=arg.v;
    tail=arg.tail;
    return *this;
}

float vdp::tailLength()
{
    float tmp = 0;
    for(int i = 0; i < tail.size()-1; i++)
        tmp += (tail[i+1]-tail[i]).mod();
    return tmp;
}

point vdp::pos()const
{
    return newPoint(x,v);
}

void vdp::operator++(int)
{
    point tmp(pos());
    v+=(4*(1-tmp.x*tmp.x)*tmp.y-tmp.x)*dt;
    x+=tmp.y*dt;
    if((tail[tail.size()-1]-pos()).mod()>0.01)
        tail.push_back(pos());
//    if(tailLength()>10)
  //      tail.erase(tail.begin());
}

vdp newVdp(const point& p){
    vdp tmp(p);
    return tmp;
}

vdp newVdp(double X,double V){
    vdp tmp(X,V);
    return tmp;
}

#endif
