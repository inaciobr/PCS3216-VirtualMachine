#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "memory.hpp"

class VirtualMachine {
public:
	VirtualMachine(unsigned int numBanks = 1 << 4, unsigned int bankSize = 1 << 12);
	~VirtualMachine() {};

	void load(std::string fileName);
	void run();

	static const std::unordered_map<uint8_t, void (VirtualMachine::*)()> instructions;

	void printPointers() const;
	void printMemoryBanks() const;
	void printMemory(int addr, int size) const;

private:
	uint16_t instructionCounter;
	uint8_t currentByte;
	uint8_t currentBank;
	int8_t accumulator;
	bool indirectMode;

	std::vector<memory> mainMemory;

	uint8_t nextByte();
	uint8_t getIndirectValue(uint16_t pos);
	uint16_t getOperand2Bytes();

	void updatePC(uint16_t operand);
	void runInstruction(uint8_t type);

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


inline const std::unordered_map<uint8_t, void (VirtualMachine::*)()> VirtualMachine::instructions = {
	{ 0x00,	&VirtualMachine::JP	 },	// Jump (UNCONDITIONAL)
	{ 0x10,	&VirtualMachine::JZ	 },	// Jump if zero
	{ 0x20,	&VirtualMachine::JN	 },	// Jump if negative
	{ 0x30,	&VirtualMachine::CN	 },	// Control
	{ 0x40,	&VirtualMachine::ADD },	// Add
	{ 0x50,	&VirtualMachine::SUB },	// Subtract
	{ 0x60,	&VirtualMachine::MUL },	// Multiply
	{ 0x70,	&VirtualMachine::DIV },	// Divide
	{ 0x80,	&VirtualMachine::LD	 },	// Load from memory
	{ 0x90,	&VirtualMachine::MM	 },	// Move to memory
	{ 0xA0,	&VirtualMachine::SC	 },	// Subroutine call
	{ 0xB0,	&VirtualMachine::OS	 },	// Operating system call
	{ 0xC0,	&VirtualMachine::IO	 },	// Input/Output
	//{ ??,	&VirtualMachine::f	 },	// Free
	//{ ??,	&VirtualMachine::f	 },	// Free
	//{ ??,	&VirtualMachine::f	 },	// Free
};
