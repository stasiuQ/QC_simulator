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

vector<double> statistics::simulate_QBER_noise(protocol * Alice, protocol * person, double min_noise, double max_noise, double step_noise)
{
	double noise_level = min_noise;
	int no_steps = static_cast<int>((max_noise - min_noise) / step_noise);

	vector<double> QBER;
	for (int i = 0; i < no_steps; i++){
		
	}
	return QBER;
}

vector<double> statistics::simulate_QBER_angle(protocol * Alice, protocol * person, double min_angle, double max_angle, double step_angle)
{
	return vector<double>();
}
