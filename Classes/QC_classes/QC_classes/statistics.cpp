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
	bool is_B92 = Alice->is_B92;
	double angle = 0;
	if (is_B92) {
		angle = dynamic_cast<B92*>(Alice)->alpha;
	}
	
	vector< vector<double> > QBER_noise;
	vector< vector<double> > QBER_estimation;
	vector< vector<double> > QBER_correction;


	for (int i = 0; i < no_steps; i++) {
		delete Alice;
		delete Bob;

		if (is_B92) {
			Alice = new B92(size, angle);
			Bob = new B92(size, angle);
		}
		else {
			Alice = new BB84(size);
			Bob = new BB84(size);
		}
		
		//****Communication*****//
		Alice->load_key();
		if(!is_B92)
			Alice->generate_basis();

		Bob->load_key();
		Bob->generate_basis();

		quantum_channel connection(Alice);
		connection.make_noise(noise_level);

		Bob->read_quantum(&connection);
		Alice->compare(Bob);

		Alice->key_reduction();
		Bob->key_reduction();

		//**** End of communication ****//

		vector<double> Y;  // filling real QBER before correction
		Y.push_back(noise_level);
		Y.push_back(this->QBER(Alice, Bob));
		QBER_noise.push_back(Y);

		// error estimation, reducing key lenght
		connection.error_estimation(Alice, Bob, 50 );
		vector<double> X;
		X.push_back(noise_level);
		X.push_back(connection.QBER_est);
		QBER_estimation.push_back(X);

		// starting error correction
		long long perm = buffer::rand_int(5);
		connection.Cascade(Alice, Bob, 0.73, 10);
		vector<double> Z;
		Z.push_back(noise_level);
		Z.push_back(this->QBER(Alice, Bob));
		QBER_correction.push_back(Z);

		noise_level += no_steps;
	}
	this->QBER_vs_noise = QBER_noise;
	this->QBER_estimation_vs_noise = QBER_estimation;
	this->QBER_correction_vs_noise = QBER_correction;
}

vector<double> statistics::simulate_QBER_angle(protocol * Alice, protocol * person, double min_angle, double max_angle, double step_angle)
{
	return vector<double>();
}
