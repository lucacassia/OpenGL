#include<cmath>
#include<vector>
#include<iostream>
#include"complex.h"

#define PI 3.14159265

// compute the FFT of x[], assuming its length is a power of 2
std::vector<complex> fft(std::vector<complex> x)
{
    int N = x.size();

    // base case
    if (N == 1){
        std::vector<complex> tmp(x);
        return tmp;
    }

    // radix 2 Cooley-Tukey FFT
    try { if (N % 2 != 0) throw "N is not a power of 2"; }
    catch (const char* e) { std::cout << e << std::endl; }

    // fft of even terms
    std::vector<complex> even;
    for (int k = 0; k < N/2; k++) {
        even.push_back(x[2*k]);
    }
    std::vector<complex> q(fft(even));

    // fft of odd terms
    std::vector<complex> odd;
    for (int k = 0; k < N/2; k++) {
        odd.push_back(x[2*k + 1]);
    }
    std::vector<complex> r(fft(odd));

    // combine
    std::vector<complex> y(N,newComplex());
    for (int k = 0; k < N/2; k++) {
        double kth = -2 * k * PI / N;
        complex wk(cos(kth), sin(kth));
        y[k]       = q[k] + wk * r[k];
        y[k + N/2] = q[k] - wk * r[k];
    }
    return y;
}

