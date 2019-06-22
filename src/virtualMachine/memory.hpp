#pragma once

#include <vector>

class memory {
public:
	memory(int size);
	uint8_t getValue(uint16_t pos) const;
	uint8_t setValue(uint16_t pos, uint8_t value);
	uint8_t getIndirectValue(uint16_t pos) const;

	void dump();

private:
	std::vector<std::uint8_t> content;
};
