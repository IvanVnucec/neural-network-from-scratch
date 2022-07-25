#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>
#include <cassert>
#include <chrono>
#include <Eigen/Dense>

using namespace std::chrono;

constexpr int msb_to_int(int a0, int a1, int a2, int a3)
{
	return (a0 << 24) | (a1 << 16) | (a2 << 8) | (a3 << 0);
}

std::vector<uint8_t> load_labels(std::string path)
{
	int a0, a1, a2, a3;

	std::ifstream stream(path, std::ios::in | std::ios::binary);
	assert(stream.good());

	a0 = stream.get();
	a1 = stream.get();
	a2 = stream.get();
	a3 = stream.get();
	int mag_num = msb_to_int(a0, a1, a2, a3);
	assert(mag_num == 2049);

	a0 = stream.get();
	a1 = stream.get();
	a2 = stream.get();
	a3 = stream.get();
	int num_of_items = msb_to_int(a0, a1, a2, a3);
	assert(num_of_items == 60000);
	
	return std::vector<uint8_t>((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
}

std::vector<Eigen::MatrixXi> load_images(std::string path)
{
	int a0, a1, a2, a3;
	std::vector<Eigen::MatrixXi> images;

	std::ifstream stream(path, std::ios::in | std::ios::binary);
	assert(stream.good());

	a0 = stream.get();
	a1 = stream.get();
	a2 = stream.get();
	a3 = stream.get();
	int mag_num = msb_to_int(a0, a1, a2, a3);
	assert(mag_num == 2051);

	a0 = stream.get();
	a1 = stream.get();
	a2 = stream.get();
	a3 = stream.get();
	int num_of_images = msb_to_int(a0, a1, a2, a3);
	assert(num_of_images == 60000);
	images.reserve(num_of_images);

	a0 = stream.get();
	a1 = stream.get();
	a2 = stream.get();
	a3 = stream.get();
	int rows = msb_to_int(a0, a1, a2, a3);
	assert(rows == 28);

	a0 = stream.get();
	a1 = stream.get();
	a2 = stream.get();
	a3 = stream.get();
	int cols = msb_to_int(a0, a1, a2, a3);
	assert(cols == 28);
	
	std::vector<uint8_t> items((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	assert(not items.empty());

	for (int k = 0; k < num_of_images; k++) {
		Eigen::MatrixXi img(rows, cols);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				img(i, j) = static_cast<int>(items[k * rows * cols + i*cols + j]);
			}
		}

		images.push_back(img);
	}

	return images;
}

int main()
{
	auto start = high_resolution_clock::now();

	std::vector<uint8_t> labels = load_labels("dataset/train-labels.idx1-ubyte");
	std::vector<Eigen::MatrixXi> images = load_images("dataset/train-images.idx3-ubyte");

	assert(labels[0] == 5);
	assert(images[0](14, 14) == 240);

	assert(labels[1] == 0);
	assert(images[1](5, 14) == 48);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << "Time taken by function: " << duration.count() << " ms" << std::endl;

	return 0;
}
