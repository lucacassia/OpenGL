#ifndef PREY
#define PREY

#include"animal.h"
#include<cstdlib>

#define RAD 5

class prey: public animal{
	public:
		prey(double,double,double);
		~prey(){};
		void move(animal*,double);
};

prey::prey(double WW,double WH,double v){
	pos.x=RAD+rand()*(WW-2*RAD)/RAND_MAX;
	pos.y=RAD+rand()*(WH-2*RAD)/RAND_MAX;
	lim.x=WW;
	lim.y=WH;
	speed=v;
	life=1;
	next=NULL;
}

void prey::move(animal* list,double dt){
	point dir;
	while(list!=NULL){
		if(list->life>0)
			dir+=(pos-list->pos)/((pos-list->pos)*(pos-list->pos));
		list=list->next;
	}
	if(dir.mod()!=0)	dir*=dt*speed/dir.mod();

	if(((pos+dir).x<RAD)or((pos+dir).x>lim.x-RAD))
		dir.x=-dir.x;
	if(((pos+dir).y<RAD)or((pos+dir).y>lim.y-RAD))
		dir.y=-dir.y;
	pos+=dir;
}

#endif
