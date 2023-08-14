#include "Time.h"

Time& time = Time::instance();

Time::Time()
{
}

Time& Time::instance()
{
    static Time instance;
    return instance;
}

double Time::deltaTime() const
{
    return _deltaTime;
}

double Time::time() const
{
    return _time;
}
