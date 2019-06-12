#pragma once
#include "protocol.h"

using namespace std;
class B92 : public protocol {
protected:
	double alpha;
	double beta, gamma, delta;


public:

	B92(int k_size = 20, double angle = 45);
	~B92();
	bool get_key(int iterator);
	bool get_basis(int iterator);
	void load_key();
	void generate_basis();
	void read_quantum(quantum_channel* q_connection);
	void spy_quantum(quantum_channel* q_connection);
	void spy_classic(protocol* Alice);
	void compare(protocol* Bob);
	void key_reduction();
	friend ostream& operator << (ostream& out, const B92 &a);
	friend class quantum_channel;
	friend class statistics;
};

