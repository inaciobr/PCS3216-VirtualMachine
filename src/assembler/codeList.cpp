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
void CodeList::insert(CodeList::Line cd) {
	this->list.push_back(cd);
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
	for (const auto &l : this->list) {
		if (!l.lineNumber)
			continue;

		// N�mero da linha
		listFile << std::setw(5) << std::right << l.lineNumber;

		// Informa��es referentes � linha
		if (l.sizeCode) {
			listFile << std::setw(6) << "0x" << std::setw(4) << std::setfill('0') << std::uppercase 
				<< std::hex << l.address << std::setfill(' ');
			
			if (l.sizeCode == 1)
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

		// Texto referente � linha
		listFile << std::setw(5) << "" << std::left << l.source << std::endl;
	}

	listFile.close();
}


/**
* Construtor de Line.
*/
CodeList::Line::Line(std::string source, unsigned lineNumber, unsigned address, unsigned sizeCode)
	: source(source), lineNumber(lineNumber), address(address), sizeCode(sizeCode), code({ 0x0000 }) {
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
