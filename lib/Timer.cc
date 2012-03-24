#include "Timer.h"
#include <cstdlib>

Timer::Timer()
{
    startCount.tv_sec = startCount.tv_usec = 0;
    endCount.tv_sec = endCount.tv_usec = 0;
    stopped = 0;
    startTimeInMicroSec = 0;
    endTimeInMicroSec = 0;
}

void Timer::start()
{
    stopped = 0;
    gettimeofday(&startCount, NULL);
}

void Timer::stop()
{
    stopped = 1;
    gettimeofday(&endCount, NULL);
}

double Timer::getElapsedTimeInMicroSec()
{
    if(!stopped)
        gettimeofday(&endCount, NULL);

    startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
    endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
    return endTimeInMicroSec - startTimeInMicroSec;
}

double Timer::getElapsedTimeInMilliSec()
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}

double Timer::getElapsedTimeInSec()
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}

double Timer::getElapsedTime()
{
    return this->getElapsedTimeInSec();
}

