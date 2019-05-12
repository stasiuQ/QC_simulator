#include "B92.h"

B92::B92(int k_size, double angle)  // "angle" argument given in degrees
{
	this->is_B92 = true;
	this->key_size = k_size;
	this->temp_key = vector<bool>(k_size);
	this->base = vector<bool>(k_size);
	this->key = vector<bool>(k_size);
	this->crossed = vector<bool>(k_size);
	
	this->alpha = sin(angle / 180 * PI);
	this->beta = cos((90 - angle) / 180 * PI);
	this->delta = this->alpha;
	this->gamma = sqrt(1 - pow(abs(this->delta), 2));
}

B92::~B92()
{

}

bool B92::get_key(int iterator)
{
	if (iterator >= this->key_size) throw "Iterator out of range";
	return this->temp_key[iterator];
}

bool B92::get_basis(int iterator)
{
	if (iterator >= this->key_size) throw "Iterator out of range";
	return this->base[iterator];
}

void B92::load_key()
{
	for (int i = 0; i < this->key_size; i++) {
		bool temp;
		temp = buffer::randomize();
		this->temp_key[i] = temp;
		this->key[i] = temp;
	}
}

void B92::generate_basis()
{
	for (int i = 0; i < this->key_size; i++) {
		bool temp;
		temp = buffer::randomize();
		this->base[i] = temp;
	}
}

void B92::read_quantum(quantum_channel * q_connection)
{
	for (int i = 0; i < this->key_size; i++) {
		if (this->base[i] == q_connection->state_base[i]) {
			this->key[i] = q_connection->state_key[i];
		}
		else {
			double rand_temp = buffer::rand_1();
			if (q_connection->state_base[i] == 0) {
				if (rand_temp <= (pow(abs(this->alpha), 2))) {
					this->key[i] = 0;
				}
				else {
					this->key[i] = 1;
				}
			}
			else
			{
				if (rand_temp <= (pow(abs(this->gamma), 2))) {
					this->key[i] = 0;
				}
				else {
					this->key[i] = 1;
				}
			}
		}
		if (this->key[i] == 1)
			this->crossed[i] = 0;  // no crossing
		else
			this->crossed[i] = 1;  // crossing out
	}
}

void B92::spy_quantum(quantum_channel * q_connection)
{
	for (int i = 0; i < this->key_size; i++) {
		if (this->base[i] == q_connection->state_base[i]) {
			this->key[i] = q_connection->state_key[i];
		}
		else {
			double rand_temp = buffer::rand_1();
			q_connection->state_base != q_connection->state_base;
			if (q_connection->state_base[i] == 0) {
				if (rand_temp <= (pow(abs(this->alpha), 2))) {
					this->key[i] = 0;
					q_connection->state_key[i] = this->key[i];
				}
				else {
					this->key[i] = 1;
					q_connection->state_key[i] = this->key[i];
				}
			}
			else
			{
				if (rand_temp <= (pow(abs(this->gamma), 2))) {
					this->key[i] = 0;
					q_connection->state_key[i] = this->key[i];
				}
				else {
					this->key[i] = 1;
					q_connection->state_key[i] = this->key[i];
				}
			}
		}
		if (this->key[i] == 1)
			this->crossed[i] = 0;  // no crossing
		else
			this->crossed[i] = 1;  // crossing out
	}
}

void B92::spy_classic(protocol * Alice)
{
	this->crossed = Alice->crossed;
	this->key_reduction();
}

void B92::compare(protocol * Bob)
{
	this->crossed = Bob->crossed;
}

void B92::key_reduction()
{
	int j = 0;   // actual key iterator
	for (int i = 0; i < key_size; i++) {
		if (crossed[i] == 1)
			key.erase(key.begin() + j);
		else
			j++;
	}
}

ostream & operator<<(ostream & out, const B92 & a)
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
