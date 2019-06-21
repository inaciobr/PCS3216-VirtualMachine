/**
* listCode.hpp
* PCS 3216 - Sistemas de Programa��o - 2019
* Bruno Brand�o In�cio
*/

#pragma once

#include <string>
#include <vector>

class ListCode {
public:
	ListCode() {};
	~ListCode() {};

	struct Line {
		unsigned int lineNumber;
		std::string source;

		unsigned int address;
		unsigned int sizeCode;

		union {
			unsigned int value;
			std::uint8_t byte[2];
		} code;
	};

	void insert(Line cd);
	void dump(std::string fileName);

private:

	std::vector<Line> lst;
};
