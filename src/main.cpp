#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>
#include <cassert>

constexpr int msb_to_int(uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3)
{
	return (a0 << 24) | (a1 << 16) | (a2 << 8) | (a3 << 0);
}

int main()
{
	std::ifstream stream("dataset/train-labels.idx1-ubyte", std::ios::in | std::ios::binary);
	assert(stream.good());

	std::vector<uint8_t> labels((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	assert(not labels.empty());

	int mag_num = msb_to_int(labels[0], labels[1], labels[2], labels[3]);
	assert(mag_num == 2049);

	int num_of_items = msb_to_int(labels[4], labels[5], labels[6], labels[7]);
	assert(num_of_items == 60000);

	return 0;
}
