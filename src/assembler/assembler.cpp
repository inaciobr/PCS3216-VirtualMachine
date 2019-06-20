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

	// Arquivos de saída.
	std::string outputFile = this->inputFile.substr(0, this->inputFile.find_last_of('.')) + ".bin";
	this->makeObject(outputFile);
	this->makeBin(outputFile);

	return outputFile;
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
		std::string label, mnemonic, operand;
		std::istringstream sline(line);

		// Retira comentários e procura a primeira palavra da linha.
		std::string command = line.substr(0, line.find_last_of(';'));

		// Trata caso onde há definição de label.
		if (command.size() && !std::isspace(command[0])) {
			sline >> label;

			if (!step)
				this->labels.define(label, instructionCounter);
		}

		// Obtém dados sobre o mnemônico e o operando.
		sline >> mnemonic >> operand;

		// Linha sem comando ou contendo apenas a definição de uma label.
		if (!mnemonic.size()) {
			if (step)
				this->list.insert({ lineNumber, line, 0 });

			continue;
		}


		
		try {
			// Obtém a instrução ou pseudo-instrução.
			auto instruction = Assembler::mnemonics.at(mnemonic);

			// Obtém o valor do operando e valida as labels.
			auto operandValue = this->operandValue(operand, step, instruction.allowLabel);

			// Trata pseudo-instruções.
			if (mnemonic == "$") {
				instructionCounter += operandValue;
			}
			else if (operand == "@") {
				instructionCounter = operandValue;
			}
			else if (operand == "#") {

			}

			if (step) {
				uint16_t code = instruction.code + (operandValue & instruction.mask);
				this->list.insert({ lineNumber, line, instruction.size, instructionCounter, code });
			}

			instructionCounter += instruction.size;
		}
		catch (std::string e) {
			throw "\n" + std::to_string(lineNumber) + ": " + e;
		}
		catch (const std::out_of_range) {
			throw "\n" + std::to_string(lineNumber) + ": O mnemônico " + mnemonic + " não foi reconhecido.";
		}



	}

	// VERIFICAR END
	// VERIFICAR TAMANHO MAXIMO (0XFFFE)
	assemblyFile.close();
	return;
}


/**
* 
*/
int Assembler::operandValue(std::string operand, bool step, bool allowLabel) {
	// Verifica se o operando está definido (todas as operações possuem um operando).
	if (!operand.size())
		throw "Operando não definido.";

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
		throw "\'" + label + "\' foi identificada como sendo uma label e não é suportada neste caso.";

	// Primeiro passo adiciona a label à lista de labels não definidas.
	if (!step) {
		this->labels.waitFor(label);
		return Label::UNDEFINED;
	}

	// Obtém o valor da label.
	int value = this->labels.getValue(label);

	// Caso não haja nenhuma operação com a label, retorna o valor dela.
	if (posOperation == std::string::npos)
		return value;

	// Caso haja alguma operação com a label, obtém o valor do segundo termo desta operação.
	auto secondTermValue = this->operandValue(operand.substr(posOperation + 1), step, allowLabel);

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
		throw "O caractere \'" + std::to_string(operand[posOperation]) + "\' não foi reconhecido.";
	}

	return value;
}


/**
* TODO
*/
void Assembler::makeObject(std::string outputFile) {

}


/**
* TODO
*/
void Assembler::makeBin(std::string outputFile) {

}
