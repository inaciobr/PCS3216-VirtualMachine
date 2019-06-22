/**
* interface.cpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#include "interface.hpp"
#include "../assembler/assembler.hpp"
#include "../virtualMachine/virtualMachine.hpp"

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
 * Cria o menu para o usuário.
*/
void Interface::menu() {
	int menu;
	enum options { EXIT, ASSEMBLER, VIRTUALMACHINE };

	std::cout << std::endl;
	std::cout << EXIT << ". Sair" << std::endl;
	std::cout << ASSEMBLER << ". Assembler" << std::endl;
	std::cout << VIRTUALMACHINE << ". Virtual Machine" << std::endl;
	std::cout << "\nDigite o numero da opcao desejada: ";

		
	switch (std::cin >> menu; menu) {
	case ASSEMBLER:
		this->assemblerMenu();
		break;

	case VIRTUALMACHINE:
		this->virtualMachineMenu();
		break;

	case EXIT:
	default:
		break;
	}

	return;
}


/**
 * Inicializa o assembler.
 */
void Interface::assemblerMenu() {
	std::string fileName = this->inputFile();

	Assembler assembler(fileName);
	std::string binFile;

	try {
		assembler.assemble();
	}
	catch (std::string e) {
		std::cout << e << std::endl;
		return;
	}

	std::cout << "\nO codigo foi montado com sucesso." << std::endl;
	return;
}


/**
 * Inicializa a máquina virtual.
 */
void Interface::virtualMachineMenu() {
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
			break;

		case READMEMORY:
			break;

		case READPOINTERS:
			VM.printPointers();
			break;

		case EXIT:
		default:
			return;
		}

		this->virtualMachineMenu();
		break;
	}
	//std::string fileName = this->inputFile();

	//VM.load(fileName);
	try {
		VM.run();
	}
	catch (std::string e) {
		std::cout << e << std::endl;
		return;
	}

	return;
}


/**
 * Controla a entrada de um arquivo.
 */
std::string Interface::inputFile() {
	std::string fileName;

	while (true) {
		std::cout << "Digite o nome do arquivo (dentro da pasta code): ";
		std::cin >> fileName;
		fileName = "code/" + fileName;

		if (std::ifstream(fileName))
			break;

		std::cout << "\nO arquivo nao pode ser aberto." << std::endl;
	}

	return fileName;
}
