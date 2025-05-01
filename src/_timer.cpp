#include "_timer.h"

_timer::_timer()
{
    //ctor

}

_timer::~_timer()
{
    //dtor
}

clock_t _timer::getTicks()
{
    return clock()-startTime;
}

void _timer::pause()
{

}

void _timer::resume()
{

}

void _timer::stop()
{

}

void _timer::reset()
{
    startTime = clock();
}
