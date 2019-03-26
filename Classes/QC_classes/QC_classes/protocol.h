#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
class protocol {
public:
	virtual void load_key(string reservoir) = 0;
	virtual void generate_basis(string reservoir) = 0;
	virtual void read_quantum(bool** message) = 0;
	virtual void spy() = 0;
	virtual void make_noise(string reservoir, int max_noise) = 0;
	virtual bool error_correction(int type) = 0;  // freind with benefits???
	virtual bool privacy_amp(int type) = 0; // freind with benefits???

};