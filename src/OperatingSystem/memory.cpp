/**
 * memory.cpp
 * PCS 3216 - Sistemas Operacionais - 2019
 * Bruno Brand�o In�cio
 */

#include "memory.hpp"
#include "event.hpp"

#include <iostream>
#include <iomanip>

/**
 * Realiza a aloca��o do job referente ao 'id' e que possui tamanho 'size'.
 * Se a mem�ria n�o possuir espa�o livre o suficiente ser� lan�ada uma exce��o para o SO.
 */
void Memory::allocate(int jobID, int size) {
	if (size > this->totalSpace - this->usedSpace)
		throw Error::MEMORY_FULL;

	this->usedSpace += size;
	this->mem.insert({ jobID, size });
}


/**
 * Remove o job 'id' da mem�ria e libera o espa�o utilizado.
 */
void Memory::free(int jobID) {
	this->usedSpace -= this->mem.at(jobID);
	this->mem.erase(jobID);
}


/**
 * Exibe o estado atual da mem�ria.
 */
void Memory::log() {
	std::cout << "=== Memoria Principal ===" << std::endl;
	std::cout << std::setw(4) << this->usedSpace << "/" << this->totalSpace
		<< " MB em uso\n" << std::endl;
	std::cout << "  ID | Memoria em uso" << std::endl;

	for (const auto& process : this->mem)
		std::cout << std::setw(4) << process.first << std::setw(14) << process.second
		<< " MB" << std::endl;
}


/**
 * Retorna um unordered_map com as informa��es referentes � mem�ria.
 */
std::unordered_map<int, int> Memory::getInfo() const {
	return this->mem;
}
