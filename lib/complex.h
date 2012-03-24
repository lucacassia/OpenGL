#ifndef COMPLEX
#define COMPLEX

#include <cmath>
#include <iostream>

#define Pi (4*atan(1))

class complex
{
	public:
		long double real;
		long double imag;
		complex(long double=0,long double=0);
		long double Re()const;
		long double Im()const;
		complex(const complex&);
		void Set(long double,long double);
		void Set();
		long double Mod()const;
		long double Arg()const;
		complex& operator=(const complex&);
		complex operator+(const complex&)const;
		complex operator-(void)const;
		complex operator-(const complex&)const;
		complex operator*(const complex&)const;
		complex operator/(const complex&)const;
		complex operator~(void)const;
		complex operator^(int)const;
		bool operator==(const complex&)const;
		bool operator!=(const complex&)const;
		friend std::ostream& operator<<(std::ostream&,const complex&);
};

complex newComplex(long double=0,long double=0);
complex expc(complex);
complex cosc(complex);
complex sinc(complex);

#endif
