#include "virtualMachine.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <fstream>

VirtualMachine::VirtualMachine(unsigned int numBanks, unsigned int bankSize) :
	currentByte(0), accumulator(0), indirectMode(false), currentBank(0)
{
	this->mainMemory.resize(numBanks, bankSize);

	std::ifstream loader("system/loader.bin", std::ifstream::binary);
	char header[3];
	loader.read((char *)&header, 3);
	this->instructionCounter = header[0] << 8 | header[1];

	std::vector<char> buffer(header[2]);
	loader.read(buffer.data(), header[2]);

	for (const auto &v: buffer)
		this->mainMemory[0].setValue(instructionCounter++, v & 0xFF);

	loader.close();
}


void VirtualMachine::load(std::string fileName) {

}

void VirtualMachine::run() {
	unsigned instruction = 1;
}

void VirtualMachine::printPointers() const {
	std::cout << "Instruction counter: 0x" << std::hex << std::setfill('0') << std::setw(4) << this->instructionCounter << std::endl;
	std::cout << "Acumulator: 0x" << std::hex << std::setfill('0') << std::setw(2) << this->accumulator << std::endl;
	std::cout << "Current Byte: 0x" << std::hex << std::setfill('0') << std::setw(2) << this->currentByte << std::endl;
	std::cout << "Current Bank: 0x" << std::hex << std::setfill('0') << std::setw(2) << this->currentBank << std::endl;
}

void VirtualMachine::printMemoryBanks() const {
	std::cout << std::to_string(this->mainMemory.size()) << " bancos de memoria ativos." << std::endl;

}

void VirtualMachine::printMemory(int addr, int size) const {
	std::cout << "      " << std::setfill('0') << std::hex << std::uppercase;

	for (int i = 0; i < 16; i++)
		std::cout << i << "   ";

	std::cout << std::endl;

	if (size & 0xF)
		size += 0x10;

	for (int i = 0; i < size >> 4; i++) {
		std::cout << "0x" << std::setw(2) << (addr&0x0FF0) + i << " ";
		for (int j = 0; j < 16; j++)
			std::cout << std::setw(2) << (int)this->mainMemory[addr >> 12].getValue((addr&0x0FF0) + j + (i << 4)) << "  ";

		std::cout << std::endl;
	}
}


uint16_t VirtualMachine::getOperand2Bytes() {
	return static_cast<uint16_t>((this->currentByte << 8) | this->nextByte()) & 0x0FFF;
}


void VirtualMachine::updatePC(uint16_t operand) {
	if (this->indirectMode) {
		this->indirectMode = false;
		this->instructionCounter = (this->mainMemory[this->currentBank].getValue(operand) << 8
									 | this->mainMemory[this->currentBank].getValue(operand + 1)) & 0x0FFF;
		this->currentBank = this->instructionCounter >> 12;

		return;
	}
	this->instructionCounter = operand;
}


void VirtualMachine::runInstruction(uint8_t type) {
	try {
		(this->*VirtualMachine::instructions.at(type & 0xF0))();
	}
	catch (const std::out_of_range) {
		throw std::string("A instrução " + std::to_string(type) + " não foi reconhecida.");
	}
}


uint8_t VirtualMachine::nextByte() {
	this->currentByte = this->mainMemory[this->currentBank].getValue(this->instructionCounter + 1);
	return this->currentByte;
}


uint8_t VirtualMachine::getIndirectValue(uint16_t pos) {
	if (this->indirectMode) {
		this->indirectMode = false;
		return this->mainMemory[this->currentBank].getIndirectValue(pos);
	}

	return this->mainMemory[this->currentBank].getValue(pos);
}


// INSTRUCTIONS
/*
* Jump
*/
void VirtualMachine::JP() {
	this->updatePC(this->getOperand2Bytes());
}


/*
* Jump on zero
*/
void VirtualMachine::JZ() {
	if (!this->accumulator)
		this->updatePC(this->getOperand2Bytes());
}


/*
* Jump on negative
*/
void VirtualMachine::JN() {
	if (this->accumulator < 0)
		this->updatePC(this->getOperand2Bytes());
}

/*
*
*/
void VirtualMachine::CN() {
	uint8_t operand = this->currentByte & 0x0F;
}


/*
* Add
*/
void VirtualMachine::ADD() {
	this->accumulator += this->getIndirectValue(this->getOperand2Bytes());
}


/*
* Subtraction
*/
void VirtualMachine::SUB() {
	this->accumulator -= this->getIndirectValue(this->getOperand2Bytes());
}


/*
* Multiplication
*/
void VirtualMachine::MUL() {
	this->accumulator *= this->getIndirectValue(this->getOperand2Bytes());
}


/*
* Division
*/
void VirtualMachine::DIV() {
	this->accumulator /= this->getIndirectValue(this->getOperand2Bytes());
}


/*
* Load
*/
void VirtualMachine::LD() {
	this->accumulator = this->getIndirectValue(this->getOperand2Bytes());
}


/*
*
*/
void VirtualMachine::MM() {
	uint16_t operand = this->getOperand2Bytes();
}


/*
*
*/
void VirtualMachine::SC() {
	uint16_t operand = this->getOperand2Bytes();
}


/*
*
*/
void VirtualMachine::OS() {
	uint8_t operand = this->currentByte & 0x0F;
}


/*
*
*/
void VirtualMachine::IO() {
	uint8_t operand = this->currentByte & 0x0F;
}
