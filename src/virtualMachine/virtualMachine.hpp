#pragma once
#include <string>

class VirtualMachine {
public:
	VirtualMachine();
	~VirtualMachine();

	void load(std::string fileName);
	void run();

private:

	enum instructions : uint8_t;
};

enum VirtualMachine::instructions : uint8_t {
		JP		= 0x00,
		JZ		= 0x10,
		JN		= 0x20,
		CN		= 0x30,
		ADD		= 0x40,
		SUB		= 0x50,
		MUL		= 0x60,
		DIV		= 0x70,
		LD		= 0x80,
		MM		= 0x90,
		SC		= 0xA0,
		OS		= 0xB0,
		IO		= 0xC0
};