#pragma once
#include <string>

class VirtualMachine {
public:
	VirtualMachine();
	~VirtualMachine();

	void load(std::string fileName);
	void run();

private:
	void JP();
	void JZ();
	void JN();
	void CN();
	void ADD();
	void SUB();
	void MUL();
	void DIV();
	void LD();
	void MM();
	void SC();
	void OS();
	void IO();
};
