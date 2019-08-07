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
	connection.Cascade(Alice, Bob, 0.5, 5);
	connection.privacy_amp(Alice, Bob, 20);  // problem with parameters!

	this->key_lenght_correction = Alice->actual_key_size;
	this->QBER_S_correction = QBER(Alice, Bob);
}

SimulationData statistics::simulate_QBER_noise(protocol* Alice, protocol* Bob, double min_noise, double max_noise, double step_noise)
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

	return { Alice, Bob };
} 

SimulationData statistics::simulate_QBER_angle(protocol * Alice, protocol * Bob, double min_angle, double max_angle, double step_angle, double noise_level)
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

	return { Alice, Bob };
}

SimulationData statistics::simulate_QBER_noise(protocol * Alice, protocol * Bob, protocol * Eve, double min_noise, double max_noise, double step_noise)
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
		delete Eve;

		if (is_B92) {
			Alice = new B92(size, angle);
			Bob = new B92(size, angle);
			Eve = new B92(size, angle);
		}
		else {
			Alice = new BB84(size);
			Bob = new BB84(size);
			Eve = new BB84(size);
		}
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

		//**** End of communication ****//

		vector<double> Y;  // filling real QBER before correction
		Y.push_back(noise_level);
		Y.push_back(this->QBER(Alice, Bob));
		QBER_noise.push_back(Y);

		// error estimation, reducing key lenght
		connection.error_estimation(Alice, Bob, 50);
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

	return { Alice, Bob, Eve };
}

SimulationData statistics::simulate_QBER_angle(protocol * Alice, protocol * Bob, protocol * Eve, double min_angle, double max_angle, double step_angle, double noise_level)
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
		delete Eve;

		Alice = new B92(size, angle);
		Bob = new B92(size, angle);
		Eve = new B92(size, angle);

		//****Communication*****//
		Alice->load_key();

		Eve->load_key();
		Eve->generate_basis();

		Bob->load_key();
		Bob->generate_basis();

		quantum_channel connection(Alice);
		connection.make_noise(noise_level);
		Eve->spy_quantum(&connection);

		Bob->read_quantum(&connection);
		Alice->compare(Bob);

		Eve->spy_classic(Alice);
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

	return { Alice, Bob, Eve };
}

SimulationData statistics::Cascade_convergence(protocol * Alice, protocol * Bob, double noise_level, double init_alpha, double end_alpha, double alpha_step,float error_est_comparison_percent, int C_steps)
{
	double alpha = init_alpha;
	int no_steps = static_cast<int>((end_alpha - init_alpha) / alpha_step);
	int size = Alice->key_size;
	bool is_B92 = Alice->is_B92;
	double angle = 0;
	if (is_B92) {
		angle = dynamic_cast<B92*>(Alice)->alpha;
	}
	vector< vector<double> > QBER_alpha;

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
	if (!is_B92)
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
	for (int i = 0; i < no_steps; i++) {
		quantum_channel temp_channel(connection);    // creating temporal operative objects, we want to work on the same keys each time
		protocol* temp_Alice;
		protocol* temp_Bob;
		if (is_B92) {
			temp_Alice = new B92(*dynamic_cast<B92*>(Alice));
			temp_Bob = new B92(*dynamic_cast<B92*>(Bob));
		}
		else {
			temp_Alice = new BB84(*dynamic_cast<BB84*>(Alice));
			temp_Bob = new BB84(*dynamic_cast<BB84*>(Bob));
		}

		temp_channel.error_estimation(temp_Alice, temp_Bob, error_est_comparison_percent);
		temp_channel.Cascade(temp_Alice, temp_Bob, alpha, C_steps);

		vector<double> temp_vector;  // filling QBER for particular alpha
		temp_vector.push_back(alpha);
		temp_vector.push_back(this->QBER(temp_Alice, temp_Bob));
		QBER_alpha.push_back(temp_vector);

		delete temp_Alice;
		delete temp_Bob;

		alpha += alpha_step;
	}
	this->QBER_vs_alpha = QBER_alpha;
	return { Alice, Bob };
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

void statistics::print_charts(string file_1, string file_2)
{
	fstream data_noise;
	fstream data_angle;
	data_noise.open(file_1, ios::out);
	data_angle.open(file_2, ios::out);

	for (int i = 0; i < this->QBER_vs_noise.size(); i++){
		data_noise << QBER_vs_noise[i][0] << "	" << QBER_vs_noise[i][1] << "	" << QBER_estimation_vs_noise[i][1] << "	" << QBER_correction_vs_noise[i][1] << endl;
	}

	for (int i = 0; i < this->QBER_vs_angle.size(); i++) {
		data_angle << QBER_vs_angle[i][0] << "	" << QBER_vs_angle[i][1] << "	" << QBER_estimation_vs_angle[i][1] << "	" << QBER_correction_vs_angle[i][1] << endl;
	}

	data_noise.close();
	data_angle.close();
}

void statistics::print_cascade_conv(string file_3)
{
	fstream data_C_conv;
	data_C_conv.open(file_3, ios::out);

	for (int i = 0; i < this->QBER_vs_alpha.size(); i++) {
		data_C_conv << QBER_vs_alpha[i][0] << "		" << QBER_vs_alpha[i][1] << endl;
	}

	data_C_conv.close();

}
