/**
* listCode.hpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#pragma once

#include <string>
#include <vector>

class ListCode {
public:
	ListCode();
	~ListCode();

	struct Line {
		unsigned lineNumber;
		unsigned address;

		union {
			unsigned value;
			std::uint8_t byte[2];
		} code;

		std::string source;
	};

	void insert(Line cd);
	void dump(std::string fileName);

	static const unsigned UNDEFINED = -1;

private:

	std::vector<Line> lst;
};
