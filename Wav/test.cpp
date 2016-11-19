#include<fstream>
#include<iostream>
#include<cstdlib>
#include<climits>
#include"../lib/Timer.h"
#include<fftw3.h>

int main(int argc, char *argv[]){

    Timer timer;

    std::ifstream file (argv[1] , std::ios::in | std::ios::binary);

    int dataSize;
    file.seekg(40);
    file.read((char*)&dataSize,4);
    dataSize/=2;

    short* sample = (short*)malloc(2*dataSize);
    file.read((char*)sample,2*dataSize);

    timer.start();

    fftw_complex *in = ( fftw_complex* ) fftw_malloc ( sizeof ( fftw_complex ) * dataSize );
    fftw_complex *out = ( fftw_complex* ) fftw_malloc ( sizeof ( fftw_complex ) * dataSize );

    for (int k = 1; k < dataSize; k++){
        in[k][0] = (double)sample[k]/SHRT_MAX;
        in[k][1] = 0;
    }

    fftw_plan p = fftw_plan_dft_1d(dataSize, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    
    fftw_execute(p);

    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    timer.stop();

    std::cout<<"dataSize: "<<dataSize<<"\n"<<timer.getElapsedTime()<<"s\n";

    return 0;
}
