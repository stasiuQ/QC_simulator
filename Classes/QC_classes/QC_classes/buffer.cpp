#include "buffer.h"

fstream buffer::data;
fstream buffer::temp_file;
const char buffer::file_name[] = "random_data.txt";

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
