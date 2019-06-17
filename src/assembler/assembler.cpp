/**
* assembler.cpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#include "assembler.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

/**
 *  Tabela de mnemônicos.
 */
const std::map<std::string, Assembler::Instruction> Assembler::mnemonics = {
	{"JP", {0x0, 2}},		// Jump (UNCONDITIONAL)
	{"JZ", {0x1, 2}},		// Jump if zero
	{"JN", {0x2, 2}},		// Jump if negative
	{"CN", {0x3, 1}},		// Control
	{ "+", {0x4, 2}},		// Add
	{ "-", {0x5, 2}},		// Subtract
	{ "*", {0x6, 2}},		// Multiply
	{ "/", {0x7, 2}},		// Divide
	{"LD", {0x8, 2}},		// Load from memory
	{"MM", {0x9, 2}},		// Move to memory
	{"SC", {0xA, 2}},		// Subroutine call
	{"OS", {0xB, 1}},		// Operating system call
	{"IO", {0xC, 1}}		// Input/Output
	//{??, {0xD, 1}},		// Free
	//{??, {0xE, 1}},		// Free
	//{??, {0xF, 1}}		// Free
};


/**
* TODO
*/
Assembler::Assembler(std::string fileName) {
	this->inputFile = fileName;
	this->outputFile = fileName.substr(0, fileName.find_last_of('.')) + ".bin";
}


/**
* TODO
*/
Assembler::~Assembler() {

}


/**
* TODO
*/
std::string Assembler::assemble() {
	//this->createListFile();

	this->runStep(0);
	this->labels.checkIntegrity();
	//this->runStep(1);

	this->labels.dump(this->inputFile + ".labels");
	//this->makeObject();

	return this->outputFile;
}


/**
* TODO
*/
void Assembler::runStep(bool step) {
	std::ifstream assemblyFile(this->inputFile);
	unsigned instructionCounter = 0;
	unsigned lineNumber = 0;

	for (std::string line; std::getline(assemblyFile, line); ) {
		std::string command = line.substr(0, line.find_last_of(';'));
		auto pos = command.find_first_not_of(" \t\n");
		lineNumber++;

		// Linha sem comando.
		if (pos == std::string::npos) {
			if (step)
				this->list.addToList({ lineNumber, ListCode::UNDEFINED, ListCode::UNDEFINED, line });

			continue;
		}

		// Leitura dos commandos.
		std::istringstream sline(line);
		std::string label, mnemonic, operand;

		// Definição de label.
		if (pos == 0) { 
			sline >> label;

			if (!step)
				this->labels.define(label, instructionCounter);
		}

		sline >> mnemonic >> operand;

		if (!mnemonic.length())
			continue;

		if (!operand.length())
			throw "\n" + std::to_string(lineNumber) + ": operando não definido.";

		// Instruções e pseudo-instruções.
		// ======
		// VERIFICAR PSEUDO
		try {
			auto opr = Assembler::mnemonics.at(mnemonic);
			instructionCounter += opr.size;
		}
		catch (const std::out_of_range& oor) {
			throw "\n" + std::to_string(lineNumber) + ": O mnemônico " + mnemonic + " não foi reconhecido.";
		}

		// VERIFICA OPERANDO


	}

	assemblyFile.close();

	return;
}


/**
* TODO
*/
void Assembler::makeObject() {

}
