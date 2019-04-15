#pragma once

#include "channels.h"
#include "buffer.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
class protocol {
protected:
	int key_size;
	bool* temp_key;
	vector<bool> key;
	bool* base;
	
public:
	bool* crossed;
	virtual void load_key() = 0;
	virtual void generate_basis() = 0;
	virtual void read_quantum(quantum_channel* q_connection) = 0;
	virtual void spy_quantum(quantum_channel* q_connection) = 0;
	virtual void spy_classic(protocol* ALice) = 0;
	virtual void make_noise(int max_noise) = 0;
	virtual bool error_correction(int type) = 0;  // freind with benefits???
	virtual bool privacy_amp(int type) = 0; // freind with benefits???
	friend class quantum_channel;

};