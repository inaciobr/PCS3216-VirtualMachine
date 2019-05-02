#pragma once
#include <string>

struct Interface {
	void start();
	void menu();
	std::string inputFile();

	void assemblerMenu();
	void virtualMachineMenu();
};
