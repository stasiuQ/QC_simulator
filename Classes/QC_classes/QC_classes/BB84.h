#pragma once
#include "protocol.h"

using namespace std;
class BB84 : public protocol {
protected:
	int key_size;
	bool* temp_key;  //original key, from randomization
	vector<bool> key;   // operational key
	bool* base;
	

public:
	bool* crossed;
	BB84(int k_size = 20);
	~BB84();
	void load_key();
	void generate_basis();
	void read_quantum(quantum_channel* q_connection);
	void spy_quantum(quantum_channel* q_connection);
	void spy_classic(protocol* Alice);
	friend void compare(BB84* Alice, BB84* Bob);
	friend ostream& operator << (ostream& out, const BB84 &a);
	friend class quantum_channel;
};

void compare(BB84* Alice, BB84* Bob);