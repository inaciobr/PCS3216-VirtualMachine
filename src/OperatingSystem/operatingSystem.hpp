/**
 * operatingSystem.hpp
 * PCS 3216 - Sistemas Operacionais - 2019
 * Bruno Brand�o In�cio
 */

#pragma once

class OperatingSystem {
public:
    OperatingSystem() {};
    ~OperatingSystem() {};

    void killJob(int id);
    void printJobs();
};
