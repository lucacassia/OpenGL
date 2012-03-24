#ifndef ASTRO
#define ASTRO

#include<cstdlib>
#include<vector>
#include"../lib/point.h"

#define RADIUM 0.03

float newRand(){
    return rand()/(float)RAND_MAX;
}

class astro{
    public:
        point pos;
        point speed;
        point color;
        double mass;
        double rad;
        std::vector<point> tail;

        astro();
        astro(const point&,const point&);
        astro(const astro&);
        ~astro(){};
        astro& operator =(const astro&);
        float tailLength();

};

astro::astro(){
    pos=newPoint(2*newRand()-1,2*newRand()-1);
    speed=newPoint();
    color=newPoint(newRand(),newRand(),newRand());
    rad=RADIUM;
    mass=rad*rad*rad;
    tail=*new std::vector<point>(1,pos);
}

astro::astro(const point& position,const point& velocity){
    pos=position;
    speed=velocity;
    color=newPoint(newRand(),newRand(),newRand());
    rad=RADIUM;
    mass=rad*rad*rad;
    tail=*new std::vector<point>(1,pos);
}

astro::astro(const astro& arg){
    pos=arg.pos;
    speed=arg.speed;
    color=arg.color;
    mass=arg.mass;
    rad=arg.rad;
    tail=arg.tail;
}

astro& astro::operator =(const astro& arg)
{
    pos=arg.pos;
    speed=arg.speed;
    color=arg.color;
    mass=arg.mass;
    rad=arg.rad;
    tail=arg.tail;
    return *this;
}

float astro::tailLength()
{
    float tmp = 0;
    for(int i = 0; i < tail.size()-1; i++)
        tmp += (tail[i+1]-tail[i]).mod();
    return tmp;
}

astro newAstro(const point& position,const point& velocity){
    astro tmp(position,velocity);
    return tmp;
}

astro newAstro(){
    astro tmp;
    return tmp;
}

#endif
