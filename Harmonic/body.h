#ifndef BODY
#define BODY

#include<cstdlib>
#include"../lib/point.h"

float newRand(){
    return rand()/(float)RAND_MAX;
}

class body{
    public:
        point pos;
        point speed;
        double k;

        body();
        body(const point&,const point&,const double=0);
        body(const body&);
        ~body(){};
        body& operator =(const body&);
        void move();

};

body::body(){
    pos=newPoint(2*newRand()-1,2*newRand()-1);
    speed=newPoint();
    k=newRand();
    
}

body::body(const point& position,const point& velocity,const double springk){
    pos=position;
    speed=velocity;
    k=springk;
    if(k==0)
        k=newRand();
}

body::body(const body& arg){
    pos=arg.pos;
    speed=arg.speed;
    k=arg.k;
}

body& body::operator =(const body& arg)
{
    pos=arg.pos;
    speed=arg.speed;
    k=arg.k;
    return *this;
}

body newBody(){
    body tmp;
    return tmp;
}

body newBody(const point& position,const point& velocity,const double springk=0){
    body tmp(position,velocity,springk);
    return tmp;
}

void body::move(){
    pos+=speed;
    speed-=pos*k*1e-5;
}

#endif
