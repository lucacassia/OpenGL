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

typedef struct _plist{
    double x,y,t;
    struct _plist *prev;
    struct _plist *next;
}plist;

void plist_add_front(plist **head_ptr, double x, double y, double t){
    plist *tmp = (plist*)malloc(sizeof(plist));
    tmp->x = x;
    tmp->y = y;
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
    double x, y, t, k1, k2, k3, k4;
    x = (*head_ptr)->x;
    y = (*head_ptr)->y;
    t = (*head_ptr)->t;
    k1 = vanderpol(x,y,t)*dt;
    k2 = vanderpol(x,y+k1/2,t+dt/2)*dt;
    k3 = vanderpol(x,y+k2/2,t+dt/2)*dt;
    k4 = vanderpol(x,y+k3,t+dt)*dt;
    plist_add_front(head_ptr, x+y*dt, y+k1/6+k2/3+k3/3+k4/6, t+dt);
}

#endif
