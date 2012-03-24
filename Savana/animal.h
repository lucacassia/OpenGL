#ifndef ANIMAL
#define ANIMAL

#include"../lib/point.h"

class animal{
	public:
		point lim;
		double speed;
		double life;
		point pos;
		animal* next;

		animal(){};
		virtual ~animal()=0;
		virtual void move(animal*,double)=0;

};

animal::~animal(){}

#endif
