/**
 * interface.hpp
 * PCS 3216 - Sistemas de Programação - 2019
 * Bruno Brandão Inácio
 */

#pragma once
#include <string>

struct Interface {
	void start();
	void menu();

	static std::string inputFile();
};
