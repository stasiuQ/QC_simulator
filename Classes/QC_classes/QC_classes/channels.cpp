#include "channels.h"
#include "protocol.h"

using namespace std;

quantum_channel::quantum_channel(protocol * person)
{
	this->key_size = person->key_size;
	this->state_key = person->key;
	this->state_base = person->base;
}

quantum_channel::~quantum_channel()
{
}

void quantum_channel::make_noise(fstream& reservoir, int max_noise)
{

}
