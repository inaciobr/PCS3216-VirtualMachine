/**
* interface.hpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#pragma once
#include <string>

struct Interface {
	void start();

	void menu();
	void assemblerMenu();
	void virtualMachineMenu();

	static std::string inputFile();
};
