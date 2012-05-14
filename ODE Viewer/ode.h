#ifndef ODE
#define ODE

#include <math.h>

#define dt (1e-3)

double chaotic(double x,double v,double t){
    return -sin(x)-0.3*v+1.4*cos(2*t/3);
}

double vanderpol(double x,double v,double t){
    return 4*(1-x*x)*v-x;
}

double lorenz_x(double x,double y,double z){
    return 10*(y-x);
}

double lorenz_y(double x,double y,double z){
    return x*(28-z)-y;
}

double lorenz_z(double x,double y,double z){
    return x*y-8*z/3;
}

typedef struct _plist{
    double x,vx,y,vy,z,vz,t;
    struct _plist *prev;
    struct _plist *next;
}plist;

void plist_add_front(plist **head_ptr, double x, double vx, double y, double vy, double z, double vz, double t){
    plist *tmp = (plist*)malloc(sizeof(plist));
    tmp->x = x;
    tmp->y = y;
    tmp->z = z;
    tmp->vx = vx;
    tmp->vy = vy;
    tmp->vz = vz;
    tmp->t = t;
    tmp->next = *head_ptr;
    tmp->prev = NULL;
    *head_ptr = tmp;
    if(tmp->next != NULL)
        (tmp->next)->prev = tmp;
}

void plist_remove_last(plist **tail_ptr){
    if(*tail_ptr != NULL){
        *tail_ptr = (*tail_ptr)->prev;
        free((*tail_ptr)->next);
        (*tail_ptr)->next = NULL;
    }
}

void plist_erase(plist **head){
    while(*head != NULL){
        plist *tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}

void plist_evolve_ode(plist **head_ptr, plist **tail_ptr){
    double x, vx, y, vy, z, vz, k1, k2, k3, k4;
/*
    k1 = vanderpol((*head_ptr)->x,(*head_ptr)->vx,(*head_ptr)->t)*dt;
    k2 = vanderpol((*head_ptr)->x,(*head_ptr)->vx+k1/2,(*head_ptr)->t+dt/2)*dt;
    k3 = vanderpol((*head_ptr)->x,(*head_ptr)->vx+k2/2,(*head_ptr)->t+dt/2)*dt;
    k4 = vanderpol((*head_ptr)->x,(*head_ptr)->vx+k3,(*head_ptr)->t+dt)*dt;
    vx += k1/6+k2/3+k3/3+k4/6;
    x  += vx*dt;
*/
    vx = lorenz_x((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z);
    k1 = lorenz_x((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z)*dt;
    k2 = lorenz_x((*head_ptr)->x+k1/2,(*head_ptr)->y,(*head_ptr)->z)*dt;
    k3 = lorenz_x((*head_ptr)->x+k2/2,(*head_ptr)->y,(*head_ptr)->z)*dt;
    k4 = lorenz_x((*head_ptr)->x+k3,(*head_ptr)->y,(*head_ptr)->z)*dt;
    x = (*head_ptr)->x + k1/6+k2/3+k3/3+k4/6;

    vy = lorenz_y((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z);
    k1 = lorenz_y((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z)*dt;
    k2 = lorenz_y((*head_ptr)->x,(*head_ptr)->y+k1/2,(*head_ptr)->z)*dt;
    k3 = lorenz_y((*head_ptr)->x,(*head_ptr)->y+k2/2,(*head_ptr)->z)*dt;
    k4 = lorenz_y((*head_ptr)->x,(*head_ptr)->y+k3,(*head_ptr)->z)*dt;
    y = (*head_ptr)->y + k1/6+k2/3+k3/3+k4/6;

    vz = lorenz_z((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z);
    k1 = lorenz_z((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z)*dt;
    k2 = lorenz_z((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z)*dt;
    k3 = lorenz_z((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z)*dt;
    k4 = lorenz_z((*head_ptr)->x,(*head_ptr)->y,(*head_ptr)->z+k3)*dt;
    z = (*head_ptr)->z + k1/6+k2/3+k3/3+k4/6;

    plist_add_front(head_ptr, x, vx, y, vy, z, vz, 0);
}

#endif
