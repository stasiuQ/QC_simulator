#pragma once

#include <fstream>
#include <string>

using namespace std;

class buffer {
private:
	static fstream data;
	static fstream temp_file;
	static const char file_name[];
public:
	static void init();
	static bool randomize();
	static void close();
};