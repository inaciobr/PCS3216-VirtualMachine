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
void ListCode::insert(Line cd) {
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

	for (const auto& l : this->lst) {
		if (!l.lineNumber)
			continue;

		listFile << std::setw(5) << std::right << l.lineNumber;

		if (l.sizeCode) {
			listFile << std::setw(6) << "0x" << std::setw(4) << std::setfill('0') << std::uppercase << std::hex << l.address << std::setfill(' ');
			
			if (l.sizeCode == 1)
				listFile << std::setw(9) << "" << std::setfill('0') << std::uppercase << std::setw(2) << static_cast<unsigned>(l.code.byte[0]);
			else
				listFile << std::setw(6) << "" << std::setfill('0') << std::uppercase << std::setw(2) << static_cast<unsigned>(l.code.byte[1]) << " " << std::setw(2) << static_cast<unsigned>(l.code.byte[0]);

			listFile << std::dec << std::setfill(' ');
		}
		else {
			listFile << std::setw(20) << "";
		}

		listFile << std::setw(5) << "" << std::left << l.source << std::endl;
	}

	listFile.close();
	return;
}
