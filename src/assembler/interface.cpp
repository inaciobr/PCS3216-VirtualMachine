/**
 * interface.cpp
 * PCS 3216 - Sistemas de Programação - 2019
 * Bruno Brandão Inácio
 */

#include "interface.hpp"
#include "assembler.hpp"

#include <iostream>
#include <fstream>


/**
 * Inicia a interface.
 */
void Interface::start() {
	std::cout << "Bem vindo ao assembler!" << std::endl;
	this->menu();

	return;
}


/**
 * Inicializa o assembler.
 */
void Interface::menu() {
	try {
		std::string fileName = this->inputFile();

		Assembler assembler(fileName);
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
 * Controla a entrada de um arquivo.
 */
std::string Interface::inputFile() {
	std::string fileName;

	std::cout << "Digite o nome do arquivo (dentro da pasta programs): ";
	std::cin >> fileName;
	fileName = "programs/" + fileName;

	if (!std::ifstream(fileName))
		throw std::string("\nO arquivo nao pode ser aberto.\n");

	return fileName;
}


/**
 * Realiza a chamada para a interface.
 */
int main() {
	Interface pc;
	pc.start();

	return 0;
}
