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

std::vector<int> load_labels(std::string path)
{
	std::vector<int> labels;

	std::ifstream stream(path, std::ios::in | std::ios::binary);
	assert(stream.good());

	std::vector<uint8_t> items((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	assert(not items.empty());

	int mag_num = msb_to_int(items[0], items[1], items[2], items[3]);
	assert(mag_num == 2049);

	int num_of_items = msb_to_int(items[4], items[5], items[6], items[7]);
	assert(num_of_items == 60000);

	labels.reserve(num_of_items);

	for (int i = 0; i < num_of_items; i++) {
		labels.push_back(static_cast<int>(items[i+8]));
	}

	return labels;
}

int main()
{
	std::vector<int> labels = load_labels("dataset/train-labels.idx1-ubyte");

	return 0;
}
