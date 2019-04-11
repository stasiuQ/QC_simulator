#pragma once
#include "protocol.h"
#include "BB84.h"

using namespace std;

class quantum_channel {
protected:
	int key_size;
	bool* state_key;
	bool* state_base;

public:
	quantum_channel(protocol* person, int k_size);
	~quantum_channel();
	void make_noise(fstream reservoir, int max_noise);

	friend class protocol;
	friend class BB84;

};