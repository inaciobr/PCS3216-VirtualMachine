/**
 * codeList.cpp
 * PCS 3216 - Sistemas de Programação - 2019
 * Bruno Brandão Inácio
 */

#include "codeList.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>


/**
 * Insere nova linha na lista.
 */
CodeList::Line& CodeList::insert(CodeList::Line line) {
	this->list.push_back(line);
	return this->list.back();
}


/**
 * Despeja o conteúdo da lista de dados em um arquivo de texto.
 */
void CodeList::dump(std::string fileName) {
	std::ofstream listFile(fileName);

	// Cabeçalho
	listFile << "=================================================" << std::endl;
	listFile << fileName << " LIST FILE" << std::endl;
	listFile << "=================================================" << std::endl;

	// Títulos
	listFile << std::right << std::setw(5) << "LINE" << std::setw(12) << "ADDRESS"
		<< std::setw(12) << "CODE" << std::setw(5) << "" << std::left << "SOURCE" << std::endl;

	// Linhas
	for (const auto &l: this->list)
		listFile << l.str();

	listFile.close();
}


/**
 * Construtor de Line.
 */
CodeList::Line::Line(unsigned lineNumber, std::string source)
	: source(source), lineNumber(lineNumber), address(0), codeSize(0), code{ 0x0000 } {
	// Retira comentários.
	std::string command = source.substr(0, source.find_first_of(';'));

	if (!command.size())
		return;

	std::istringstream sline(command);

	// Obtém dados sobre a label, se houver uma.
	if (!std::isspace(command[0]))
		sline >> this->label;

	// Obtém dados sobre o mnemônico e o operando.
	sline >> this->mnemonic >> this->operand;
}


/**
 * Formato de exibição da linha.
 */
std::string CodeList::Line::str() const {
	std::stringstream line;

	// Número da linha
	line << std::setw(5) << std::right << this->lineNumber;

	// Informações referentes à linha
	if (this->codeSize) {
		line << std::setw(8) << "0x" << std::setw(4) << std::setfill('0') << std::uppercase
			<< std::hex << this->address << std::setfill(' ');

		if (this->codeSize == 2)
			line << std::setw(7) << "" << std::setfill('0') << std::uppercase << std::setw(2)
				<< static_cast<unsigned>(this->code.byte[1]) << " " << std::setw(2) << static_cast<unsigned>(this->code.byte[0]);
		else
			line << std::setw(10) << "" << std::setfill('0') << std::uppercase << std::setw(2)
				<< static_cast<unsigned>(this->code.byte[0]);

		line << std::dec << std::setfill(' ');
	}
	else {
		line << std::setw(24) << "";
	}

	// Texto referente à linha
	line << std::setw(5) << "" << std::left << this->source << std::endl;

	return line.str();
}
