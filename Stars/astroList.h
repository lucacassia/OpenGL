#ifndef ASTROLIST
#define ASTROLIST

#include<cstdlib>
#include"astro.h"

class astroList{
    private:
        double gravity;
        std::vector<astro> planets;
        void merge(void);
    public:
        astroList(unsigned=0);
        astroList(const astroList&);
        ~astroList(){};

        void Move(void);
        void Add(const astro&);
        void Clear(void);
        void Draw(void (*)(point,double,point),void (*)(std::vector<point>,point));
};

	astroList::astroList(unsigned n){
        gravity = 1e-4;
		planets = *(new std::vector<astro>());
		for (int i = 0 ; i < n ; i++)
			planets.push_back(newAstro());
	}

	astroList::astroList(const astroList& arg){
        gravity = arg.gravity;
        planets = *(new std::vector<astro>());
    	for(int i = 0 ; i < arg.planets.size() ; i++)
            planets.push_back(arg.planets[i]);
    }

	void astroList::merge(){
		for( int i = 0 ; i < planets.size() ; i++){
			for (int j = i + 1 ; j < planets.size() ; j++){
				if( (planets[i].pos - planets[j].pos).mod() < planets[i].rad + planets[j].rad ){
					float mi = planets[i].mass;
					float mj = planets[j].mass;
					planets[i].pos   =  planets[i].pos*mi/(mi+mj) + planets[j].pos*mj/(mi+mj);
					planets[i].speed =  planets[i].speed*mi/(mi+mj) + planets[j].speed*mj/(mi+mj);
					planets[i].color =  planets[i].color*mi/(mi+mj) + planets[j].color*mj/(mi+mj);
                    planets[i].mass = mi+mj;
					planets[i].rad = cbrt(mi+mj);
                    planets[i].tail.clear();
                    planets[i].tail.push_back(planets[i].pos);
					planets.erase(planets.begin()+j);
				}
			}
		}
	}

    void astroList::Add(const astro& arg){
        planets.push_back(arg);
    }

    void astroList::Clear(){
        planets.clear();
    }

    void astroList::Move(){
        merge();
        astroList clone(*this);
        for(int i = 0 ; i < planets.size() ; i++){
            if( planets[i].tailLength()> 0.5 || planets[i].tail.size() > 128)
                planets[i].tail.erase(planets[i].tail.begin());
            planets[i].tail.push_back(planets[i].pos);
            for(int j = 0 ; j < planets.size() ; j++){
                if(i!=j){
                    point r(clone.planets[j].pos - clone.planets[i].pos);
                    planets[i].speed += r*gravity*planets[j].mass/(r.mod()*r.mod()*r.mod());
                }
                planets[i].pos += planets[i].speed;
            }
        }
        merge();
    }

    void astroList::Draw(void drawf(point,double,point),void drawl(std::vector<point>,point)){
        for(int i = 0 ; i < planets.size() ; i++){
            drawf(planets[i].pos,planets[i].rad,planets[i].color);
            for(int j = 0 ; j < planets[i].tail.size() ; j++){
                drawl(planets[i].tail,planets[i].color);
            }
        }
    }

#endif
