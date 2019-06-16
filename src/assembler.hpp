/**
* assembler.hpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#pragma once

#include <string>
#include <map>


class Assembler {
public:
	Assembler(std::string fileName);
	~Assembler();

	void assemble();

	void dumpLabelTable();

	void createListFile();
	void addToListFile(unsigned line = 0, unsigned address = -1, unsigned code = 0xfc, std::string source = "");


private:
	void run(bool step);


	std::string inputFile;
	std::string outputFile;

	std::map<std::string, unsigned int> labels;


	struct instruction {
		unsigned int code;
		unsigned int size;
	};

	static const std::map<const char*, instruction> mnemonics;
};
