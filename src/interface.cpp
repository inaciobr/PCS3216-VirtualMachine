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
	int menu;

	while (true) {
		std::cout << std::endl;
		std::cout << "1. Assembler" << std::endl;
		std::cout << "2. Virtual Machine" << std::endl;
		std::cout << "3. Exit" << std::endl;
		std::cout << "\nEnter the number of an option: ";
		std::cin >> menu;

		switch (menu) {
		case 1:	// Assembler
			this->assemblerMenu();
			break;

		case 2:	// Virtual Machine
			this->virtualMachineMenu();
			break;

		default: // Exit
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
	std::string fileName = "test.asm";

	Assembler assembler;
	assembler.assemble(fileName);

	return;
}

void Interface::virtualMachineMenu() {
	std::string fileName = this->inputFile();

	VirtualMachine VM;
	VM.load(fileName);
	VM.run();

	return;
}
