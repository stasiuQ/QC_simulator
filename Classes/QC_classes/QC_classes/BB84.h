#pragma once
#include "protocol.h"

using namespace std;
class BB84 : public protocol {
protected:
	int key_size;
	bool* temp_key;
	vector<bool> key;
	bool* base;
	bool* crossed;

public:
	BB84(int k_size = 20);
	~BB84();
	void load_key(fstream reservoir);
	void generate_basis(fstream reservoir);
	void read_quantum(quantum_channel* q_connection);
	friend void compare(BB84 *Alice, BB84* Bob);
};