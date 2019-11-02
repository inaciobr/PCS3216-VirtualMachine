/**
 * label.cpp
 * PCS 3216 - Sistemas de Programação - 2019
 * Bruno Brandão Inácio
 */

#include "label.hpp"
#include "assembler.hpp"

#include <fstream>
#include <iomanip>


/**
 * Adiciona label como UNDEFINED apenas se já não tiver sido definida.
 */
void Label::waitFor(std::string label) {
	this->labels.insert({ label, Label::UNDEFINED });
}


/**
 * Define o valor de uma label.
 */
void Label::define(std::string label, unsigned int address) {
	label = label.substr(0, label.find_first_of(":+-/*?\\"));

	if (Assembler::mnemonics.find(label) != Assembler::mnemonics.end())
		throw label + " nao pode ser utilizado como uma label pois e um mnemonico de uma instrucao.";

	if (!this->labels.count(label) || this->labels.at(label) == Label::UNDEFINED)
		this->labels[label] = address;
	else
		throw "A label " + label + " foi definida mais de uma vez.";
}


/**
 * Retorna o valor de uma label.
 */
unsigned Label::getValue(std::string label) {
	return this->labels.at(label);
}


/**
 * Verifica se foi atribuído um valor a todas as labels identificadas.
 */
void Label::checkIntegrity() {
	for (const auto &l: this->labels)
		if (l.second == Label::UNDEFINED)
			throw "A label " + l.first + " foi usada, mas nao foi definida.";
}


/**
 * Salva a tabela de labels em um arquivo.
 */
void Label::dump(std::string fileName) {
	std::ofstream labelsFile(fileName);

	labelsFile << "=================================================" << std::endl;
	labelsFile << fileName << " LABEL TABLE FILE" << std::endl;
	labelsFile << "=================================================" << std::endl;

	labelsFile << std::left << std::setw(20) << "Label" << "Value" << std::endl;

	for (const auto &l : this->labels)
		labelsFile << std::left << std::setw(20) << l.first
			<< "0x" << std::setw(4) << std::right << std::setfill('0') << std::uppercase
			<< std::hex << (l.second & 0xFFFF) << std::setfill(' ') << std::endl;

	labelsFile.close();
}
