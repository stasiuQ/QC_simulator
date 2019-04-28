#pragma once

#include <fstream>
#include <vector>

using namespace std;

class protocol;
class BB84;

class quantum_channel {
protected:
	int key_size;
	vector<bool> state_key;
	vector<bool> state_base;

public:
	quantum_channel(protocol* person);
	~quantum_channel();
	void make_noise(double max_noise);

	friend class protocol;
	friend class BB84;

};