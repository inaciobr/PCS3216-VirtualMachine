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

	Line insert(Line line);
	void dump(std::string fileName);

private:
	std::vector<Line> list;
};

struct CodeList::Line {
	Line(unsigned lineNumber, std::string source);

	void setCode(unsigned address, unsigned size, unsigned code);
	std::string str() const;

	std::string label;
	std::string mnemonic;
	std::string operand;

	unsigned address;

	union {
		unsigned value;
		std::uint8_t byte[2];
	} code;

private:
	std::string source;
	unsigned lineNumber;

	unsigned codeSize;
};
