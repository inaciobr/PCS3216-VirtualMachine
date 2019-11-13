/**
 * operatingSystem.cpp
 * PCS 3216 - Sistemas Operacionais - 2019
 * Bruno Brand�o In�cio
 */

#include "operatingSystem.hpp"

#include <algorithm>

#include <random>
#include <iostream>
#include <iomanip>
#include <string>


OperatingSystem::OperatingSystem()
    : processor(nullptr),
      memory(nullptr),
      disk(nullptr) {
}


/**
 * Adiciona job ao sistema operacional.
 */
PredictedEvent OperatingSystem::addJob(Job &&j) {
    this->jobs[j.id] = std::make_shared<Job>(j);
    return { j.id, 0, Event::MEM_ALLOC };
}


/**
 * Tenta alocar um job para a mem�ria principal do sistema.
 */
PredictedEvent OperatingSystem::memAlloc(int jobID) {
    auto job = this->jobs.at(jobID);

    try {
        this->memory->allocate(jobID, job->memoryUsed);
        return { jobID, 0, Event::CPU_RUN };
    }
    catch (Error) {
        this->waitMemory(jobID);
    }

    return { jobID, 0, Event::NONE };
}


/**
 * Tenta iniciar o processamento de um job.
 */
PredictedEvent OperatingSystem::process(int jobID) {
    auto job = this->jobs.at(jobID);

    try {
        return this->processor->run(job, 1);
    }
    catch (Error) {
        this->waitProcessor(jobID);
    }

    return { jobID, 0, Event::NONE };
}


void OperatingSystem::waitMemory(int jobID) {

}


void OperatingSystem::waitProcessor(int jobID) {

}


void OperatingSystem::waitDisk(int jobID) {

}


/**
 * Adiciona o processador que ser� utilizado pelo sistema.
 */
void OperatingSystem::addProcessor(Processor&& p) {
    this->processor = std::make_unique<Processor>(p);
}


/**
 * Adiciona a mem�ria que ser� utilizada pelo sistema.
 */
void OperatingSystem::addMemory(Memory&& m) {
    this->memory = std::make_unique<Memory>(m);
}


/**
 * Adiciona o dsico que ser� utilizado pelo sistema.
 */
void OperatingSystem::addDisk(Disk&& d) {
    this->disk = std::make_unique<Disk>(d);
}


/**
 * Exibe informa��o sobre todos os jobs no sistema.
 */
void OperatingSystem::info() {
    std::cout << "=== JOBS ===" << std::endl;
    std::cout << " ID | PRIORITY |            STATUS | TIME NEEDED | MEMORY NEEDED" << std::endl;

    for (const auto& [jobID, job] : this->jobs)
        std::cout << std::setw(3) << job->id
        << std::setw(11) << Translate::priority.at(job->priority)
        << std::setw(20) << Translate::state.at(job->state)
        << std::setw(14) << job->totalTime
        << std::setw(16) << job->memoryUsed
        << std::endl;
}


/**
 * Exibe informa��es sobre os dispostivos de hardware associados ao sistema.
 */
void OperatingSystem::infoHardware() {
    std::cout << "=== Dispositivos ===" << std::endl;

    this->processor->info();
    this->memory->info();
    this->disk->info();
}




/**
 * Define um job com valores estoc�sticos baseados em uma distribui��o
 * estat�stica usual dos jobs.
 */
 /*std::tuple<int, Job> OperatingSystem::stochasticJob(int maxTime) {
     std::random_device r;

     int time = 0;

     return std::make_tuple(time, Job(10, 20, Priority::NORMAL));
 }*/
