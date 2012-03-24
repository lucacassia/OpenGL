#ifndef ANIMALLIST
#define ANIMALLIST

#include"prey.h"
#include"predator.h"

class animalList{
	private:
		animal* head;
	public:
		animalList(const char*,unsigned,double,double,double,double=1);
		~animalList();
		animal* init(const char*,unsigned,double,double,double,double);
		void clear(animal*);
		void Run(animal*,double,point,void (*)(point,double,point));
		animal* Head(void);
};

animalList::animalList(const char*type,unsigned n,double WW,double WH,double speed,double life){
	head=init(type,n,WW,WH,speed,life);
}

animalList::~animalList(){
	clear(head);
}

animal* animalList::init(const char*type,unsigned n,double WW,double WH,double speed,double life){
	animal* tmp=NULL;
	if(n!=0)
		if(type=="prey"){
			tmp=new prey(WW,WH,speed);
			tmp->next=init(type,n-1,WW,WH,speed,life);
		}else if(type=="predator"){
			tmp=new predator(WW,WH,speed,life);
			tmp->next=init(type,n-1,WW,WH,speed,life);
		}
	return tmp;
}

void animalList::clear(animal* list){
	if(list!=NULL){
		clear(list->next);
		delete list;
	}
}

void animalList::Run(animal* other,double dt,point color,void drawf(point,double,point)){
	animal* tmp=head;
	while(tmp!=NULL){
		if(tmp->life>0){
			tmp->move(other,dt);
			drawf(tmp->pos,RAD,color);
		}
		tmp=tmp->next;
	}
}

animal* animalList::Head(){
	return head;
}

#endif
