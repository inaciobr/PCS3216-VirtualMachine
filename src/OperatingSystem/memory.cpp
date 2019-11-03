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
void Memory::allocate(int id, int size) {
	if (size > this->totalSpace - this->usedSpace)
		throw Event::Error::MEMORY_FULL;

	this->usedSpace += size;
	this->mem.insert({ id, size });
}


/**
 * Remove o job 'id' da mem�ria e libera o espa�o utilizado.
 */
void Memory::free(int id) {
	this->usedSpace -= this->mem.at(id);
	this->mem.erase(id);
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