#pragma once

#include <string>
#include <map>


class Assembler {
public:
	Assembler();
	~Assembler();

	void assemble(std::string fileName);

	static const std::map<const char*, unsigned int> mnemonics;
};
