#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>
#include <cassert>
#include <Eigen/Dense>

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

std::vector<Eigen::MatrixXi> load_images(std::string path)
{
	std::vector<Eigen::MatrixXi> images;

	std::ifstream stream(path, std::ios::in | std::ios::binary);
	assert(stream.good());

	std::vector<uint8_t> items((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	assert(not items.empty());

	int mag_num = msb_to_int(items[0], items[1], items[2], items[3]);
	assert(mag_num == 2051);

	int num_of_images = msb_to_int(items[4], items[5], items[6], items[7]);
	assert(num_of_images == 60000);
	images.reserve(num_of_images);

	int rows = msb_to_int(items[8], items[9], items[10], items[11]);
	assert(rows == 28);

	int cols = msb_to_int(items[12], items[13], items[14], items[15]);
	assert(cols == 28);

	for (int k = 0; k < num_of_images; k++) {
		Eigen::MatrixXi img(rows, cols);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				img(i, j) = static_cast<int>(items[k * rows * cols + i*cols + j + 16]);
			}
		}

		images.push_back(img);
	}

	return images;
}

int main()
{
	std::vector<int> labels = load_labels("dataset/train-labels.idx1-ubyte");
	std::vector<Eigen::MatrixXi> images = load_images("dataset/train-images.idx3-ubyte");

	std::cout << labels[0] << std::endl;
	std::cout << images[0] << std::endl;

	std::cout << labels[1] << std::endl;
	std::cout << images[1] << std::endl;

	return 0;
}
