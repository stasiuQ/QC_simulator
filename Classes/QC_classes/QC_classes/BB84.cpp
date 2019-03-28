#include "BB84.h"

using namespace std;

BB84::BB84(int k_size) {
	this->key_size = k_size;
	this->temp_key = new bool[k_size];
	this->base = new bool[k_size];
	this->key = vector<bool>(k_size);
	this->crossed = new bool[k_size];
}

BB84::~BB84 (){
	delete[] this->temp_key;
	delete[] this->base;
	this->key.~key;
	delete[] this->crossed;
}

void BB84::load_key (fstream reservoir) { // wyjatki !!!!
	if (!reservoir.is_open()) throw "Can't touch this, check reservoir";
}

void BB84::generate_basis(fstream reservoir) {

}

void BB84::read_quantum(quantum_channel * q_connection)
{
	if (this->key_size != q_connection->key_size) throw "Quantum connection failed!";
	for (int i = 0; i < this->key_size; i++) {
		if (this->base[i] == q_connection->state_base[i]) {    // the same base, so that a state is not changed
			this->temp_key[i] = q_connection->state_key[i];
		}
		else {
			this->temp_key[i] = randomize();   // different bases, the state collapses randomly
		}
	}
}




void compare(BB84* Alice, BB84* Bob) {
	int i;
	if (Alice->key_size > Bob->key_size) {
		throw "Alice has bigger!!";
		return;
	}
	while (i<size(Alice->key)) {
		if (Alice->base[i] != Bob->base[i]) {
			Bob->crossed[i] = 0; // 0 means crossed out element of key
			Bob->key.erase(Bob->key.begin() + i);
			Alice->key.erase(Alice->key.begin() + i);
		}
		else {
			Bob->crossed[i] = 1; i++;
		}
	}
	if (size(Bob->key) > size(Alice->key)) {
		for (int j = i; j < Bob->key_size; j++) {
			Bob->key.erase(Bob->key.begin() + j);
		}
	}
}