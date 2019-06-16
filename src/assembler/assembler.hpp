/**
* assembler.hpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#pragma once

#include <string>
#include <map>

#include "label.hpp"


class Assembler {
public:
	Assembler(std::string fileName);
	~Assembler();

	std::string assemble();

private:
	void run(bool step);

	void makeObject();
	void createListFile();

	Label labels;



	struct listCode {
		unsigned line = 0;
		unsigned address = -1;
		unsigned code = -1;
		std::string source = "";
	};

	void addToListFile(listCode lst);


	std::string inputFile;
	std::string outputFile;


	struct instruction {
		unsigned int code;
		unsigned int size;
	};

	static const std::map<const char*, instruction> mnemonics;
};
