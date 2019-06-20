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

	// Arquivos de sa�da.
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
	unsigned lineNumber = 0;

	for (std::string line; std::getline(assemblyFile, line); ) {
		// Retira coment�rios e procura a primeira palavra da linha.
		std::string command = line.substr(0, line.find_last_of(';'));
		auto pos = command.find_first_not_of(" \t\n");
		lineNumber++;

		// Linha sem comando.
		if (pos == std::string::npos) {
			if (step)
				this->list.insert({ lineNumber, line, 0 });

			continue;
		}

		// Leitura dos commandos.
		std::istringstream sline(line);
		std::string label, mnemonic, operand;

		// Defini��o de label.
		if (pos == 0) { 
			sline >> label;

			if (!step)
				this->labels.define(label, instructionCounter);
		}

		// Obt�m mnem�nico e o operando.
		sline >> mnemonic >> operand;

		// Linha cont�m apenas a defini��o de uma label.
		if (!mnemonic.length()) {
			if (step)
				this->list.insert({ lineNumber, line, 0 });

			continue;
		}

		try {
			// Trata instru��es e pseudo-instru��es
			auto instruction = Assembler::mnemonics.at(mnemonic);

			// Obt�m o valor do operando e valida as labels.
			int operandValue = this->operandValue(operand, step, instruction.allowLabel);

			// Trata pseudo-instru��es.
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
			throw "\n" + std::to_string(lineNumber) + ": O mnem�nico " + mnemonic + " n�o foi reconhecido.";
		}



	}

	assemblyFile.close();
	return;
}


/**
* 
*/
int Assembler::operandValue(std::string operand, bool step, bool allowLabel) {
	// Verifica se o operando est� definido (todas as opera��es possuem um operando).
	if (!operand.length())
		throw "Operando n�o definido.";

	// Imediatos
	if (operand[0] == '/')
		return std::stoi(operand.substr(1), nullptr, 16);
	else if (operand[0] == '\'')
		return static_cast<unsigned>(operand[1]);
	else if (operand.find_first_not_of("+-0123456789") == std::string::npos)
		return std::stoi(operand);


	// Labels
	std::string::size_type posOperation = operand.find_first_of("+-/*");
	std::string label = operand.substr(0, posOperation);

	if (!allowLabel)
		throw "\'" + label + "\' foi identificada como sendo uma label e n�o � suportada neste caso.";


	// Primeiro passo adiciona a label � lista de labels n�o definidas.
	if (!step) {
		this->labels.waitFor(label);

		return 0;
	}

	// Obt�m o valor da label.
	int value = this->labels.getValue(label);

	// Caso n�o haja nenhuma opera��o em cima da label, retorna o valor dela.
	if (posOperation == std::string::npos)
		return value;

	// Caso haja alguma opera��o em cima da label, obt�m o valor do segundo termo.
	std::string::size_type posEnd;
	std::string secTerm = operand.substr(posOperation + 1);
	int secTermValue = std::stoi(secTerm, &posEnd);

	// Se ouver algum caractere n�o reconhecido no segundo termo, acusa erro.
	if (secTerm.size() != posEnd)
		throw "Os caracteres \'" + secTerm.substr(posEnd) + "\' n�o foram reconhecidos.";

	// Calcula o valor resultante da opera��o em cima da label.
	switch (operand[posOperation]) {
	case '+':
		value += secTermValue;
		break;

	case '-':
		value -= secTermValue;
		break;

	case '*':
		value *= secTermValue;
		break;

	case '/':
		value /= secTermValue;
		break;

	default:
		throw "O caractere \'" + std::to_string(operand[posOperation]) + "\' n�o foi reconhecido.";
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
