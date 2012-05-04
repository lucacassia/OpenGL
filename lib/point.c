#include"point.h"

point::point(double X,double Y,double Z)
{
	x=X;
	y=Y;
	z=Z;
}

point::point(const point& arg)
{
	x=arg.x;
	y=arg.y;
	z=arg.z;
}

point point::clear(void)
{
    x=0;
    y=0;
    z=0;
	return *this;
}

point point::operator +(const point& arg)const
{
	point tmp(arg);
	tmp.x+=x;
	tmp.y+=y;
	tmp.z+=z;
	return tmp;
}

double point::operator *(const point& arg)const
{
	return x*arg.x+y*arg.y+z*arg.z;
}

point point::operator *(double arg)const
{
	point tmp(*this);
	tmp.x*=arg;
	tmp.y*=arg;
	tmp.z*=arg;
	return tmp;
}

point point::operator /(double arg)const
{
	return (*this)*(1/arg);
}

point point::operator -()const
{
	return (*this)*(-1);
}

point point::operator -(const point& arg)const
{
	return (*this)+(-arg);
}

point point::operator +=(const point& arg)
{
	return (*this)=(*this)+arg;
}

point point::operator -=(const point& arg)
{
	return (*this)=(*this)-arg;
}

point point::operator *=(double arg)
{
	return (*this)=(*this)*arg;
}

point point::operator /=(double arg)
{
	return (*this)=(*this)/arg;
}

double point::mod(void)const
{
	return sqrt((*this)*(*this));
}

bool point::operator ==(const point& arg)const
{
	return (x==arg.x && y==arg.y && z==arg.z);
}

bool point::operator !=(const point& arg)const
{
	return !((*this)==arg);
}

point& point::operator =(const point& arg)
{
	x=arg.x;
	y=arg.y;
	z=arg.z;
	return *this;
}

std::ostream& operator <<(std::ostream& s,const point& arg)
{
	s<<"("<<arg.x<<","<<arg.y<<","<<arg.z<<")";
	return s;
}

point newPoint(double x,double y,double z)
{
    point tmp(x,y,z);
    return tmp;
}


