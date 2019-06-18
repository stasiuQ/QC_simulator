#pragma once
#include "protocol.h"
#include "BB84.h"
#include "B92.h"

using namespace std;

class statistics {
protected:
	vector< vector<double> > QBER_vs_noise;
	vector< vector<double> > QBER_estimation_vs_noise;
	vector< vector<double> > QBER_correction_vs_noise;    // QBER vs noise after correction

	vector< vector<double> > QBER_vs_angle;
	vector< vector<double> > QBER_estimation_vs_angle;
	vector< vector<double> > QBER_correction_vs_angle;    // QBER vs noise after correction

	int initial_key_size;
	int key_lenght;
	int key_lenght_correction;

	double QBER_S;   // single communication run QBER before error correction
	double QBER_est;
	double QBER_S_correction;

public:
	statistics(protocol* Alice, protocol* Bob, double noise_level);
	statistics(protocol* Alice, protocol* Bob, protocol* Eve, double noise_level);
	double QBER(protocol* Alice, protocol* person);

	void execute_communication(protocol* Alice, protocol* Bob, double noise_level);
	void execute_communication(protocol* Alice, protocol* Bob, protocol* Eve, double noise_level);
	void simulate_QBER_noise(protocol* Alice, protocol* Bob, double min_noise, double max_noise, double step_noise);
	void simulate_QBER_angle(protocol* Alice, protocol* Bob, double min_angle, double max_angle, double step_angle, double noise_level);
	void print_stats();

};
