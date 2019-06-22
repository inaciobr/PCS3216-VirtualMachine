#include "memory.hpp"

memory::memory(int size) {
	this->content.resize(size);
}

uint8_t memory::getIndirectValue(uint16_t pos) const {
	uint16_t address = (this->getValue(pos) << 8 | this->getValue(pos + 1)) & 0x0FFF;
	return this->getValue(address);
}

uint8_t memory::getValue(uint16_t pos) const {
	return this->content[pos];
}
