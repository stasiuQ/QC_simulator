#include "channels.h"
#include "protocol.h"

using namespace std;

quantum_channel::quantum_channel(protocol * person, int k_size)
{
	this->key_size = k_size;
	this->state_key = new bool[k_size];
	this->state_base = new bool[k_size];
	for (int i = 0; i < k_size; i++) {
		this->state_key[i] = person->temp_key[i];
		this->state_base[i] = person->base[i];
	}

}

quantum_channel::~quantum_channel()
{
	delete[] this->state_key;
	delete[] this->state_base;
}

void quantum_channel::make_noise(fstream reservoir, int max_noise)
{

}
