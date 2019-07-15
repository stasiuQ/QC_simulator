#pragma once
#include "protocol.h"

using namespace std;
class BB84 : public protocol {
protected:
	//int key_size;
	//bool* temp_key;  //original key, from randomization
	//vector<bool> key;   // operational key
	//bool* base;
	

public:
	//bool* crossed
	
	BB84(int k_size = 20);
	BB84(const BB84& object);  // copying constructor
	~BB84();
	bool get_key(int iterator);
	bool get_basis(int iterator);
	void load_key();
	void generate_basis();
	void read_quantum(quantum_channel* q_connection);
	void spy_quantum(quantum_channel* q_connection);
	void spy_classic(protocol* Alice);
	void compare(protocol* Bob);
	void key_reduction();
	friend ostream& operator << (ostream& out, const BB84 &a);
	friend class quantum_channel;
	friend class statistics;
};
