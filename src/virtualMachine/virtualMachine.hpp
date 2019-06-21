#pragma once
#include <string>
#include <unordered_map>

class VirtualMachine {
public:
	VirtualMachine();
	~VirtualMachine();

	void load(std::string fileName);
	void run();

	struct instruction;
	static const std::unordered_map<uint8_t, instruction> instructions;

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

struct VirtualMachine::instruction {
	int size;
	void (VirtualMachine::*function)();
};

inline const std::unordered_map<uint8_t, VirtualMachine::instruction> VirtualMachine::instructions = {
	{ 0x00,	{ 2, } },	// Jump (UNCONDITIONAL)
	{ 0x01,	{ 2, } },	// Jump if zero
	{ 0x02,	{ 2, } },	// Jump if negative
	{ 0x03,	{ 1, } },	// Control
	{ 0x04,	{ 2, } },	// Add
	{ 0x05,	{ 2, } },	// Subtract
	{ 0x06,	{ 2, } },	// Multiply
	{ 0x07,	{ 2, } },	// Divide
	{ 0x08,	{ 2, } },	// Load from memory
	{ 0x09,	{ 2, } },	// Move to memory
	{ 0x0A,	{ 2, } },	// Subroutine call
	{ 0x0B,	{ 1, } },	// Operating system call
	{ 0x0C,	{ 1, } },	// Input/Output
	//{ ??,	{ 1, } },	// Free
	//{ ??,	{ 1, } },	// Free
	//{ ??,	{ 1, } },	// Free
};