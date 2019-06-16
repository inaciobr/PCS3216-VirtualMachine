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
	std::cout << this->labels.at("test") << std::endl;

	this->run(0);

	this->dumpLabelTable();
	this->createListFile();
	this->addToListFile();
	return;
}


/**
 *
 */
void Assembler::run(bool step) {
	std::ifstream assemblyFile(this->inputFile);

	for (std::string line; std::getline(assemblyFile, line); ) {
		// Retira comentários
		std::string command = line.substr(0, line.find_last_of(';'));

		std::cout << command << std::endl;
	}

	assemblyFile.close();
	return;
}


/**
 * Salva a tabela de labels em um arquivo.
 */
void Assembler::dumpLabelTable() {
	std::ofstream labelsFile(this->inputFile + ".labels");

	labelsFile << "=================================================" << std::endl;
	labelsFile << this->inputFile << ".labels" << " LABEL TABLE FILE" << std::endl;
	labelsFile << "=================================================" << std::endl;

	labelsFile << std::left << std::setw(20) << "Label" << "Value" << std::endl;
	for (const auto &x: this->labels)
		labelsFile << std::left << std::setw(20) << x.first << x.second << std::endl;

	labelsFile.close();
	return;
}


/**
*
*/
void Assembler::createListFile() {
	std::ofstream listFile(this->inputFile + ".lst");

	listFile << "=================================================" << std::endl;
	listFile << this->inputFile << ".lst" << " LIST FILE" << std::endl;
	listFile << "=================================================" << std::endl;

	listFile << std::right << std::setw(5) << "LINE" << std::setw(10) << "ADDRESS"
		     << std::setw(10) << "CODE" << std::setw(5) << "" << std::left << "SOURCE" << std::endl;

	listFile.close();
	return;
}


/**
*
*/
void Assembler::addToListFile(unsigned line, unsigned address, unsigned code, std::string source) {
	std::ofstream listFile(this->inputFile + ".lst", std::ios::app);

	if (!line)
		listFile << std::setw(5) << "";
	else
		listFile << std::setw(5) << std::right << line;

	if (address == -1)
		listFile << std::setw(10) << "";
	else
		listFile << std::setw(6) << "" << std::setw(4) << std::setfill('0') << std::right << std::uppercase << std::hex << address << std::setfill(' ');

	if (code == -1)
		listFile << std::setw(10) << "";
	else if (code&0xff00)
		listFile << std::setw(6) << "" << std::setw(4) << std::setfill('0') << std::right << std::uppercase << std::hex << code << std::setfill(' ');
	else
		listFile << std::setw(8) << "" << std::setw(2) << std::setfill('0') << std::right << std::uppercase << std::hex << code << std::setfill(' ');

	listFile << std::setw(5) << "" << std::left << source << std::endl;

	listFile.close();
	return;
}
