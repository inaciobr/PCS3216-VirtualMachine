#pragma once
#include <string>

class VirtualMachine {
public:
	VirtualMachine();
	~VirtualMachine();

	void load(std::string fileName);
	void run();
};
