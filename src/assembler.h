#pragma once

#include <string>



class Assembler {
public:
	Assembler();
	~Assembler();

	void run(std::string fileName);

	static const struct translation {
		std::string name;
		int code;
	} mnemonics[];
};
