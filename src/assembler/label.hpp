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
	Label();
	~Label();

	void define(std::string label, unsigned address);
	void waitFor(std::string label);

	void dump(std::string fileName);
	void checkIntegrity();

private:
	std::unordered_map<std::string, unsigned int> labels;

	static const unsigned UNDEFINED = -1;
};