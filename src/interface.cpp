#include "interface.h"
#include "assembler.h"
#include "virtualMachine.h"

#include <iostream>


Interface::Interface() {

}


Interface::~Interface() {

}


void Interface::start() {
	std::cout << "Welcome!" << std::endl;

	std::cout << "Menu:" << std::endl;
	std::cout << "1. Assembler" << std::endl;
	std::cout << "2. Virtual Machine" << std::endl;
	std::cout << "3. Exit" << std::endl;
	std::cout << "Type the number relative to the option:" << std::endl;

	int menu;
	std::cin >> menu;

	switch (menu) {
	case 1: {
		std::string fileName = "test.asm";

		Assembler assembler;
		assembler.run(fileName);

		std::cout << assembler.mnemonics[0].name << std::endl;
		break;
	}

	case 2: {
		virtualMachine vm;
		break;
	}

	default:
		break;
	}

	system("pause");
}
