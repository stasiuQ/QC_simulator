#include "buffer.h"
#include <cmath>

fstream buffer::data;
fstream buffer::temp_file;
const char buffer::file_name[] = "random_data.txt";
const int buffer::bit_size = 32;   // grid of 0-1 distrubtion , changable
long long buffer::max_rand = static_cast<long long>(pow(2, (bit_size + 1)) - 1);

void buffer::init()
{
	data.open(file_name, ios::in);
	if (!data.is_open()) throw "The chamber has not been opened!";
	if (data.eof()) throw "Empty file!";
}

bool buffer::randomize()
{
	if (data.eof()) throw "Chceck the reservoir";
	bool bit;
	data >> bit;
	return bit;
}

double buffer::rand_1()
{
	double number = 0;
	for (int i = 0; i <= bit_size; i++) {
		number += buffer::randomize()*pow(2, i);
	}

	number = number/ max_rand;
	return number;
}

long long buffer::rand_int(int bits_number)
{
	double number = 0;
	for (int i = 0; i <= bits_number; i++) {
		number += buffer::randomize()*pow(2, i);
	}

	return static_cast<long long>(number);
}

void buffer::close()
{
	temp_file.open("temp_data.txt", ios::out);
	bool temp;
	while (!data.eof()) {
		data >> temp;
		temp_file << temp << " ";
	}
	temp_file.close();
	data.close();
	remove(file_name);
	rename("temp_data.txt", file_name);
}
