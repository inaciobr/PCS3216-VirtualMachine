#include "assembler.hpp"

#include <iostream>
#include <fstream>


const std::map<const char*, unsigned int> Assembler::mnemonics = {
	{"JP", 0x0000},		// Jump (UNCONDITIONAL)
	{"JZ", 0x1000},		// Jump if zero
	{"JN", 0x2000},		// Jump if negative
	{"CN", 0x3000},		// Control
	{ "+", 0x4000},		// Add
	{ "-", 0x5000},		// Subtract
	{ "*", 0x6000},		// Multiply
	{ "/", 0x7000},		// Divide
	{"LD", 0x8000},		// Load from memory
	{"MM", 0x9000},		// Move to memory
	{"SC", 0xA000},		// Subroutine call
	{"OS", 0xB000},		// Operating system call
	{"IO", 0xC000}		// Input/Output
	//{??, 0xD000},		// Free
	//{??, 0xE000},		// Free
	//{??, 0xF000}		// Free
};

Assembler::Assembler() {

} 

Assembler::~Assembler() {

}

void Assembler::assemble(std::string fileName) {
	std::cout << this->mnemonics.at("MM") << std::endl;
}
