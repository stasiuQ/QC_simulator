#include "BB84.h"

using namespace std;

BB84::BB84(int k_size) {
	this->is_BB84 = true;
	this->key_size = k_size;
	this->actual_key_size = k_size;
	this->temp_key = vector<bool>(k_size);
	this->base = vector<bool>(k_size);
	this->key = vector<bool>(k_size);
	this->crossed = vector<bool>(k_size);
}

BB84::BB84(const BB84 & object)
{
	this->is_BB84 = true;
	this->is_B92 = false;
	this->key_size = object.key_size;
	this->temp_key = object.temp_key;
	this->key = object.key;
	this->base = object.base;
	this->actual_key_size = object.actual_key_size;
	this->crossed = object.crossed;
}

BB84::~BB84 (){
}

bool BB84::get_key(int iterator)
{
	if (iterator >= this->key_size) throw "Iterator out of range";
	return this->temp_key[iterator];
}

bool BB84::get_basis(int iterator)
{
	if (iterator >= this->key_size) throw "Iterator out of range";
	return this->base[iterator];
}

void BB84::load_key () { // wyjatki !!!!
	for (int i = 0; i < this->key_size; i++) {
		bool temp;
		temp = buffer::randomize();
		this->temp_key[i] = temp;
		this->key[i] = temp;
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
		this->key[i] = this->temp_key[i];
	}
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
		this->key[i] = this->temp_key[i];
	}
}

void BB84::spy_classic(protocol * Alice)
{
	for (int i = 0; i < this->key_size; i++) {  // rewriting Alice's public crossed tab
		this->crossed[i] = Alice->crossed[i];
	}
	this->key_reduction();
}




void BB84::compare(protocol* Bob) {

	if (this->key_size != Bob->key_size) {
		throw "Different size of keys";
		return;
	}
	for (int i = 0; i < this->key_size; i++) {
		if (this->base[i] == Bob->get_basis(i)) {
			this->crossed[i] = 0;
			Bob->crossed[i] = 0;   // zero means no crossing out
		}
		else {
			this->crossed[i] = 1;
			Bob->crossed[i] = 1;   // one means crossed out
		}
	}
}

void BB84::key_reduction()
{
	int j = 0;   // actual key iterator
	for (int i = 0; i < key_size; i++) {
		if (crossed[i] == 1)
			key.erase(key.begin() + j);
		else
			j++;
	}
	this->actual_key_size = key.size();
}

ostream & operator<<(ostream & out, const BB84& a)
{
	out << "Original key: " << endl;
	for (int i = 0; i < a.key_size; i++) {
		out << static_cast<int>(a.temp_key[i]) << " ";
	}
	out << endl << "Base: " << endl;
	for (int i = 0; i < a.key_size; i++) {
		out << static_cast<int>(a.base[i]) << " ";
	}
	out << endl << "Actual key: " << endl;
	for (int i = 0; i < a.key.size(); i++) {
		out << static_cast<int>(a.key[i]) << " ";
	}
	out << endl << endl << endl;
	return out;
}
