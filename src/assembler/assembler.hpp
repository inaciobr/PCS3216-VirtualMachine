/**
* assembler.hpp
* PCS 3216 - Sistemas de Programa��o - 2019
* Bruno Brand�o In�cio
*/

#pragma once

#include <string>
#include <map>

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
	};

	static const std::map<std::string, Instruction> mnemonics;

private:
	void runStep(bool step);
	void makeObject();

	Label labels;
	ListCode list;

	std::string inputFile;
	std::string outputFile;
};
