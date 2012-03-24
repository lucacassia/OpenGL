#ifndef POINT
#define POINT

#include<iostream>
#include<cmath>

class point{
	public:
		double x,y,z;
		point(double=0,double=0,double=0);
		point(const point&);
        point clear(void);
		point operator +(const point&)const;
		double operator *(const point&)const;
		point operator *(double)const;
		point operator /(double)const;
		point operator -(void)const;
		point operator -(const point&)const;
		point operator +=(const point&);
		point operator -=(const point&);
		point operator *=(double);
		point operator /=(double);
		double mod(void)const;
		bool operator ==(const point&)const;
		bool operator !=(const point&)const;
		point& operator =(const point&);
		friend std::ostream& operator <<(std::ostream&,const point&);
};

point newPoint(double=0,double=0,double=0);

#endif
