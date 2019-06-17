/**
* label.cpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#include "label.hpp"
#include "assembler.hpp"

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
	label = label.substr(0, label.find_last_of(':'));

	for (const auto& m : Assembler::mnemonics) {
		if (label == m.first)
			throw label + " não pode ser utilizado como uma label pois é um mnemônico de uma instrução.";
	}

	if (!this->labels.count(label) || this->labels.at(label) == UNDEFINED) {
		this->labels[label] = address;
	}
	else {
		throw "A label " + label + " foi definida mais de uma vez.";
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
			<< "0x" << std::setw(4) << std::right << std::setfill('0') << std::uppercase << std::hex << l.second
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
			throw "A label " + l.first + " foi usada, mas não foi definida.";
	}

	return;
}
