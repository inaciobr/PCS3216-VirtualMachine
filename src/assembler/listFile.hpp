/**
* listFile.hpp
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

	struct Code {
		unsigned line;
		unsigned address;
		unsigned code;
		std::string source;
	};

	void addToList(Code cd);
	void dumpList(std::string fileName)

	static const unsigned UNDEFINED = -1;

private:

	std::vector<Code> lst;
};
