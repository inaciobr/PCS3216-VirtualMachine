/**
* assembler.cpp
* PCS 3216 - Sistemas de Programa��o - 2019
* Bruno Brand�o In�cio
*/

#include "assembler.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>


/**
* TODO
*/
std::string Assembler::assemble() {
	// Roda os passos do assembler e verifica se todas as labels usadas foram definidas.
	this->runStep(0);
	this->labels.checkIntegrity();
	this->runStep(1);

	// Escreve arquivos com os dados da tabela de labels e outro com a listagem.
	this->labels.dump(this->inputFile + ".labels");
	this->list.dump(this->inputFile + ".lst");

	return std::string("");
}


/**
* TODO
*/
void Assembler::runStep(bool step) {
	std::ifstream assemblyFile(this->inputFile);
	unsigned instructionCounter = 0;
	unsigned lineNumber = 1;

	for (std::string line; std::getline(assemblyFile, line); lineNumber++) {
		// Leitura dos dados da linha.
		Line lineValues(line, lineNumber);

		// Trata caso onde h� defini��o de label.
		if (!step && lineValues.label.size())
			this->labels.define(lineValues.label, instructionCounter);

		// Trata instru��es e pseudo-instru��es.
		try {
			Assembler::processedInstruction instruction = this->processInstruction(lineValues, instructionCounter, step);

			if (step)
				this->list.insert({ lineNumber, line, instructionCounter, instruction.size, instruction.code });

			instructionCounter = instruction.nextInstruction;
		}
		catch (std::string e) {
			throw "\n" + std::to_string(lineNumber) + ": " + e;
		}
	}

	assemblyFile.close();

	// VERIFICAR END
	if (instructionCounter >= 0xFFFE)
		throw "\nO c�digo possui tamanho " + std::to_string(instructionCounter) + ", ultrapassando o limite de 0xFFFE";

	return;
}


Assembler::processedInstruction Assembler::processInstruction(Assembler::Line lineValues, unsigned int instructionCounter, bool step) {
	if (!lineValues.mnemonic.size())
		return { instructionCounter, 0, 0 };

	// Obt�m a instru��o ou pseudo-instru��o.
	Assembler::Instruction instruction;
	try {
		instruction = Assembler::mnemonics.at(lineValues.mnemonic);
	}
	catch (const std::out_of_range) {
		throw std::string("O mnem�nico " + lineValues.mnemonic + " n�o foi reconhecido.");
	}

	uint16_t operandValue = this->operandValue(lineValues.operand, step, instruction.allowLabel);
	uint16_t code = instruction.code + (operandValue & instruction.mask);

	// Trata pseudo-instru��es
	if (lineValues.mnemonic == "$") {
		instructionCounter += operandValue;
	}
	else if (lineValues.mnemonic == "@") {
		instructionCounter = operandValue;

		if (step) {
			if (this->code.size()) {
				this->saveObject();
				this->code.clear();
			}

			this->code.push_back((operandValue & 0xFF00) >> 8);
			this->code.push_back(operandValue & 0xFF);
			this->code.push_back(0x00);
		}
	}
	else if (step && lineValues.mnemonic == "#") {
		if (!this->code.size())
			throw std::string("Tentativa de finalizar um programa que n�o foi inicializado.");

		this->code[2] = this->code.size() - 3;

		this->saveObject();
		this->code.clear();
	}
	else {
		instructionCounter += instruction.size;

		if (step) {
			if (instruction.size == 2)
				this->code.push_back((code & 0xFF00) >> 8);

			this->code.push_back(code & 0xFF);
		}
	}

	return { instructionCounter, instruction.size, code };
}

/**
* Obt�m o valor do operando e valida as labels.
*/
int Assembler::operandValue(std::string operand, bool step, bool allowLabel) {
	// Verifica se o operando est� definido (todas as opera��es possuem um operando).
	if (!operand.size())
		throw std::string("Operando n�o definido.");

	// Verifica se h� alguma opera��o para ocorrer.
	std::string::size_type posOperation = operand.find_first_of("+-/*");

	// Imediatos
	if (operand[0] == '/')
		return std::stoi(operand.substr(1), nullptr, 16);
	else if (operand[0] == '\'')
		return static_cast<unsigned>(operand[1]);
	else if (operand[0] == '+' || operand[0] == '-' || std::isdigit(operand[0]))
		return std::stoi(operand);

	// Labels
	std::string label = operand.substr(0, posOperation);

	// Envia uma exce��o caso labels n�o sejam permitidas.
	if (!allowLabel)
		throw std::string("\'" + label + "\' foi identificada como sendo uma label e n�o � suportada neste caso.");

	// Primeiro passo adiciona a label � lista de labels n�o definidas.
	if (!step) {
		this->labels.waitFor(label);
		return Label::UNDEFINED;
	}

	// Obt�m o valor da label.
	int value = this->labels.getValue(label);

	// Caso n�o haja nenhuma opera��o com a label, retorna o valor dela.
	if (posOperation == std::string::npos)
		return value;

	// Caso haja alguma opera��o com a label, obt�m o valor do segundo termo desta opera��o.
	auto secondTermValue = operandValue(operand.substr(posOperation + 1), step, allowLabel);

	// Calcula o valor resultante da opera��o em cima da label.
	switch (operand[posOperation]) {
	case '+':
		value += secondTermValue;
		break;

	case '-':
		value -= secondTermValue;
		break;

	case '*':
		value *= secondTermValue;
		break;

	case '/':
		value /= secondTermValue;
		break;

	default:
		throw std::string("O caractere \'" + std::to_string(operand[posOperation]) + "\' n�o foi reconhecido.");
	}

	return value;
}


/**
* TODO
*/
void Assembler::saveObject() {
	std::string outputFile = this->inputFile.substr(0, this->inputFile.find_last_of('.'));

	this->makeObject(outputFile + ".obj");
	this->makeBin(outputFile + ".bin");
}


/**
* TODO
*/
void Assembler::makeObject(std::string outputFile) {
	std::ofstream objectFile(outputFile);
	uint8_t checkSum = 0xFF;

	objectFile << std::hex << std::setfill('0');

	for (const auto& c : this->code) {
		checkSum ^= c;
		objectFile << std::uppercase << std::setw(2) << static_cast<unsigned>(c) << " ";
	}

	objectFile << static_cast<unsigned>(checkSum);

	objectFile.close();

	return;
}


/**
* TODO
*/
void Assembler::makeBin(std::string outputFile) {
	std::ofstream objectFile(outputFile, std::ofstream::binary);
	uint8_t checkSum = 0xFF;

	for (const auto& c : this->code) {
		checkSum ^= c;
		objectFile << c;
	}

	objectFile << checkSum;

	objectFile.close();

	return;
}

Assembler::Line::Line(std::string text, unsigned int position) : text(text), position(position) {
	// Retira coment�rios e procura a primeira palavra da linha.
	std::string command = text.substr(0, text.find_last_of(';'));
	std::istringstream sline(command);

	// Obt�m dados sobre a label, se houver uma.
	if (command.size() && !std::isspace(command[0]))
		sline >> this->label;

	// Obt�m dados sobre o mnem�nico e o operando.
	sline >> this->mnemonic >> this->operand;
}
