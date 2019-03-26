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
	void load_key(string reservoir);
	void generate_basis(string reservoir);
	friend void compare(BB84 *Alice, BB84* Bob);
};