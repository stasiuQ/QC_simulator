#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "channels.h"

using namespace std;
class protocol {
protected:
	int key_size;
	bool* temp_key;
	vector<bool> key;
	bool* base;
	bool* crossed;
public:
	virtual void load_key(fstream reservoir) = 0;
	virtual void generate_basis(fstream reservoir) = 0;
	virtual void read_quantum(quantum_channel* q_connection) = 0;
	virtual void spy() = 0;
	virtual void make_noise(fstream reservoir, int max_noise) = 0;
	virtual bool error_correction(int type) = 0;  // freind with benefits???
	virtual bool privacy_amp(int type) = 0; // freind with benefits???
	friend class quantum_channel;

};