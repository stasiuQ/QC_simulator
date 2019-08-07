#pragma once
#include "protocol.h"
#include "BB84.h"
#include "B92.h"

using namespace std;

struct SimulationData {   // struct created in order to keep pointers on Alice, Bob and Eve after calling for STATS simulation methods
	protocol* Alice = nullptr;
	protocol* Bob = nullptr;
	protocol* Eve = nullptr;

	void apply(protocol** AlicePtr, protocol** BobPtr, protocol** EvePtr = nullptr) {
		assign(AlicePtr, Alice);
		assign(BobPtr, Bob);
		assign(EvePtr, Eve);
	}

private:
	void assign(protocol** ptr, protocol* value) {
		if (ptr == nullptr) {
			return;
		}
		*ptr = value;
	}
};

class statistics {
protected:
	vector< vector<double> > QBER_vs_noise;
	vector< vector<double> > QBER_estimation_vs_noise;
	vector< vector<double> > QBER_correction_vs_noise;    // QBER vs noise after correction

	vector< vector<double> > QBER_vs_angle;
	vector< vector<double> > QBER_estimation_vs_angle;
	vector< vector<double> > QBER_correction_vs_angle;    // QBER vs noise after correction

	vector< vector<double> > QBER_vs_alpha;

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
	SimulationData simulate_QBER_noise(protocol* Alice, protocol* Bob, double min_noise, double max_noise, double step_noise);
	SimulationData simulate_QBER_angle(protocol* Alice, protocol* Bob, double min_angle, double max_angle, double step_angle, double noise_level);
	SimulationData simulate_QBER_noise(protocol* Alice, protocol* Bob, protocol* Eve, double min_noise, double max_noise, double step_noise);
	SimulationData simulate_QBER_angle(protocol* Alice, protocol* Bob, protocol* Eve, double min_angle, double max_angle, double step_angle, double noise_level);
	SimulationData Cascade_convergence(protocol * Alice, protocol * Bob, double noise_level, double init_alpha, double end_alpha, double alpha_step,float error_est_camparison_percent, int C_steps);
	void print_stats();
	void print_charts(string file_1, string file_2);
	void print_cascade_conv(string file_3);

};
