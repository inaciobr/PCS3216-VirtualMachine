/**
* label.cpp
* PCS 3216 - Sistemas de Programa��o - 2019
* Bruno Brand�o In�cio
*/

#include "label.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

/**
 *
 */
Label::Label() {
	this->labels = {};
}


/**
 *
 */
Label::~Label() {

}


/**
 *
 */
void Label::define(std::string label, unsigned address) {
	if (!this->labels.count(label) || this->labels.at(label) == UNDEFINED) {
		this->labels[label] = address;
	}
	else {
		throw "A label " + label + " j� foi inserida anteriormente.";
	}

	return;
}


/**
 *
 */
void Label::waitFor(std::string label) {
	this->labels.insert({ label, UNDEFINED });
	return;
}


/**
 * Salva a tabela de labels em um arquivo.
 */
void Label::dump(std::string fileName) {
	std::ofstream labelsFile(fileName);

	labelsFile << "===========================================" << std::endl;
	labelsFile << fileName << " LABEL TABLE FILE" << std::endl;
	labelsFile << "===========================================" << std::endl;

	labelsFile << std::left << std::setw(20) << "Label" << "Value" << std::endl;
	for (const auto& l : this->labels) {
		labelsFile << std::left << std::setw(20) << l.first
			<< "0x" << std::setw(8) << std::right << std::setfill('0') << std::uppercase << std::hex << l.second
			<< std::setfill(' ') << std::endl;
	}

	labelsFile.close();
	return;
}


/**
 *
 */
void Label::checkIntegrity() {
	for (const auto& l : this->labels) {
		if (l.second == UNDEFINED)
			throw "A label " + l.first + " foi usada, mas n�o foi definida.";
	}

	return;
}
