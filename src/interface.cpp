#include "interface.h"

#include <iostream>


Interface::Interface() {

}


Interface::~Interface() {

}


void Interface::start() {
	std::cout << "Welcome!" << std::endl;
	system("pause");
}