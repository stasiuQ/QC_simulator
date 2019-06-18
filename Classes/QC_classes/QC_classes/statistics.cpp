#include "statistics.h"

using namespace std;

statistics::statistics(protocol * Alice, protocol * Bob, double noise_level)
{
	this->execute_communication(Alice, Bob, noise_level);
}

statistics::statistics(protocol * Alice, protocol * Bob, protocol * Eve, double noise_level)
{
	this->execute_communication(Alice, Bob, Eve, noise_level);
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

void statistics::execute_communication(protocol * Alice, protocol * Bob, double noise_level)
{
	//****Communication*****//
	Alice->load_key();
	if (Alice->is_BB84)
		Alice->generate_basis();

	Bob->load_key();
	Bob->generate_basis();

	this->initial_key_size = Alice->key_size;

	quantum_channel connection(Alice);
	connection.make_noise(noise_level);

	Bob->read_quantum(&connection);
	Alice->compare(Bob);

	Alice->key_reduction();
	Bob->key_reduction();

	this->key_lenght = Alice->actual_key_size;
	this->QBER_S = QBER(Alice, Bob);

	//* Error correction algorithm *//
	connection.error_estimation(Alice, Bob, 70);
	this->QBER_est = connection.QBER_est;
	long long perm = buffer::rand_int(5);
	connection.Cascade(Alice, Bob, 0.5, 5);
	connection.privacy_amp(Alice, Bob, 20);   // problem with parameters !!!!

	this->key_lenght_correction = Alice->actual_key_size;
	this->QBER_S_correction = QBER(Alice, Bob);
}

void statistics::execute_communication(protocol * Alice, protocol * Bob, protocol * Eve, double noise_level)
{
	//****Communication*****//
	Alice->load_key();
	if (Alice->is_BB84)
		Alice->generate_basis();

	Eve->load_key();
	Eve->generate_basis();
	
	Bob->load_key();
	Bob->generate_basis();

	this->initial_key_size = Alice->key_size;

	quantum_channel connection(Alice);
	connection.make_noise(noise_level);

	Eve->spy_quantum(&connection);

	Bob->read_quantum(&connection);
	Alice->compare(Bob);

	Eve->spy_classic(Alice);
	Alice->key_reduction();
	Bob->key_reduction();

	this->key_lenght = Alice->actual_key_size;
	this->QBER_S = QBER(Alice, Bob);

	//* Error correction algorithm *//
	connection.error_estimation(Alice, Bob, 70);
	this->QBER_est = connection.QBER_est;
	long long perm = buffer::rand_int(5);
	connection.Cascade(Alice, Bob, 0.5, 5);
	connection.privacy_amp(Alice, Bob, 20);  // problem with parameters!

	this->key_lenght_correction = Alice->actual_key_size;
	this->QBER_S_correction = QBER(Alice, Bob);
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
		connection.Cascade(Alice, Bob, 0.73, 10);
		vector<double> Z;
		Z.push_back(noise_level);
		Z.push_back(this->QBER(Alice, Bob));
		QBER_correction.push_back(Z);

		noise_level += step_noise;
	}
	this->QBER_vs_noise = QBER_noise;
	this->QBER_estimation_vs_noise = QBER_estimation;
	this->QBER_correction_vs_noise = QBER_correction;
}

void statistics::simulate_QBER_angle(protocol * Alice, protocol * Bob, double min_angle, double max_angle, double step_angle, double noise_level)
{
	double angle = min_angle;
	int no_steps = static_cast<int>((max_angle - min_angle) / step_angle);

	int size = Alice->key_size;
	bool is_B92 = Alice->is_B92;
	if (!is_B92) {
		throw "Invalid protocol!";
	}

	vector< vector<double> > QBER_noise;
	vector< vector<double> > QBER_estimation;
	vector< vector<double> > QBER_correction;


	for (int i = 0; i < no_steps; i++) {
		delete Alice;
		delete Bob;

		Alice = new B92(size, angle);
		Bob = new B92(size, angle);

		//****Communication*****//
		Alice->load_key();

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
		Y.push_back(angle);
		Y.push_back(this->QBER(Alice, Bob));
		QBER_noise.push_back(Y);

		// error estimation, reducing key lenght
		connection.error_estimation(Alice, Bob, 50);
		vector<double> X;
		X.push_back(angle);
		X.push_back(connection.QBER_est);
		QBER_estimation.push_back(X);

		// starting error correction
		connection.Cascade(Alice, Bob, 0.73, 10);
		vector<double> Z;
		Z.push_back(angle);
		Z.push_back(this->QBER(Alice, Bob));
		QBER_correction.push_back(Z);

		angle += step_angle;
	}
	this->QBER_vs_angle = QBER_noise;
	this->QBER_estimation_vs_angle = QBER_estimation;
	this->QBER_correction_vs_angle = QBER_correction;
}

void statistics::simulate_QBER_noise(protocol * Alice, protocol * Bob, protocol * Eve, double min_noise, double max_noise, double step_noise)
{
}

void statistics::simulate_QBER_angle(protocol * Alice, protocol * Bob, protocol * Eve, double min_angle, double max_angle, double step_angle, double noise_level)
{
}

void statistics::print_stats()
{
	cout << "Initial key size:" << this->initial_key_size << endl;
	cout << "Key lenght - no correction:" << this->key_lenght << endl;
	cout << "Key lenght - after correction" << this->key_lenght_correction << endl;
	cout << "QBER:" << this->QBER_S << endl;
	cout << "QBER - estimation:" << this->QBER_est << endl;
	cout << "QBER - after correction:" << this->QBER_S_correction << endl;

}

void statistics::print_charts()
{
}
