/**
* interface.cpp
* PCS 3216 - Sistemas de Programa��o - 2019
* Bruno Brand�o In�cio
*/

#include "interface.hpp"
#include "assembler/assembler.hpp"
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
 * Cria o menu para o usu�rio.
*/
void Interface::menu() {
	int menu;
	enum options { EXIT, ASSEMBLER, VIRTUALMACHINE };

	while (true) {
		std::cout << std::endl;
		std::cout << EXIT << ". Sair" << std::endl;
		std::cout << ASSEMBLER << ". Assembler" << std::endl;
		std::cout << VIRTUALMACHINE << ". Virtual Machine" << std::endl;
		std::cout << "\nDigite o n�mero da op��o desejada: ";

		/*
		switch (std::cin >> menu; menu) {
		case ASSEMBLER:
			this->assemblerMenu();
			break;

		case VIRTUALMACHINE:
			this->virtualMachineMenu();
			break;

		case EXIT:
		default:
			return;
		}
		*/
		this->assemblerMenu();
		break;
	}

	return;
}


/**
 * Inicializa o assembler.
 */
void Interface::assemblerMenu() {
	//std::string fileName = this->inputFile();
	std::string fileName = "code/test.asm";

	Assembler assembler(fileName);
	std::string binFile;

	try {
		binFile = assembler.assemble();
	}
	catch (std::string e) {
		std::cout << e << std::endl;
		return;
	}

	std::cout << "\nO c�digo foi montado no arquivo " << binFile << " com sucesso." << std::endl;
	return;
}


/**
 * Inicializa a m�quina virtual.
 */
void Interface::virtualMachineMenu() {
	std::string fileName = this->inputFile();

	VirtualMachine VM;
	VM.load(fileName);
	VM.run();

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

		std::cout << "\nO arquivo n�o pode ser aberto." << std::endl;
	}

	return fileName;
}
