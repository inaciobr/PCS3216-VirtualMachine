/**
 * interface.hpp
 * PCS 3216 - Sistemas de Programa��o - 2019
 * Bruno Brand�o In�cio
 */

#pragma once
#include <string>

struct Interface {
	void start();
	void menu();

	static std::string inputFile();
};
