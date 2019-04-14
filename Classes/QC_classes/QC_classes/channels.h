#pragma once

#include <fstream>

using namespace std;

class protocol;
class BB84;

class quantum_channel {
protected:
	int key_size;
	bool* state_key;
	bool* state_base;

public:
	quantum_channel(protocol* person, int k_size);
	~quantum_channel();
	void make_noise(fstream& reservoir, int max_noise);

	friend class protocol;
	friend class BB84;

};