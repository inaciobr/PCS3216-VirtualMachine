/**
 * processor.cpp
 * PCS 3216 - Sistemas Operacionais - 2019
 * Bruno Brand�o In�cio
 */

#include "processor.hpp"


/**
 * Inicia a execu��o de um 'job' e informa a pr�xima opera��o.
 * Retorna uma tupla referente ao pr�ximo evento.
 */
std::tuple<int, Event, int> Processor::run(Job *job, int time) {
	if (this->isRunning)
		throw Error::CPU_UNAVAILABLE;
	
	this->time = time;
	this->job = job;
	this->isRunning = true;

	std::tuple<int, Event, double> nextEvent;

	switch (auto [operation, timeUntil] = this->job->getNextOperation(); operation) {
	case Job::Operation::IO_READ:
	case Job::Operation::IO_WRITE:
	case Job::Operation::FINISH:
		nextEvent = std::make_tuple(this->job->id, Event::CPU_RELEASE, timeUntil);
		break;

	default:
		throw "Opera��o n�o reconhecida na CPU.";
	}

	return nextEvent;
}


/**
 * Libera o uso da CPU e atualiza o 'job' para o ponto onde a CPU
 * foi liberada. Este ponto deve ser referente ao ponto da pr�xima
 * opera��o onde � previsto que o 'job' seja interrompido.
 * Outros tipos de paradas devem ser tratadas como interrup��es.
 */
std::tuple<int, Event, int> Processor::release(int time) {
	if (!isRunning)
		throw "Tentativa de parar a CPU quando ela j� estava parada.";

	std::tuple<int, Event, int> nextEvent = std::make_tuple(this->job->id, Event::CPU_RUN, 0);

	if (auto [operation, duration] = this->job->getNextOperation(); duration == time - this->time) {
		switch (operation) {
		case Job::Operation::IO_READ:
			nextEvent = std::make_tuple(this->job->id, Event::IO_START_READ, 0);
			break;

		case Job::Operation::IO_WRITE:
			nextEvent = std::make_tuple(this->job->id, Event::IO_START_WRITE, 0);
			break;

		case Job::Operation::FINISH:
			nextEvent = std::make_tuple(this->job->id, Event::MEM_FREE, 0);
			break;

		default:
			throw "Opera��o de parada de processo n�o reconhecida na CPU.";
		}
	}

	this->job->process(time - this->time);

	this->job = nullptr;
	this->time = time;
	this->isRunning = false;

	return nextEvent;
}
