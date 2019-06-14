#pragma once

#include "channels.h"
#include "buffer.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

constexpr auto PI = 3.14159265358979323846;
using namespace std;
class protocol {
protected:
	bool is_BB84 = false;
	bool is_B92 = false;
	vector<bool> temp_key;  // original randomly generated key 
	vector<bool> key;		// actual key
	vector<bool> base;		// randomly generated basis
	int actual_key_size;    // actual, operative key size
	
public:
	int key_size;
	vector<bool> crossed;
	virtual bool get_key(int iterator) = 0;
	virtual bool get_basis(int iterator) = 0;
	virtual void load_key() = 0;
	virtual void generate_basis() = 0;
	virtual void read_quantum(quantum_channel* q_connection) = 0;
	virtual void compare(protocol* Bob)= 0;
	virtual void key_reduction() = 0;
	virtual void spy_quantum(quantum_channel* q_connection) = 0;
	virtual void spy_classic(protocol* ALice) = 0;
	//virtual bool error_correction(int type) = 0;  // freind with benefits???
	//virtual bool privacy_amp(int type) = 0; // freind with benefits???
	friend class quantum_channel;
	friend class statistics;
};