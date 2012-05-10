#ifndef ODE
#define ODE

#include<vector>
#include"point.h"

#define dt (1e-3)

class ode{
    public:
        double x;
        double v;
        double t;
        std::vector<point> tail;
        double chaotic(double,double,double);
        double vanderpol(double,double,double);

    public:
        ode(double,double);
        ode(const ode&);
        ode(const point&);
        ~ode(){};
        ode& operator =(const ode&);
        float tailLength();
        point pos()const;
        void operator++(int);

};

double ode::chaotic(double X,double V,double T){
    return -sin(X)-0.3*V+1.4*cos(2*T/3);
}

double ode::vanderpol(double X,double V,double T){
    return 4*(1-X*X)*V-X;
}

ode::ode(double X,double V){
    x=X;
    v=V;
    t=0;
    tail.resize(1,newPoint(x,v));
}

ode::ode(const point& p){
    x=p.x;
    v=p.y;
    t=0;
    tail.resize(1,p);
}

ode::ode(const ode& arg){
    x=arg.x;
    v=arg.v;
    t=arg.t;
    tail=arg.tail;
}

ode& ode::operator =(const ode& arg)
{
    x=arg.x;
    v=arg.v;
    t=arg.t;
    tail=arg.tail;
    return *this;
}

float ode::tailLength()
{
    float tmp = 0;
    for(size_t i = 0; i < tail.size()-1; i++)
        tmp += (tail[i+1]-tail[i]).mod();
    return tmp;
}

point ode::pos()const
{
    return newPoint(x,v);
}

void ode::operator++(int)
{
    double k1,k2,k3,k4;
    k1=vanderpol(x,v,t)*dt;
    k2=vanderpol(x,v+k1/2,t+dt/2)*dt;
    k3=vanderpol(x,v+k2/2,t+dt/2)*dt;
    k4=vanderpol(x,v+k3,t+dt)*dt;
    x+=v*dt;
    v+=k1/6+k2/3+k3/3+k4/6;
    t+=dt;
    if((tail[tail.size()-1]-pos()).mod()>0.01)
        tail.push_back(pos());
    if(tailLength()>500)
        tail.erase(tail.begin());
}

ode newode(const point& p){
    ode tmp(p);
    return tmp;
}

ode newode(double X,double V){
    ode tmp(X,V);
    return tmp;
}

#endif

typedef struct _plist{
    double x,y;
    struct _plist *next;
}plist;

void plist_add_front(plist **head_ptr, double x, double y){
    plist *tmp = (plist*)malloc(sizeof(plist));
    tmp->x = x;
    tmp->y = y;
    tmp->next = *head_ptr;
    *head_ptr = tmp;
}

void plist_remove_last(plist *head){
    plist *tmp = head;
    while(tmp->next != NULL)
        tmp = tmp->next;
    
}
