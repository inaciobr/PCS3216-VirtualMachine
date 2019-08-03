/**
* label.hpp
* PCS 3216 - Sistemas de Programa��o - 2019
* Bruno Brand�o In�cio
*/

#pragma once

#include <string>
#include <unordered_map>

class Label {
public:
	Label() {};
	~Label() {};

	void waitFor(std::string label);
	void define(std::string label, unsigned int address);
	
	unsigned getValue(std::string label);
	
	void dump(std::string fileName);
	void checkIntegrity();

	static constexpr unsigned UNDEFINED = -1;

private:
	std::unordered_map<std::string, unsigned> labels;
};
