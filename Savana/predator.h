#ifndef PREDATOR
#define PREDATOR

#include"animal.h"
#include<cstdlib>

class predator: public animal{
	public:
		predator(double,double,double,double);
		~predator(){};
		animal* prox(animal*);
		void move(animal*,double);
};

predator::predator(double WW,double WH,double v,double l){
	pos.x=RAD+rand()*(WW-2*RAD)/RAND_MAX;
	pos.y=RAD+rand()*(WH-2*RAD)/RAND_MAX;
	lim.x=WW;
	lim.y=WH;
	speed=v;
	life=l;
	next=NULL;
}

animal* predator::prox(animal*s){
	while((s!=NULL)and(s->life<=0))
		s=s->next;
	animal* tmp=s;
	while(s!=NULL){
		if((s->life>0)and((s->pos-pos).mod()<(tmp->pos-pos).mod()))
			tmp=s;
		s=s->next;
	}
	return tmp;
}

void predator::move(animal* list,double dt){
	animal* PROX=prox(list);
	if(PROX!=NULL){
		pos+=(PROX->pos-pos)*dt*speed/(PROX->pos-pos).mod();
		if((PROX->pos-pos).mod()<lim.mod()/100){
			life+=150;
			PROX->life=0;
		}
	}
	life-=dt;
}

#endif
