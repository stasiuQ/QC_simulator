#pragma once
#include "protocol.h"

using namespace std;

class statistics {
protected:
	vector<double> QBER_vs_noise;
	vector<double> QBER_correction;
	int initial_key_size;
	int key_lenght;
	int key_lenght_correction;
public:
	statistics(protocol* Alice, protocol* Bob);
	statistics(protocol* Alice, protocol* Bob, protocol* Eve);
	double QBER(protocol* Alice, protocol* person);

	vector<double> simulate_QBER_noise(protocol* Alice, protocol* person, double min_noise, double max_noise, double step_noise);
	vector<double> simulate_QBER_angle(protocol* Alice, protocol* person, double min_angle, double max_angle, double step_angle);

};
