/**
 * assembler.cpp
 * PCS 3216 - Sistemas de Programação - 2019
 * Bruno Brandão Inácio
 */

#include "assembler.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>

#include <iostream>


/**
 * Executa a montagem do código.
 */
void Assembler::assemble() {
	// Roda os passos do assembler e verifica se todas as labels usadas foram definidas.
	this->firstPass();
	this->secondPass();

	std::string outputFile = this->inputFile.substr(0, this->inputFile.find_last_of('.'));

	// Escreve arquivos com os dados da tabela de labels e outro com a listagem.
	this->labels.dump(this->inputFile + ".labels");
	this->list.dump(this->inputFile + ".lst");

	// Escreve arquivo com a saída em decimal (ASCII) e outro em binário.
	this->makeObject(outputFile + ".obj");
	this->makeBin(outputFile + ".bin");
}


/**
 * Executa o primeiro passo da montagem do código.
 */
void Assembler::firstPass() {
	std::string line;
	std::ifstream assemblyFile(this->inputFile);
	unsigned instructionCounter = 0x0000;

	for (unsigned lineNumber = 1; std::getline(assemblyFile, line); lineNumber++) {
		// Leitura dos dados da linha.
		auto lineData = this->list.insert({ lineNumber, line });

		// Trata caso onde há definição de label.
		if (lineData.label.size())
			this->labels.define(lineData.label, instructionCounter);

		// Trata instruções e pseudo-instruções.
		try {
			if (unsigned size = this->firstProcess(lineData);  size) {
				lineData.setInstruction(instructionCounter, size);
				instructionCounter += size;
			}
		}
		catch (std::string e) {
			throw "\n" + std::to_string(lineNumber) + ": " + e;
		}
		catch (int e) {
			instructionCounter = e;
		}
	}

	assemblyFile.close();

	if (instructionCounter >= 0xFFFE)
		throw "\nO codigo possui tamanho " + std::to_string(instructionCounter) + ", ultrapassando o limite de 0xFFFE";
}


/**
* Executa o segundo passo da montagem do código.
*/
void Assembler::secondPass() {
	this->labels.checkIntegrity();

	for (auto &lineData: this->list) {
		// Trata instruções e pseudo-instruções.
		try {
			unsigned code = this->secondProcess(lineData);
			lineData.code.value = code;
		}
		catch (std::string e) {
			throw "\n" + std::to_string(lineData.lineNumber) + ": " + e;
		}
	}
}


/**
 * Realiza o processamento de instruções do primeiro passo.
 */
unsigned Assembler::firstProcess(CodeList::Line lineValues) {
	if (!lineValues.mnemonic.size())
		return 0;

	// Obtém a instrução ou pseudo-instrução.
	Assembler::Instruction instruction;
	try {
		instruction = Assembler::mnemonics.at(lineValues.mnemonic);
	}
	catch (const std::out_of_range) {
		throw std::string("O mnemonico " + lineValues.mnemonic + " nao foi reconhecido.");
	}

	int operandValue = this->operandValue(lineValues.operand, instruction.allowLabel, false); // TODO

	// Trata pseudo-instruções
	if (lineValues.mnemonic == "@")
		throw operandValue;

	if (lineValues.mnemonic == "$")
		return operandValue;

	return instruction.size;
}


/**
 * Realiza o processamento de instruções do primeiro passo.
 */
unsigned Assembler::secondProcess(CodeList::Line lineValues) {
	if (!lineValues.mnemonic.size())
		return 0;

	// Obtém a instrução ou pseudo-instrução.
	Assembler::Instruction instruction;
	try {
		instruction = Assembler::mnemonics.at(lineValues.mnemonic);
	}
	catch (const std::out_of_range) {
		throw std::string("O mnemonico " + lineValues.mnemonic + " nao foi reconhecido.");
	}

	int operandValue = this->operandValue(lineValues.operand, instruction.allowLabel, false);

	return instruction.size;
}



/**
 * Obtém o valor do operando e valida as labels.
 */
int Assembler::operandValue(std::string operand, bool allowLabel, bool evaluateLabel) {
	// Verifica se o operando está definido (todas as operações possuem um operando).
	if (!operand.size())
		throw std::string("Operando nao definido.");

	// Verifica se há alguma operação para ocorrer.
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

	// Envia uma exceção caso labels não sejam permitidas.
	if (!allowLabel)
		throw std::string("\'" + label + "\' foi identificada como sendo uma label e não é suportada neste caso.");

	// Primeiro passo adiciona a label à lista de labels não definidas.
	if (!evaluateLabel) {
		this->labels.waitFor(label);
		return Label::UNDEFINED;
	}

	// Obtém o valor da label.
	int value = this->labels.getValue(label);

	// Caso não haja nenhuma operação com a label, retorna o valor dela.
	if (posOperation == std::string::npos)
		return value;

	// Caso haja alguma operação com a label, obtém o valor do segundo termo desta operação.
	auto secondTermValue = operandValue(operand.substr(posOperation + 1), allowLabel, evaluateLabel);

	// Calcula o valor resultante da operação em cima da label.
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
		throw std::string("O caractere \'" + std::to_string(operand[posOperation]) + "\' não foi reconhecido.");
	}

	return value;
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

