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

	this->run(0);
	//this->run(1);
	this->labels.define("AAA", 4124);
	this->labels.waitFor("BBB");
	this->labels.waitFor("AAA");
	this->labels.waitFor("BBB");
	this->labels.waitFor("CCC");
	this->labels.define("CCC", 12);
	//this->labels.checkIntegrity();

	this->labels.dump(this->inputFile + ".labels");
	//this->makeObject();

	return this->outputFile;
}


/**
* TODO
*/
void Assembler::run(bool step) {
	std::ifstream assemblyFile(this->inputFile);

	unsigned i = 1;
	for (std::string line; std::getline(assemblyFile, line); i++) {
		if (line.empty())
			continue;

		std::string command = line.substr(0, line.find_last_of(';'));
		std::istringstream sline(command);

		auto pos = command.find_first_not_of(" \t\n");
		if (pos == 0 && !step) {
			std::string label;
			sline >> label;
			this->labels.define(label , 10);
			std::cout << label << std::endl;
		} else if (pos == std::string::npos) {
			if (!step)
				this->addToListFile({ i, (unsigned)-1, (unsigned)-1, line });
			continue;
		} else {
			
		}

		//std::cout << command << std::endl;
	}

	assemblyFile.close();
	return;
}


/**
* TODO
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
* TODO
*/
void Assembler::addToListFile(Assembler::listCode lst) {
	std::ofstream listFile(this->inputFile + ".lst", std::ios::app);

	if (!lst.line)
		listFile << std::setw(5) << "";
	else
		listFile << std::setw(5) << std::right << lst.line;

	if (lst.address == -1)
		listFile << std::setw(10) << "";
	else
		listFile << std::setw(6) << "" << std::setw(4) << std::setfill('0') << std::right << std::uppercase << std::hex << lst.address << std::setfill(' ');

	if (lst.code == -1)
		listFile << std::setw(10) << "";
	else if (lst.code&0xff00)
		listFile << std::setw(6) << "" << std::setw(4) << std::setfill('0') << std::right << std::uppercase << std::hex << lst.code << std::setfill(' ');
	else
		listFile << std::setw(8) << "" << std::setw(2) << std::setfill('0') << std::right << std::uppercase << std::hex << lst.code << std::setfill(' ');

	listFile << std::setw(5) << "" << std::left << lst.source << std::endl;

	listFile.close();
	return;
}

/**
* TODO
*/
void Assembler::makeObject() {

}
