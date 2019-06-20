#include "virtualMachine.hpp"

#include <iostream>
#include <fstream>


VirtualMachine::VirtualMachine() {

}


VirtualMachine::~VirtualMachine() {

}

void VirtualMachine::load(std::string fileName) {

}

void VirtualMachine::run() {
	unsigned instruction = 1;

	switch (instruction) {
	case VirtualMachine::JP:
	case VirtualMachine::JZ:
	case VirtualMachine::JN:
	case VirtualMachine::CN:
	case VirtualMachine::ADD:
	case VirtualMachine::SUB:
	case VirtualMachine::MUL:
	case VirtualMachine::DIV:
	case VirtualMachine::LD:
	case VirtualMachine::MM:
	case VirtualMachine::SC:
	case VirtualMachine::OS:
	case VirtualMachine::IO:

	default:
		throw "Instrução não reconhecida";
	}
}
