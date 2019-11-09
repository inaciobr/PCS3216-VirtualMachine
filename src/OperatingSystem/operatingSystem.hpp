/**
 * operatingSystem.hpp
 * PCS 3216 - Sistemas Operacionais - 2019
 * Bruno Brand�o In�cio
 */

#pragma once

#include <memory>

#include "processor.hpp"
#include "memory.hpp"
#include "disk.hpp"

class OperatingSystem {
public:
    OperatingSystem();
    ~OperatingSystem() {};

    void addProcessor(Processor &&p);
    void addMemory(Memory &&m);
    void addDisk(Disk &&d);

    void info();

private:
    std::unique_ptr<Processor> processor;
    std::unique_ptr<Memory> memory;
    std::unique_ptr<Disk> disk;
};
