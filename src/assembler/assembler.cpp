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
	{"JP",	{0x0000,	2,	0x0FFF	}},	// Jump (UNCONDITIONAL)
	{"JZ",	{0x1000, 2,	0x0FFF	}},	// Jump if zero
	{"JN",	{0x2000, 2,	0x0FFF	}},	// Jump if negative
	{"CN",	{0x30,	1,	0x0F	}},	// Control
	{ "+",	{0x4000, 2,	0x0FFF	}},	// Add
	{ "-",	{0x5000, 2,	0x0FFF	}},	// Subtract
	{ "*",	{0x6000, 2,	0x0FFF	}},	// Multiply
	{ "/",	{0x7000, 2,	0x0FFF	}},	// Divide
	{"LD",	{0x8000, 2,	0x0FFF	}},	// Load from memory
	{"MM",	{0x9000, 2,	0x0FFF	}},	// Move to memory
	{"SC",	{0xA000, 2,	0x0FFF	}},	// Subroutine call
	{"OS",	{0xB0,	1,	0x0F	}},	// Operating system call
	{"IO",	{0xC0,	1,	0x0F	}}	// Input/Output
	//{??,	{0xD,	1,	0x0F	}},	// Free
	//{??,	{0xE,	1,	0x0F	}},	// Free
	//{??,	{0xF,	1,	0x0F	}}	// Free
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
	//this->labels.checkIntegrity();
	this->runStep(1);

	this->labels.dump(this->inputFile + ".labels");
	this->list.dump(this->inputFile + ".lst");

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
		// Retira comentários e procura a primeira palavra da linha.
		std::string command = line.substr(0, line.find_last_of(';'));
		auto pos = command.find_first_not_of(" \t\n");
		lineNumber++;

		// Linha sem comando.
		if (pos == std::string::npos) {
			if (step)
				this->list.insert({ lineNumber, ListCode::UNDEFINED, ListCode::UNDEFINED, line });

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

		// Obtém mnemônico e o operando.
		sline >> mnemonic >> operand;

		// Linha contém apenas a definição de uma label.
		if (!mnemonic.length()) {
			if (step)
				this->list.insert({ lineNumber, ListCode::UNDEFINED, ListCode::UNDEFINED, line });

			continue;
		}

		// Verifica se o operando está definido (todas as operações possuem um operando).
		if (!operand.length())
			throw "\n" + std::to_string(lineNumber) + ": operando não definido.";

		// Obtém o valor do operando e valida as labels.
		int operandValue;
		try {
			operandValue = this->operandValue(operand, step);
		}
		catch (std::string e) {
			throw "\n" + std::to_string(lineNumber) + ": " + e;
		}





		uint16_t code = 0xfebc;
		// Instruções e pseudo-instruções.
		// ======
		// VERIFICAR PSEUDO
		if (operand == "@") {

		}
		else if (operand == "#") {

		}
		else if (operand == "K") {

		}
		else if (operand == "$") {

		}
		else {
			try {
				auto opr = Assembler::mnemonics.at(mnemonic);

				if (step)
					this->list.insert({ lineNumber, instructionCounter, code, line });

				instructionCounter += opr.size;
			}
			catch (const std::out_of_range) {
				continue;
				throw "\n" + std::to_string(lineNumber) + ": O mnemônico " + mnemonic + " não foi reconhecido.";
			}
		}


	}

	assemblyFile.close();
	return;
}


int Assembler::operandValue(std::string operand, bool step) {
	// Imediatos
	if (operand[0] == '/') {
		return std::stoi(operand.substr(1, std::string::npos), nullptr, 16);
	}
	else if (operand[0] == '\'') {
		return static_cast<unsigned>(operand[1]);
	}
	else if (operand.find_first_not_of("+-0123456789") == std::string::npos) {
		return std::stoi(operand);
	}

	// Labels
	std::string::size_type posOper = operand.find_last_of("+-/*");
	std::string label = operand.substr(0, posOper);

	if (!step) {
		this->labels.waitFor(label);
		return 0;
	}

	int value = this->labels.getValue(label);

	if (posOper == std::string::npos)
		return value;

	switch (operand[posOper]) {
	case '+':
		value += std::stoi(operand.substr(posOper + 1));
		break;

	case '-':
		value -= std::stoi(operand.substr(posOper + 1));
		break;

	case '*':
		value *= std::stoi(operand.substr(posOper + 1));
		break;

	case '/':
		value /= std::stoi(operand.substr(posOper + 1));
		break;

	default:
		throw "O caractere \'" + std::to_string(operand[posOper]) + "\' não foi reconhecido.";
	}

	return value;
}



/**
* TODO
*/
void Assembler::makeObject() {

}
