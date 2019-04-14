#pragma once

#include <fstream>
#include <string>

using namespace std;

class buffer {
private:
	static fstream data;
public:
	static void init(string file_name);
	static bool randomize();
	static void close();
};