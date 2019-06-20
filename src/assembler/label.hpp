/**
* label.hpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#pragma once

#include <string>
#include <unordered_map>

class Label {
public:
	Label() {};
	~Label() {};

	void define(std::string label, unsigned int address);
	void waitFor(std::string label);
	unsigned int getValue(std::string label);

	void dump(std::string fileName);
	void checkIntegrity();

	static const unsigned UNDEFINED = -1;

private:
	std::unordered_map<std::string, unsigned> labels;
};
