/**
* listCode.cpp
* PCS 3216 - Sistemas de Programação - 2019
* Bruno Brandão Inácio
*/

#include "listCode.hpp"

#include <fstream>
#include <iomanip>


/**
 *
 */
ListCode::ListCode() {

}


/**
 *
 */
ListCode::~ListCode() {

}


/**
 *
 */
void ListCode::insert(Code cd) {
	this->lst.push_back(cd);

	return;
}


/**
 *
 */
void ListCode::dump(std::string fileName) {
	std::ofstream listFile(fileName);

	listFile << "=================================================" << std::endl;
	listFile << fileName << " LIST FILE" << std::endl;
	listFile << "=================================================" << std::endl;

	listFile << std::right << std::setw(5) << "LINE" << std::setw(10) << "ADDRESS"
		<< std::setw(10) << "CODE" << std::setw(5) << "" << std::left << "SOURCE" << std::endl;

	for (const auto& code : this->lst) {
		if (code.line == UNDEFINED)
			listFile << std::setw(5) << "";
		else
			listFile << std::setw(5) << std::right << code.line;

		if (code.address == UNDEFINED)
			listFile << std::setw(10) << "";
		else
			listFile << std::setw(6) << "0x" << std::setw(4) << std::setfill('0') << std::right << std::uppercase << std::hex << code.address << std::dec << std::setfill(' ');

		if (code.code == UNDEFINED)
			listFile << std::setw(10) << "";
		else if (code.code & 0xff00)
			listFile << std::setw(6) << "" << std::setw(4) << std::setfill('0') << std::right << std::uppercase << std::hex << code.code << std::dec << std::setfill(' ');
		else
			listFile << std::setw(8) << "" << std::setw(2) << std::setfill('0') << std::right << std::uppercase << std::hex << code.code << std::dec << std::setfill(' ');

		listFile << std::setw(5) << "" << std::left << code.source << std::endl;
	}

	listFile.close();
	return;
}
