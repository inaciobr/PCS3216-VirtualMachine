/**
 * codeList.hpp
 * PCS 3216 - Sistemas de Programa��o - 2019
 * Bruno Brand�o In�cio
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

	std::vector<Line> list;

	std::vector<Line>::iterator begin() { return list.begin(); }
	std::vector<Line>::iterator end() { return list.end(); }
	std::vector<Line>::const_iterator begin() const { return list.begin(); }
	std::vector<Line>::const_iterator end() const { return list.end(); }
};


/**
 * Estrutura usada para descrever as linhas de c�digo assembly.
 */
struct CodeList::Line {
	Line(unsigned lineNumber, std::string source);

	void setInstruction(unsigned address, unsigned size);
	std::string str() const;

	std::string label;
	std::string mnemonic;
	std::string operand;

	unsigned address;

	unsigned codeSize;
	union {
		unsigned value;
		std::uint8_t byte[2];
	} code;

	std::string source;
	unsigned lineNumber;
};
