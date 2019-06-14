#include "statistics.h"

using namespace std;

statistics::statistics(protocol * Alice, protocol * Bob)
{
}

statistics::statistics(protocol * Alice, protocol * Bob, protocol * Eve)
{
}

double statistics::QBER(protocol * Alice, protocol * person)
{
	int counter = 0;
	for (int i = 0; i < Alice->key.size(); i++) {
		if (Alice->key[i] != person->key[i])
			counter++;
	}
	return static_cast<double>(counter) / Alice->key.size();
}

void statistics::simulate_QBER_noise(protocol* Alice, protocol* Bob, double min_noise, double max_noise, double step_noise)
{
	double noise_level = min_noise;
	int no_steps = static_cast<int>((max_noise - min_noise) / step_noise);
	
	int size = Alice->key_size;
	bool B92 = Alice->is_B92;
	double angle = 0;
	if (B92)
	
	vector< vector<double> > QBER_noise;
	for (int i = 0; i < no_steps; i++) {
		delete Alice;
		delete Bob;

		if (B)
		
		Alice->load_key();
		Alice->generate_basis();

		Bob->load_key();
		Bob->generate_basis();

		quantum_channel connection(Alice);
		connection.make_noise(noise_level);

		Bob->read_quantum(&connection);
		Alice->compare(Bob);

		Alice->key_reduction();
		Bob->key_reduction();
		
		vector<double> Y;
		Y.push_back(this->QBER(Alice, Bob));
		Y.push_back(noise_level);
		QBER_noise.push_back(Y);
		noise_level += no_steps;
	}
	this->QBER_vs_noise = QBER_noise;
}

vector<double> statistics::simulate_QBER_angle(protocol * Alice, protocol * person, double min_angle, double max_angle, double step_angle)
{
	return vector<double>();
}
