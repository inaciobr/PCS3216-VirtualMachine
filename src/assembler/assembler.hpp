/**
* assembler.hpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#include "label.hpp"
#include "codeList.hpp"


class Assembler {
public:
	Assembler(std::string fileName) : inputFile(fileName) {};
	~Assembler() {};

	struct Instruction;
	struct processedInstruction;
	struct Line;

	void assemble();
	static const std::unordered_map<std::string, Instruction> mnemonics;

private:
	void runStep(bool step);
	void saveObject();

	processedInstruction processInstruction(Assembler::Line lineValues, unsigned int instructionCounter, bool step);
	int operandValue(std::string, bool step, bool allowLabel = true);
	
	void makeObject(std::string outputFile);
	void makeBin(std::string outputFile);

	std::string inputFile;

	Label labels;
	CodeList list;

	std::vector<uint8_t> code;

};

struct Assembler::Line {
	Line(std::string text, unsigned position);

	std::string text;
	unsigned int position;

	std::string label;
	std::string mnemonic;
	std::string operand;
};

/**
 *  Estrutura para a definição de instruções e pseudo-instruções.
 */
struct Assembler::Instruction {
	unsigned int code;
	unsigned int size;
	unsigned int mask;
	bool allowLabel;
};

struct Assembler::processedInstruction {
	unsigned int nextInstruction;
	unsigned int size;
	unsigned int code;
};

/**
 *  Tabela de mnemônicos.
 */
inline const std::unordered_map<std::string, Assembler::Instruction> Assembler::mnemonics = {
	// Instruções
	{ "JP",	{ 0x0000,	2,	0x0FFF,	true	} },	// Jump (UNCONDITIONAL)
	{ "JZ",	{ 0x1000,	2,	0x0FFF,	true	} },	// Jump if zero
	{ "JN",	{ 0x2000,	2,	0x0FFF,	true	} },	// Jump if negative
	{ "CN",	{ 0x30,		1,	0x0F,	false	} },	// Control
	{  "+",	{ 0x4000,	2,	0x0FFF,	true	} },	// Add
	{  "-",	{ 0x5000,	2,	0x0FFF,	true	} },	// Subtract
	{  "*",	{ 0x6000,	2,	0x0FFF,	true	} },	// Multiply
	{  "/",	{ 0x7000,	2,	0x0FFF,	true	} },	// Divide
	{ "LD",	{ 0x8000,	2,	0x0FFF,	true	} },	// Load from memory
	{ "MM",	{ 0x9000,	2,	0x0FFF,	true	} },	// Move to memory
	{ "SC",	{ 0xA000,	2,	0x0FFF,	true	} },	// Subroutine call
	{ "OS",	{ 0xB0,		1,	0x0F,	false	} },	// Operating system call
	{ "IO",	{ 0xC0,		1,	0x0F,	false	} },	// Input/Output
	//{ ??,	{ 0xD,		1,	0x0F,	true	} },	// Free
	//{ ??,	{ 0xE,		1,	0x0F,	true	} },	// Free
	//{ ??,	{ 0xF,		1,	0x0F,	true	} },	// Free

	// Pseudo-Instruções
	{ "K",	{ 0x00,		1,	0xFF,	false	} },	// DB
	{ "$",	{ 0x00,		0,	0x00,	false	} },	// BLOC
	{ "@",	{ 0x0000,	0,	0xFFFF,	false	} },	// ORG
	{ "#",	{ 0x0000,	0,	0x0000,	true	} },	// END
};
