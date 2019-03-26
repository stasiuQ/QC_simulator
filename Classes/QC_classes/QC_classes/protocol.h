#pragma once
#include <fstream>

using namespace std;
class protocol {
public:
	virtual void load_key(fstream data_file) = 0;
	virtual void generate_basis(fstream data_file) = 0;
	virtual void read_quantum(bool** message) = 0;
	virtual void spy () = 0;
};