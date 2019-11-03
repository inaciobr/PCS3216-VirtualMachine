/**
 * assembler.cpp
 * PCS 3216 - Sistemas de Programa��o - 2019
 * Bruno Brand�o In�cio
 */

#include "assembler.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>

#include <iostream>


/**
 * Executa a montagem do c�digo.
 */
void Assembler::assemble() {
	this->labels = Label();
	this->list = CodeList();

	// Roda os passos do assembler e verifica se todas as labels usadas foram definidas.
	this->firstPass();
	this->secondPass();

	std::string outputFile = this->inputFile.substr(0, this->inputFile.find_last_of('.'));

	// Escreve arquivos com os dados da tabela de labels e outro com a listagem.
	this->labels.dump(this->inputFile + ".labels");
	this->list.dump(this->inputFile + ".lst");

	// Escreve arquivo com a sa�da em decimal (ASCII) e outro em bin�rio.
	this->makeCode(outputFile + ".bin", Assembler::flushBin);
	this->makeCode(outputFile + ".obj", Assembler::flushHex);
}


/**
 * Executa o primeiro passo da montagem do c�digo.
 */
void Assembler::firstPass() {
	unsigned instructionCounter = 0x0000;

	std::string line;
	std::ifstream assemblyFile(this->inputFile);

	for (unsigned lineNumber = 1; std::getline(assemblyFile, line); lineNumber++) {
		auto &lineData = this->list.insert({ lineNumber, line });

		if (lineData.label.size())
			this->labels.define(lineData.label, instructionCounter);

		if (!lineData.mnemonic.size())
			continue;

		// Nesta etapa, � necess�rio apenas descobrir as labels usadas,
		// o tamanho das instru��es e e tratar pseudo-instru��es.
		Assembler::Instruction instruction;

		try {
			try {
				instruction = Assembler::mnemonics.at(lineData.mnemonic);
			}
			catch (const std::out_of_range) {
				throw std::string("O mnemonico " + lineData.mnemonic + " nao foi reconhecido.");
			}

			int operandValue = this->operandValue(lineData.operand, instruction.allowLabel, false);

			if (lineData.mnemonic == "@") {
				instructionCounter = operandValue;
				continue;
			}

			if (lineData.mnemonic == "#")
				break;

			if (lineData.codeSize = lineData.mnemonic == "$" ? operandValue : instruction.size; lineData.codeSize) {
				lineData.address = instructionCounter;
				instructionCounter += lineData.codeSize;
			}
		}

		catch (std::string e) {
			throw "\n" + std::to_string(lineNumber) + ": " + e;
		}
	}

	assemblyFile.close();

	if (instructionCounter >= 0xFFFE)
		throw "\nO codigo possui tamanho " + std::to_string(instructionCounter) + ", ultrapassando o limite de 0xFFFE.";
}


/**
* Executa o segundo passo da montagem do c�digo.
*/
void Assembler::secondPass() {
	this->labels.checkIntegrity();

	for (auto &lineData: this->list) {
		if (lineData.mnemonic.empty() || lineData.mnemonic == "$")
			continue;

		// Trata instru��es e pseudo-instru��es.
		try {
			// Obt�m a instru��o ou pseudo-instru��o.
			Assembler::Instruction instruction = Assembler::mnemonics.at(lineData.mnemonic);
			int operandValue = this->operandValue(lineData.operand, instruction.allowLabel, true);

			lineData.code.value = instruction.code | (operandValue & instruction.mask);
		}
		catch (std::string e) {
			throw "\n" + std::to_string(lineData.lineNumber) + ": " + e;
		}
	}
}


/**
 * Obt�m o valor do operando e valida as labels.
 */
int Assembler::operandValue(std::string operand, bool allowLabel, bool evaluateLabel) {
	// Verifica se o operando est� definido (todas as opera��es possuem um operando).
	if (!operand.size())
		throw std::string("Operando nao definido.");

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
	if (!evaluateLabel) {
		this->labels.waitFor(label);
		return Label::UNDEFINED;
	}

	// Obt�m o valor da label.
	int value = this->labels.getValue(label);

	// Caso n�o haja nenhuma opera��o com a label, retorna o valor dela.
	if (posOperation == std::string::npos)
		return value;

	// Caso haja alguma opera��o com a label, obt�m o valor do segundo termo desta opera��o.
	auto secondTermValue = operandValue(operand.substr(posOperation + 1), allowLabel, evaluateLabel);

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
 * A sa�da ser� feita em um arquivo bin�rio, pronto para ser lido pela m�quina virtual.
 */
void Assembler::flushBin(std::string outputFile, uint8_t code[]) {
	std::ofstream objectFile(outputFile, std::ofstream::binary | std::ofstream::app);
	uint8_t checkSum = 0x00;

	for (int i = 0; i < code[2] + 3; i++) {
		checkSum += code[i];
		objectFile << code[i];
	}

	objectFile << checkSum;
	objectFile.close();
}


/**
 * A sa�da ser� feita em um arquivo de texto em c�digo ascii
 * com o c�digo exibido em hexadecimal.
 */
void Assembler::flushHex(std::string outputFile, uint8_t code[]) {
	std::ofstream objectFile(outputFile, std::ofstream::app);
	uint8_t checkSum = 0x00;

	objectFile << std::hex << std::setfill('0');

	for (int i = 0; i < code[2] + 3; i++) {
		checkSum += code[i];
		objectFile << std::uppercase << std::setw(2) << static_cast<unsigned>(code[i]) << " ";

		// Quebra de linha para facilitar a visualiza��o.
		if (!((i + 1) % 16))
			objectFile << std::endl;
	}

	objectFile << std::uppercase << std::setw(2) << static_cast<unsigned>(checkSum) << "\n\n";
	objectFile.close();
}

/**
 * Monta o vetor de sa�da do c�digo montado.
 * O formato � dado a seguir:
 * byte 1: Quantidade de bytes a serem lidos
 * byte 2 e 3: Endere�o onde o c�digo deve ser montado
 * �ltimo byte: checksum
 * retante: c�digo
 */
void Assembler::makeCode(std::string outputFile, void (*flush)(std::string, uint8_t[]) = Assembler::flushBin) {
	// Limpar arquivo.
	std::ofstream a(outputFile);
	a.close();

	uint8_t seq[0xFE] = {};
	uint8_t *code = seq + 3;

	for (const auto &line : this->list) {
		if (!line.codeSize)
			continue;

		if (seq[2] == 0) {
			seq[0] = (line.address & 0xFF00) >> 8;
			seq[1] = line.address & 0xFF;
		}
		
		if (line.codeSize == 2) {
			code[seq[2]++] = line.code.byte[1];
			code[seq[2]++] = line.code.byte[0];
		}
		else if (line.codeSize > 2) {
			int size = line.codeSize;
			int address = line.address;

			while ((seq[2] + size) >= 0xFF) {
				size -= 0xFE - seq[2];
				address += 0xFE - seq[2];
				seq[2] = 0xFE;

				flush(outputFile, seq);
				memset(seq, 0, sizeof(seq));

				seq[0] = (address & 0xFF00) >> 8;
				seq[1] = address & 0xFF;
			}

			seq[2] += size;
		}
		else {
			code[seq[2]++] = line.code.byte[0];
		}


		if (seq[2] >= 0xFE) {
			flush(outputFile, seq);
			memset(seq, 0, sizeof(seq));
		}
	}

	flush(outputFile, seq);
}
