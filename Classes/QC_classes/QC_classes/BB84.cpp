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
	delete[] this->crossed; 
}

void BB84::load_key () { // wyjatki !!!!
	for (int i = 0; i < this->key_size; i++) {
		bool temp;
		temp = buffer::randomize();
		this->temp_key[i] = temp;
		this->key.push_back(temp);
	}
}

void BB84::generate_basis() {
	for (int i = 0; i < this->key_size; i++) {
		bool temp;
		temp = buffer::randomize();
		this->base[i] = temp;
	}
}

void BB84::read_quantum(quantum_channel * q_connection)
{
	if (this->key_size != q_connection->key_size) throw "Quantum connection failed!";
	for (int i = 0; i < this->key_size; i++) {
		if (this->base[i] == q_connection->state_base[i]) {    // the same base, so that a state is not changed
			this->temp_key[i] = q_connection->state_key[i];
		}
		else {
			this->temp_key[i] = buffer::randomize();   // different bases, the state collapses randomly
		}
	}
	delete q_connection;
}

void BB84::spy_quantum(quantum_channel * q_connection)
{
	if (this->key_size != q_connection->key_size) throw "Spy failed";
	for (int i = 0; i < this->key_size; i++) {
		if (this->base[i] == q_connection->state_base[i]) {    // the same base, so that a state is not changed
			this->temp_key[i] = q_connection->state_key[i];
		}
		else {
			this->temp_key[i] = buffer::randomize();   // different bases, the state collapses randomly
			q_connection->state_base[i] = this->base[i];
			q_connection->state_key[i] = this->temp_key[i];
		}
	}
}

void BB84::spy_classic(protocol * Alice)
{
	for (int i = 0; i < this->key_size; i++) {  // rewriting Alice's public crossed tab
		this->crossed[i] = Alice->crossed[i];
	}
	int i = 0;
	int j = 0;
	while (i < this->key_size) {
		if (this->crossed[i] == 0) {
			this->key.erase(this->key.begin() + i - j);
			j++;
		}
		i++;
	}
}




void compare(BB84* Alice, BB84* Bob) {
	int i = 0;  // key iterator
	int j = 0; // crossed iterator
	if (Alice->key_size != Bob->key_size) {
		throw "Different size of keys";
		return;
	}
	while (i < static_cast<int>(Alice->key.size())) {
		if (Alice->base[i] != Bob->base[i]) {
			Bob->crossed[j] = 0; // 0 means crossed out element of key
			Bob->key.erase(Bob->key.begin() + i);
			Alice->key.erase(Alice->key.begin() + i);
		}
		else {
			Bob->crossed[i] = 1; i++;
		}
		j++;
	}
}

ostream & operator<<(ostream & out, const BB84 & a)
{
	out << "Original key: " << endl;
	for (int i = 0; i < a.key_size; i++) {
		out << a.temp_key[i] << " ";
	}
	out << endl << "Base: " << endl;
	for (int i = 0; i < a.key_size; i++) {
		out << a.base[i] << " ";
	}
	out << endl << "Actual key: " << endl;
	for (int i = 0; i < a.key.size(); i++) {
		out << a.key[i] << " ";
	}
	out << endl;
	return out;
}
