/**
* codeList.cpp
* PCS 3216 - Sistemas de Programa��o - 2019
* Bruno Brand�o In�cio
*/

#include "codeList.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>


/**
 * Insere nova linha na mem�ria.
 */
CodeList::Line CodeList::insert(CodeList::Line line) {
	this->list.push_back(line);
	return line;
}


/**
 * Despeja o conte�do da lista de dados em um arquivo de texto.
 */
void CodeList::dump(std::string fileName) {
	std::ofstream listFile(fileName);

	// Cabe�alho
	listFile << "=================================================" << std::endl;
	listFile << fileName << " LIST FILE" << std::endl;
	listFile << "=================================================" << std::endl;

	// T�tulos
	listFile << std::right << std::setw(5) << "LINE" << std::setw(10) << "ADDRESS"
		<< std::setw(10) << "CODE" << std::setw(5) << "" << std::left << "SOURCE" << std::endl;

	// Linhas
	for (const auto &l : this->list)
		listFile << l.str();

	listFile.close();
}


/**
* Construtor de Line.
*/
CodeList::Line::Line(unsigned lineNumber, std::string source)
	: source(source), lineNumber(lineNumber), address(0), codeSize(0), code({ 0x0000 }) {
	// Retira coment�rios e procura a primeira palavra da linha.
	std::string command = source.substr(0, source.find_last_of(';'));

	// Linha n�o nenhum comando.
	if (!command.size())
		return;

	std::istringstream sline(command);

	// Obt�m dados sobre a label, se houver uma.
	if (!std::isspace(command[0]))
		sline >> this->label;

	// Obt�m dados sobre o mnem�nico e o operando.
	sline >> this->mnemonic >> this->operand;
}


/**
* Define os valores relacioandos a um c�digo de instru��o.
*/
void CodeList::Line::setCode(unsigned address, unsigned size, unsigned code) {
	this->address = address;
	this->codeSize = size;
	this->code.value = code;
}


/**
* Formato de exibi��o da linha.
*/
std::string CodeList::Line::str() const {
	std::stringstream line;

	// N�mero da linha
	line << std::setw(5) << std::right << this->lineNumber;

	// Informa��es referentes � linha
	if (this->codeSize) {
		line << std::setw(6) << "0x" << std::setw(4) << std::setfill('0') << std::uppercase
			<< std::hex << this->address << std::setfill(' ');

		if (this->codeSize == 1)
			line << std::setw(9) << "" << std::setfill('0') << std::uppercase << std::setw(2)
				<< static_cast<unsigned>(this->code.byte[0]);
		else
			line << std::setw(6) << "" << std::setfill('0') << std::uppercase << std::setw(2)
				<< static_cast<unsigned>(this->code.byte[1]) << " " << std::setw(2) << static_cast<unsigned>(this->code.byte[0]);

		line << std::dec << std::setfill(' ');
	}
	else {
		line << std::setw(21) << "";
	}

	// Texto referente � linha
	line << std::setw(5) << "" << std::left << this->source << std::endl;

	return line.str();
}
