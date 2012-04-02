#include "complex.h"

complex::complex(long double r,long double i)
{
	real=r;
	imag=i;
}

long double complex::Re()const
{
	return real;
}

long double complex::Im()const
{
	return imag;
}

complex::complex(const complex& c)
{
	real=c.Re();
	imag=c.Im();
}

void complex::Set(long double r,long double i)
{
	real=r;
	imag=i;
}

void complex::Set()
{
	std::cout<<"Enter Real: ";
	std::cin>>this->real;
	std::cout<<"Enter Imaginary: ";
	std::cin>>this->imag;
}

long double complex::Mod()const
{
	return sqrt(real*real+imag*imag);
}

long double complex::Arg()const
{
	return atan(imag/real);
}

complex& complex::operator=(const complex& c)
{
	real=c.Re();
	imag=c.Im();
	return *this;
}


complex complex::operator+(const complex& c)const
{
	complex tmp(real+c.Re(),imag+c.Im());
	return tmp;
}

complex complex::operator-()const
{
	complex tmp(-real,-imag);
	return tmp;
}

complex complex::operator-(const complex& c)const
{
	return (*this)+(-c);
}

complex complex::operator*(const complex& c)const
{
	complex tmp(real*c.Re()-imag*c.Im(),real*c.Im()+imag*c.Re());
	return tmp;
}

complex complex::operator/(const complex& c)const
{
	long double div=(c.real*c.real) + (c.imag*c.imag);
	complex tmp;
	tmp.real=(real*c.real)+(imag*c.imag);
	tmp.real/=div;
	tmp.imag=(imag*c.real)-(real*c.imag);
	tmp.imag/=div;
	return tmp;
}

complex complex::operator~()const
{
	complex tmp(real,-imag);
	return tmp;
}

complex complex::operator^(int n)const
{
	complex tmp(1,0);
	if(n>=0)
		for(int i=0;i<n;i++)
			tmp=tmp*(*this);
	else
		for(int i=0;i>n;i--)
			tmp=tmp/(*this);
	return tmp;
}

bool complex::operator==(const complex& c)const
{
	return (real==c.Re())&&(imag==c.Im());
}

bool complex::operator!=(const complex& c)const
{
	return !((*this)==c);
}

std::ostream& operator<<(std::ostream &s,const complex& c)
{
	if(c.Im()<0)
		s<<c.Re()<<c.Im()<<"i";
	else
		s<<c.Re()<<"+"<<c.Im()<<"i";
	return s;
}

complex newComplex(long double r,long double i)
{
    complex tmp(r,i);
    return tmp;
}

complex expc(const complex& z){
    return newComplex(exp(z.Re())*cos(z.Im()),exp(z.Re())*sin(z.Im()));
}

complex cosc(const complex& z){
    return (newComplex(exp(-z.Im())*cos(z.Re()),exp(-z.Im())*sin(z.Re()))+newComplex(exp(z.Im())*cos(-z.Re()),exp(z.Im())*sin(-z.Re())))/newComplex(2.0,0.0);
}

complex sinc(const complex& z){
    return (newComplex(exp(-z.Im())*cos(z.Re()),exp(-z.Im())*sin(z.Re()))-newComplex(exp(z.Im())*cos(-z.Re()),exp(z.Im())*sin(-z.Re())))/newComplex(0.0,2.0);
}

complex tanc(const complex& z){
    return sinc(z)/cosc(z);
}

