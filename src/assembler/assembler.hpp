/**
 * assembler.hpp
 * PCS 3216 - Sistemas de Programa��o - 2019
 * Bruno Brand�o In�cio
 */

#pragma once

#include <string>
#include <unordered_map>

#include "label.hpp"
#include "codeList.hpp"


class Assembler {
public:
	Assembler(std::string fileName) : inputFile(fileName) {};
	~Assembler() {};

	void assemble();

	struct Instruction;
	static const std::unordered_map<std::string, Instruction> mnemonics;

private:
	void firstPass();
	void secondPass();

	int operandValue(std::string operand, bool allowLabel, bool evaluateLabel);
	
	void makeCode(std::string outputFile, void (*flush)(std::string, uint8_t[]));
	static void flushBin(std::string outputFile, uint8_t code[]);
	static void flushHex(std::string outputFile, uint8_t code[]);

	std::string inputFile;

	Label labels;
	CodeList list;
};


/**
 *  Estrutura para a defini��o de instru��es e pseudo-instru��es.
 */
struct Assembler::Instruction {
	unsigned int code;
	unsigned int size;
	unsigned int mask;
	bool allowLabel;
};


/**
 *  Tabela de mnem�nicos.
 */
inline const std::unordered_map<std::string, Assembler::Instruction> Assembler::mnemonics = {
	// Instru��es
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

	// Pseudo-Instru��es
	{ "K",	{ 0x00,		1,	0xFF,	false	} },	// DB
	{ "$",	{ 0x00,		0,	0x00,	false	} },	// BLOC
	{ "@",	{ 0x0000,	0,	0xFFFF,	false	} },	// ORG
	{ "#",	{ 0x0000,	0,	0xFFFF,	true	} },	// END
};
