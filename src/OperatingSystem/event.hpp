/**
 * event.hpp
 * PCS 3216 - Sistemas Operacionais - 2019
 * Bruno Brand�o In�cio
 */

#pragma once

class EventList {
public:

};


enum class Event {
    // Mem�ria
    MEM_ALLOC,
    MEM_FREE,

    // Disco
    IO_START_READ,
    IO_START_WRITE,
    IO_COMPLETE,

    // Processor
    CPU_RUN,
    CPU_RELEASE,
    CPU_DONE,

    CPU_INTERRUPT,
};

enum class Error {
    // Mem�ria
    MEMORY_FULL,

    // Disco
    DISK_UNAVAILABLE,

    // Processador
    CPU_UNAVAILABLE,
};

struct predictEvent {
    int jobID;
    double duration;
    Event event;
};
