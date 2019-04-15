#include "buffer.h"

fstream buffer::data;

void buffer::init(string file_name)
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
	data.close();
}
