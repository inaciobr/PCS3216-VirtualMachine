/**
* codeList.hpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#pragma once

#include <string>
#include <vector>

class CodeList {
public:
	CodeList() {};
	~CodeList() {};

	struct Line;

	void insert(Line cd);
	void dump(std::string fileName);

private:
	std::vector<Line> list;
};

struct CodeList::Line {
	Line(std::string source, unsigned lineNumber, unsigned address, unsigned sizeCode);

	std::string source;
	unsigned lineNumber;

	std::string label;
	std::string mnemonic;
	std::string operand;

	unsigned address;
	unsigned sizeCode;

	union {
		unsigned value;
		std::uint8_t byte[2];
	} code;
};
