/**
* listFile.hpp
* PCS 3216 - Sistemas de Programa��o - 2019
* Bruno Brand�o In�cio
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
