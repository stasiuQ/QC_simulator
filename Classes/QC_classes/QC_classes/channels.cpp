#include "channels.h"
#include "protocol.h"

using namespace std;

quantum_channel::quantum_channel(protocol * person)
{
	this->key_size = person->key_size;

	if (person->is_BB84) {
		this->state_key = person->key;
		this->state_base = person->base;
	}
	else if (person->is_B92) {
		this->state_base = person->key;
		for (int i = 0; i < this->key_size; i++) {
			this->state_key.push_back(0);
		}
	}
}

quantum_channel::~quantum_channel()
{
}

void quantum_channel::make_noise(double max_noise)    // max_noise is a maximal percentage of changed bits
{
	if (max_noise < 0 || max_noise > 100)
		throw "Inappropriate max noise parameter";
	unsigned int rand_iterator;
	bool temp;
	for (int i = 0; i < static_cast<int>(max_noise*key_size/100); i++) {
		rand_iterator = static_cast<unsigned int>(buffer::rand_1()*(key_size));
		if (rand_iterator == key_size)
			rand_iterator--;
		temp = buffer::randomize();
		if (temp == true) {
			state_base[rand_iterator] = (state_base[rand_iterator] + 1) % 2;
			temp = buffer::randomize();
			state_key[rand_iterator] = temp;
		}
	}
}
