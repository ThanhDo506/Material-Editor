#pragma once

class Time
{
public:
    static Time& instance();
    double deltaTime() const;
    double time() const;

private:
    Time(const Time& obj)           = delete;
    void operator=(const Time& obj) = delete;
    Time();
    double _timeStep = 1.0 / 60.0;
    double _time;
    double _deltaTime;
    double _lastUpdatedTime;

    friend class Application;
};
