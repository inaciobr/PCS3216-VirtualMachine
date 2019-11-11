
/**
 * processor.hpp
 * PCS 3216 - Sistemas Operacionais - 2019
 * Bruno Brand�o In�cio
 */

#pragma once

#include "event.hpp"
#include "job.hpp"

#include <tuple>

class Processor {
public:
    Processor() : job(nullptr), isRunning(0), time(0) {};
    ~Processor() {};

    PredictedEvent run(Job *job, int time);
    PredictedEvent release(int time);

    void info();

private:
    Job *job;
    bool isRunning;
    int time;
};
