/**
* assembler.hpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#pragma once

#include <string>
#include <map>
#include <functional>

#include "label.hpp"
#include "ListCode.hpp"


class Assembler {
public:
	Assembler(std::string fileName);
	~Assembler();

	std::string assemble();

	struct Instruction {
		unsigned int code;
		unsigned int size;
		unsigned int mask;
	};

	static const std::map<std::string, Instruction> mnemonics;
	static const std::map<std::string, Instruction> pseudo;

private:
	void runStep(bool step);
	void makeObject();

	int operandValue(std::string, bool step);

	Label labels;
	ListCode list;

	std::string inputFile;
	std::string outputFile;

	std::vector<uint8_t> code;
};
