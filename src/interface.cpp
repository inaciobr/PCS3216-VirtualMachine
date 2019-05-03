#include "interface.hpp"
#include "assembler.hpp"
#include "virtualMachine.hpp"

#include <iostream>
#include <fstream>


void Interface::start() {
	std::cout << "Welcome!" << std::endl;
	this->menu();
}

void Interface::menu() {
	enum options { EXIT, ASSEMBLER, VIRTUALMACHINE };
	int menu;

	while (true) {
		std::cout << std::endl;
		std::cout << EXIT << ". Exit" << std::endl;
		std::cout << ASSEMBLER << ". Assembler" << std::endl;
		std::cout << VIRTUALMACHINE << ". Virtual Machine" << std::endl;
		std::cout << "\nEnter the number of an option: ";
		std::cin >> menu;

		switch (menu) {
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
	}
}

std::string Interface::inputFile() {
	std::string fileName;

	while (true) {
		std::cout << "Enter the file name: ";
		std::cin >> fileName;
		std::cout << std::endl;

		if (std::ifstream(fileName))
			break;

		std::cout << "File can't be opened." << std::endl;
	}

	return fileName;
}

void Interface::assemblerMenu() {
	//std::string fileName = this->inputFile();
	std::string fileName = "assembly/test.asm";

	Assembler assembler(fileName);
	assembler.assemble();

	return;
}

void Interface::virtualMachineMenu() {
	std::string fileName = this->inputFile();

	VirtualMachine VM;
	VM.load(fileName);
	VM.run();

	return;
}
