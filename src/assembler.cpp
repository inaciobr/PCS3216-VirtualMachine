/**
* assembler.cpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#include "assembler.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>


/**
 *  Tabela de mnemônicos.
 */
const std::map<const char*, Assembler::instruction> Assembler::mnemonics = {
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
 *  
 */
Assembler::Assembler(std::string fileName) {
	this->inputFile = fileName;
	this->outputFile = fileName.substr(0, fileName.find_last_of('.')) + ".bin";
	
	this->labels = {};
}


/**
 *
 */
Assembler::~Assembler() {

}


/**
 *
 */
void Assembler::assemble() {
	this->labels.insert({"test", 10});

	std::cout << this->mnemonics.at("MM").code << std::endl;
	std::cout << this->mnemonics.at("MM").size << std::endl;
	std::cout << this->labels.at("test") << std::endl;

	this->run(0);

	this->dumpLabelTable();
	return;
}


/**
 *
 */
void Assembler::run(bool step) {
	std::ifstream assemblyFile(this->inputFile);

	for (std::string line; std::getline(assemblyFile, line); ) {
		std::cout << line << std::endl;

		// Retira comentários
		std::string command = line.substr(0, line.find_last_of(';'));
		std::cout << command << std::endl;
	}

	assemblyFile.close();
	return;
}


/**
 *
 */
void Assembler::dumpLabelTable() {
	std::ofstream labelsFile(this->inputFile + ".labels");

	labelsFile << "=================================================" << std::endl;
	labelsFile << this->inputFile << ".labels" << " LABEL TABLE FILE" << std::endl;
	labelsFile << "=================================================" << std::endl;
	labelsFile << std::left << std::setw(25) << "Label" << std::setw(25) << "Value" << std::endl;
	
	for (auto const &x: this->labels)
		labelsFile << std::left << std::setw(25) << x.first << std::setw(25) << x.second << std::endl;

	labelsFile.close();
	return;
}
