/**
* codeList.cpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#include "codeList.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>


/**
 * Insere nova linha na memória.
 */
CodeList::Line CodeList::insert(CodeList::Line line) {
	this->list.push_back(line);
	return line;
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
	listFile << std::right << std::setw(5) << "LINE" << std::setw(10) << "ADDRESS"
		<< std::setw(10) << "CODE" << std::setw(5) << "" << std::left << "SOURCE" << std::endl;

	// Linhas
	for (const auto &l : this->list) {
		if (!l.lineNumber)
			continue;

		// Número da linha
		listFile << std::setw(5) << std::right << l.lineNumber;

		// Informações referentes à linha
		if (l.codeSize) {
			listFile << std::setw(6) << "0x" << std::setw(4) << std::setfill('0') << std::uppercase 
				<< std::hex << l.address << std::setfill(' ');
			
			if (l.codeSize == 1)
				listFile << std::setw(9) << "" << std::setfill('0') << std::uppercase << std::setw(2) 
					<< static_cast<unsigned>(l.code.byte[0]);
			else
				listFile << std::setw(6) << "" << std::setfill('0') << std::uppercase << std::setw(2) 
					<< static_cast<unsigned>(l.code.byte[1]) << " " << std::setw(2) << static_cast<unsigned>(l.code.byte[0]);

			listFile << std::dec << std::setfill(' ');
		}
		else {
			listFile << std::setw(21) << "";
		}

		// Texto referente à linha
		listFile << std::setw(5) << "" << std::left << l.source << std::endl;
	}

	listFile.close();
}


/**
* Construtor de Line.
*/
CodeList::Line::Line(unsigned lineNumber, std::string source)
	: source(source), lineNumber(lineNumber), address(0), codeSize(0), code({ 0x0000 }) {
	// Retira comentários e procura a primeira palavra da linha.
	std::string command = source.substr(0, source.find_last_of(';'));

	// Linha não nenhum comando.
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
* Define os valores relacioandos a um código de instrução.
*/
void CodeList::Line::setCode(unsigned address, unsigned size, unsigned code) {
	this->address = address;
	this->codeSize = size;
	this->code.value = code;
}
