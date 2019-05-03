#pragma once

#include <string>
#include <map>


class Assembler {
public:
	Assembler(std::string fileName);
	~Assembler();

	void assemble();

	static const std::map<const char*, unsigned int> mnemonics;

private:
	std::string inputFile;
	std::string outputFile;
};
