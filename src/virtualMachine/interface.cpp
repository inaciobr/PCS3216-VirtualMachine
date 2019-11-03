/**
* interface.cpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#include "interface.hpp"
#include "virtualMachine.hpp"

#include <iostream>
#include <fstream>

/**
 * Inicia a interface.
 */
void Interface::start() {
	std::cout << "Bem vindo!" << std::endl;
	this->menu();

	return;
}


/**
 * Inicializa a máquina virtual.
 */
void Interface::menu() {
	VirtualMachine VM;

	std::cout << "\nA Maquina Virtual esta em execucao" << std::endl;
	VM.printMemoryBanks();

	int menu;
	enum options { EXIT, RUN, READMEMORY, READPOINTERS };

	while (true) {
		std::cout << std::endl;
		std::cout << EXIT << ". Sair" << std::endl;
		std::cout << RUN << ". Executar um programa" << std::endl;
		std::cout << READMEMORY << ". Ler memoria" << std::endl;
		std::cout << READPOINTERS << ". Ler ponteiros" << std::endl;
		std::cout << "\nDigite o numero da opcao desejada: ";

		switch (std::cin >> menu; menu) {
		case RUN:
			//std::string fileName = this->inputFile();

			//VM.load(fileName);
			try {
				VM.run();
			}
			catch (std::string e) {
				std::cout << e << std::endl;
				return;
			}

		case READMEMORY: {
			int addr, size;

			std::cout << "Digite o endereco que deseja ler: ";
			std::cin >> addr;
			std::cout << "Digite a quantidade de valores que deseja ler: ";
			std::cin >> size;

			VM.printMemory(addr, size);
			break;
		}

		case READPOINTERS:
			VM.printPointers();
			break;

		case EXIT:
		default:
			return;
		}
	}

	return;
}


/**
 * Controla a entrada de um arquivo.
 */
std::string Interface::inputFile() {
	std::string fileName;

	while (true) {
		std::cout << "Digite o nome do arquivo (dentro da pasta programs): ";
		std::cin >> fileName;
		fileName = "programs/" + fileName;

		if (std::ifstream(fileName))
			break;

		std::cout << "\nO arquivo nao pode ser aberto." << std::endl;
	}

	return fileName;
}

int main() {
	Interface pc;
	pc.start();

	return 0;
}
